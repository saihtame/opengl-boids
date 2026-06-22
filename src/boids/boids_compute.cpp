#include "boids_compute.hpp"
#include "shaders/shader.hpp"
#include "shaders/shader_program.hpp"
#include <glad/gl.h>
#include <glm/ext/vector_float3.hpp>
#include <memory>


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

    // Create simulation shader program
    Shaders::Shader sim_shader(shader_sim_path, GL_COMPUTE_SHADER);
    sim_shader_prog = std::make_unique<Shaders::ShaderProgram>();
    sim_shader_prog->attach_shader(sim_shader);
    sim_shader_prog->link();
}

void BoidsCompute::compute(float delta, BoidsData& data) {
    // Run compute shaders
    run_grid_key_shader(data);
    run_grid_hist_shader(data);
    //run_grid_sort_shader(data);
    run_sim_shader(delta, data);

    // Switch instance data buffers
    data.switch_instance_buffers();
}

inline void BoidsCompute::run_grid_key_shader(const BoidsData& data) {
    // Prepare cell key shader program
    spatial_key_shader_prog->use();
    spatial_key_shader_prog->set_uniform_uint("boidCount", params->boids);
    spatial_key_shader_prog->set_uniform_vec3("bounds", params->bounds);
    spatial_key_shader_prog->set_uniform_float("cellSize", params->view_range);
    spatial_key_shader_prog->set_uniform_ivec3("gridSize", data.spatial_grid_size);

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

inline void BoidsCompute::run_grid_sort_shader(const BoidsData& ) {

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
    sim_shader_prog->set_uniform_ivec3("gridSize", data.spatial_grid_size);

    // Bind instances data input uniform
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, data.instances_BO_A);
    // Bind instances data output uniform
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, data.instances_BO_B);

    // Dispatch shader programs
    int dispatches = (initialized_boids + work_group_size - 1) / work_group_size;
    glDispatchCompute(dispatches, 1, 1);
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
}

}
