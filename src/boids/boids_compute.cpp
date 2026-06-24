#include "boids_compute.hpp"
#include "boids/boids_data.hpp"
#include "shaders/shader.hpp"
#include "shaders/shader_program.hpp"
#include <cstdint>
#include <glad/gl.h>
#include <glm/ext/vector_float3.hpp>
#include <iostream>
#include <memory>
#include <sys/types.h>


#define VALIDATE_SHADER_RESULTS

namespace ParticleSim::Boids {

BoidsCompute::BoidsCompute(const std::shared_ptr<BoidsParams>& parameters)
    : initialized_boids(parameters->boids), params(parameters) {
    // Create spatial cell key shader program
    Shaders::Shader cell_key_shader(spatial_key_shader_path, GL_COMPUTE_SHADER);
    spatial_key_shader_prog = std::make_unique<Shaders::ShaderProgram>();
    spatial_key_shader_prog->attach_shader(cell_key_shader);
    spatial_key_shader_prog->link();

    // Create spatial cell key shader program
    Shaders::Shader cell_hist_shader(spatial_hist_shader_path, GL_COMPUTE_SHADER);
    spatial_hist_shader_prog = std::make_unique<Shaders::ShaderProgram>();
    spatial_hist_shader_prog->attach_shader(cell_key_shader);
    spatial_hist_shader_prog->link();

    // Create spatial grid sort shader program
    Shaders::Shader grid_sort_shader(spatial_radix_shader_path, GL_COMPUTE_SHADER);
    spatial_sort_shader_prog = std::make_unique<Shaders::ShaderProgram>();
    spatial_sort_shader_prog->attach_shader(grid_sort_shader);
    spatial_sort_shader_prog->link();

    // Create spatial post shader program
    Shaders::Shader grid_post_shader(spatial_post_shader_path, GL_COMPUTE_SHADER);
    spatial_post_shader_prog = std::make_unique<Shaders::ShaderProgram>();
    spatial_post_shader_prog->attach_shader(grid_post_shader);
    spatial_post_shader_prog->link();

    // Create simulation shader program
    Shaders::Shader sim_shader(shader_sim_path, GL_COMPUTE_SHADER);
    sim_shader_prog = std::make_unique<Shaders::ShaderProgram>();
    sim_shader_prog->attach_shader(sim_shader);
    sim_shader_prog->link();
}

void BoidsCompute::compute(float delta, BoidsData& data) {
    /*---- Run Compute Shaders ----*/
    // Grid key shader
    run_grid_key_shader(data);
    #ifdef VALIDATE_SHADER_RESULTS
    validate_grid_key_shader(data);
    #endif
    // Grid key histogram shader
    run_grid_hist_shader(data);
    #ifdef VALIDATE_SHADER_RESULTS
    validate_grid_hist_shader(data);
    #endif
    // Grid key sorting shader
    run_grid_sort_shader(data);
    #ifdef VALIDATE_SHADER_RESULTS
    validate_grid_radix_shader(data);
    #endif
    // Grid post shader
    run_grid_post_shader(data);
    #ifdef VALIDATE_SHADER_RESULTS
    validate_grid_post_shader(data);
    #endif
    // Sim shader
    run_sim_shader(delta, data);

    // Switch instance data buffers
    data.switch_instance_buffers();
}

inline void BoidsCompute::run_grid_key_shader(const BoidsData& data) {
    // Prepare cell key shader program
    spatial_key_shader_prog->use();
    spatial_key_shader_prog->set_uniform_uint("boidCount", initialized_boids);

    // Bind instances data input uniform
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, data.instances_BO_A);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, data.spatial_grid_entries_A);

    // Dispatch shader program
    int dispatches = (initialized_boids + work_group_size - 1) / work_group_size;
    glDispatchCompute(dispatches, 1, 1);
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
}

inline void BoidsCompute::run_grid_hist_shader(const BoidsData& data) {
    spatial_hist_shader_prog->use();
    spatial_hist_shader_prog->set_uniform_uint("boidCount", params->boids);

    // Bind entries and histogram buffers
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, data.spatial_grid_entries_A);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, data.spatial_grid_hist_BO);

    // Dispatch shader program
    int dispatches = (initialized_boids + work_group_size - 1) / work_group_size;
    glDispatchCompute(dispatches, 1, 1);
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
}

inline void BoidsCompute::run_grid_sort_shader(BoidsData& data) {
    spatial_sort_shader_prog->use();
    spatial_sort_shader_prog->set_uniform_uint("boidCount", params->boids);

    // Bind buffers
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, data.spatial_grid_hist_BO);

    // Dispatch shader in multiple passes
    for (uint32_t p = 0; p < BoidsData::grid_radix_passes; p++) {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, data.spatial_grid_entries_A);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, data.spatial_grid_entries_B);
        spatial_sort_shader_prog->set_uniform_uint("pass", p);
        glDispatchCompute(1, 1, 1);
        glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
        data.switch_entries_buffers();
    }
}

inline void BoidsCompute::run_grid_post_shader(BoidsData& data) {
    // Prepare shader program
    spatial_post_shader_prog->use();
    spatial_post_shader_prog->set_uniform_uint("boidCount", params->boids);
    spatial_post_shader_prog->set_uniform_uvec3("gridSize", data.spatial_grid_size);
    // Bind buffers
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, data.spatial_grid_entries_A);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, data.spatial_grid_cells_BO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, data.spatial_grid_elements_BO);

    // Dispatch
    int dispatches = (initialized_boids + work_group_size - 1) / work_group_size;
    glDispatchCompute(dispatches, 1, 1);
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
}

inline void BoidsCompute::run_sim_shader(float delta, const BoidsData& data) {
    // Prepare simulation shader program
    sim_shader_prog->use();
    sim_shader_prog->set_uniform_uint("boidCount", initialized_boids);
    sim_shader_prog->set_uniform_vec3("bounds", params->bounds);
    sim_shader_prog->set_uniform_float("boidMaxSpeed", params->boid_max_speed);
    sim_shader_prog->set_uniform_float("boidMinSpeed", params->boid_min_speed);
    sim_shader_prog->set_uniform_float("viewRange", params->view_range);
    sim_shader_prog->set_uniform_float("viewCosine", params->view_cosine);
    sim_shader_prog->set_uniform_float("collisionAvoidanceStrength", params->collision_avoidance_strength);
    sim_shader_prog->set_uniform_float("acceleration", params->acceleration);
    sim_shader_prog->set_uniform_float("seperationFactor", params->seperation_factor);
    sim_shader_prog->set_uniform_float("alignmentFactor", params->alignment_factor);
    sim_shader_prog->set_uniform_float("cohesionFactor", params->cohesion_factor);
    sim_shader_prog->set_uniform_float("delta", delta);
    sim_shader_prog->set_uniform_uvec3("gridSize", data.spatial_grid_size);

    // Bind instances data input uniform
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, data.instances_BO_A);
    // Bind instances data output uniform
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, data.instances_BO_B);
    // Bind spatial grid data
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, data.spatial_grid_entries_A);

    // Dispatch shader programs
    int dispatches = (initialized_boids + work_group_size - 1) / work_group_size;
    glDispatchCompute(dispatches, 1, 1);
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
}

/*---- Validation Functions used for debugging ----*/
std::array<uint32_t, 2> mortonEncode(uint32_t x, uint32_t y, uint32_t z) {
    std::array<uint32_t, 2> key = {0u, 0u};

    for (uint32_t i = 0; i < 21u; ++i) {
        uint32_t xBit = (x >> i) & 1u;
        uint32_t yBit = (y >> i) & 1u;
        uint32_t zBit = (z >> i) & 1u;

        uint32_t xPos = 3u * i;
        uint32_t yPos = xPos + 1u;
        uint32_t zPos = xPos + 2u;

        if (xPos < 32u) key[0] |= xBit << xPos; else key[1] |= xBit << (xPos - 32u);
        if (yPos < 32u) key[0] |= yBit << yPos; else key[1] |= yBit << (yPos - 32u);
        if (zPos < 32u) key[0] |= zBit << zPos; else key[1] |= zBit << (zPos - 32u);
    }
    return key;
}

inline void BoidsCompute::validate_grid_key_shader(const BoidsData& data) {
    /*---- Copy data from GPU to CPU ----*/
    // Instance buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, data.instances_BO_A);
    BoidsData::Instance* instance_buffer = (BoidsData::Instance*)malloc(data.instances_buffer_size);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, data.instances_buffer_size, instance_buffer);
    // Entries buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, data.spatial_grid_entries_A);
    BoidsData::SortEntry* entries_buffer = (BoidsData::SortEntry*)malloc(data.grid_entries_buffer_size);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, data.grid_entries_buffer_size, entries_buffer);

    /*---- Validate Correct Data ----*/
    for (uint32_t i = 0; i < initialized_boids; i++) {
        // Check entries index being written correct
        auto entry = entries_buffer[i];
        if (entry.value != i) {
            std::cout << "Unexpected SortEntry value: " << entries_buffer[i].value << "\tExpected value: " << i << std::endl;
        }

        // Check that morton encoding is done correctly
        auto instance = instance_buffer[i];
        auto goodKey = mortonEncode(instance.grid_pos_x, instance.grid_pos_y, instance.grid_pos_z);
        if (entry.key[0] != goodKey[0]) {
            std::cerr << "Unexpected SortEntry lower key: " << entry.key[0] << "\tExpected value: " << goodKey[0] << std::endl;
        }
        if (entry.key[1] != goodKey[1]) {
            std::cerr << "Unexpected SortEntry upper key: " << entry.key[1] << "\tExpected value: " << goodKey[1] << std::endl;
        }
    }

    /*---- Cleanup ----*/
    free(instance_buffer);
    free(entries_buffer);
}

inline void BoidsCompute::validate_grid_hist_shader(const BoidsData& data) {
    /*---- Copy data from GPU to CPU ----*/
    // Entries
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, data.spatial_grid_entries_A);
    BoidsData::SortEntry* entries_buffer = (BoidsData::SortEntry*)malloc(data.grid_entries_buffer_size);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, data.grid_entries_buffer_size, entries_buffer);
    // Histograms
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, data.spatial_grid_hist_BO);
    BoidsData::Histogram* histograms_buffer = (BoidsData::Histogram*)malloc(data.grid_hist_buffer_size);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, data.grid_hist_buffer_size, entries_buffer);

    /*---- Cleanup ----*/
    free(entries_buffer);
    free(histograms_buffer);
}

inline void BoidsCompute::validate_grid_radix_shader(const BoidsData& ) {

}

inline void BoidsCompute::validate_grid_post_shader(const BoidsData& ) {

}

}
