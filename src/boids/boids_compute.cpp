#include "boids_compute.hpp"
#include "boids/boids_data.hpp"
#include "shaders/shader.hpp"
#include "shaders/shader_program.hpp"
#include <SDL3/SDL_timer.h>
#include <array>
#include <cstdint>
#include <glad/gl.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_uint3.hpp>
#include <iostream>
#include <memory>
#include <sys/types.h>
#include <unordered_set>
#include <iomanip>


//#define VALIDATE_SHADER_RESULTS
#define TIME_SHADER_STAGES

namespace ParticleSim::Boids {

#ifdef VALIDATE_SHADER_RESULTS
inline void validate_grid_key_shader(const BoidsData& data);
inline void validate_grid_hist_shader(const BoidsData& data);
inline void validate_grid_radix_shader(const BoidsData& data);
inline void validate_grid_post_shader(const BoidsData& data);
#endif

BoidsCompute::BoidsCompute(const std::shared_ptr<BoidsParams>& parameters)
    :  params(parameters) {
    // Create spatial cell key shader program
    Shaders::Shader cell_key_shader(spatial_key_shader_path, GL_COMPUTE_SHADER);
    spatial_key_shader_prog = std::make_unique<Shaders::ShaderProgram>();
    spatial_key_shader_prog->attach_shader(cell_key_shader);
    spatial_key_shader_prog->link();

    // Create spatial cell key shader program
    Shaders::Shader cell_hist_shader(spatial_hist_shader_path, GL_COMPUTE_SHADER);
    spatial_hist_shader_prog = std::make_unique<Shaders::ShaderProgram>();
    spatial_hist_shader_prog->attach_shader(cell_hist_shader);
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
    #ifdef TIME_SHADER_STAGES
    uint64_t timer_freq = SDL_GetPerformanceFrequency();
    uint64_t start_time = SDL_GetPerformanceCounter();
    uint64_t total_time = 0u;
    std::cout << std::fixed << std::setprecision(8);
    #endif
    /*---- Run Compute Shaders ----*/
    // Grid key shader
    run_grid_key_shader(data);
    #ifdef TIME_SHADER_STAGES
    uint64_t stop_time = SDL_GetPerformanceCounter();
    uint64_t delta_time = stop_time - start_time;
    total_time += delta_time;
    std::cout << "       Grid Key Shader Time\t" << double(delta_time) / double(timer_freq) << "s\n";
    start_time = SDL_GetPerformanceCounter();
    #endif
    #ifdef VALIDATE_SHADER_RESULTS
    validate_grid_key_shader(data);
    #endif
    // Grid key histogram shader
    run_grid_hist_shader(data);
    #ifdef TIME_SHADER_STAGES
    stop_time = SDL_GetPerformanceCounter();
    delta_time = stop_time - start_time;
    total_time += delta_time;
    std::cout << "Grid Histograms Shader Time\t" << double(delta_time) / double(timer_freq) << "s\n";
    start_time = SDL_GetPerformanceCounter();
    #endif
    #ifdef VALIDATE_SHADER_RESULTS
    validate_grid_hist_shader(data);
    #endif
    // Grid key sorting shader
    run_grid_sort_shader(data);
    #ifdef TIME_SHADER_STAGES
    stop_time = SDL_GetPerformanceCounter();
    delta_time = stop_time - start_time;
    total_time += delta_time;
    std::cout << "      Grid Sort Shader Time\t" << double(delta_time) / double(timer_freq) << "s\n";
    start_time = SDL_GetPerformanceCounter();
    #endif
    #ifdef VALIDATE_SHADER_RESULTS
    validate_grid_radix_shader(data);
    #endif
    // Grid post shader
    run_grid_post_shader(data);
    #ifdef TIME_SHADER_STAGES
    stop_time = SDL_GetPerformanceCounter();
    delta_time = stop_time - start_time;
    total_time += delta_time;
    std::cout << " Grid Post Sort Shader Time\t" << double(delta_time) / double(timer_freq) << "s\n";
    start_time = SDL_GetPerformanceCounter();
    #endif
    #ifdef VALIDATE_SHADER_RESULTS
    validate_grid_post_shader(data);
    #endif
    // Sim shader
    run_sim_shader(delta, data);
    #ifdef TIME_SHADER_STAGES
    stop_time = SDL_GetPerformanceCounter();
    delta_time = stop_time - start_time;
    total_time += delta_time;
    std::cout << "            Sim Shader Time\t" << double(delta_time) / double(timer_freq) << "s\n";
    start_time = SDL_GetPerformanceCounter();
    std::cout << "                 Total Time\t" << float(total_time) / float(timer_freq) << "s" << std::endl;
    #endif
}

inline void BoidsCompute::run_grid_key_shader(const BoidsData& data) {
    // Prepare cell key shader program
    spatial_key_shader_prog->use();
    spatial_key_shader_prog->set_uniform_uint("boidCount", data.initialized_boids);

    // Bind instances data input uniform
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, data.instances_BO_A);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, data.spatial_grid_entries_A);

    // Dispatch shader program
    int dispatches = (data.initialized_boids + work_group_size - 1) / work_group_size;
    glDispatchCompute(dispatches, 1, 1);
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
}

inline void BoidsCompute::run_grid_hist_shader(const BoidsData& data) {
    // Clear histogram buffer before running
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, data.spatial_grid_hist_BO);
    GLuint zero = 0;
    glClearBufferData(GL_SHADER_STORAGE_BUFFER,
                    GL_R32UI,
                    GL_RED_INTEGER,
                    GL_UNSIGNED_INT,
                    &zero);
    glMemoryBarrier(GL_BUFFER_UPDATE_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

    // Prepare shader
    spatial_hist_shader_prog->use();
    spatial_hist_shader_prog->set_uniform_uint("boidCount", data.initialized_boids);

    // Bind entries and histogram buffers
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, data.spatial_grid_entries_A);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, data.spatial_grid_hist_BO);

    // Dispatch shader program
    int dispatches = (data.initialized_boids + work_group_size - 1) / work_group_size;
    glDispatchCompute(dispatches, 1, 1);
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
}

inline void BoidsCompute::run_grid_sort_shader(BoidsData& data) {
    spatial_sort_shader_prog->use();
    spatial_sort_shader_prog->set_uniform_uint("boidCount", data.initialized_boids);

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
    // Clear grid cells buffer before running
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, data.spatial_grid_cells_BO);
    GLuint zero = 0;
    glClearBufferData(GL_SHADER_STORAGE_BUFFER,
                    GL_R32UI,
                    GL_RED_INTEGER,
                    GL_UNSIGNED_INT,
                    &zero);
    glMemoryBarrier(GL_BUFFER_UPDATE_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

    // Prepare shader program
    spatial_post_shader_prog->use();
    spatial_post_shader_prog->set_uniform_uint("boidCount", data.initialized_boids);
    spatial_post_shader_prog->set_uniform_ivec3("gridSize", data.spatial_grid_size);
    // Bind buffers
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, data.spatial_grid_entries_A);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, data.spatial_grid_cells_BO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, data.spatial_grid_elements_BO);

    // Dispatch
    int dispatches = (data.initialized_boids + work_group_size - 1) / work_group_size;
    glDispatchCompute(dispatches, 1, 1);
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
}

inline void BoidsCompute::run_sim_shader(float delta, BoidsData& data) {
    // Prepare simulation shader program
    sim_shader_prog->use();
    sim_shader_prog->set_uniform_uint("boidCount", data.initialized_boids);
    sim_shader_prog->set_uniform_vec3("bounds", data.initialized_bounds);
    sim_shader_prog->set_uniform_float("boidMaxSpeed", params->boid_max_speed);
    sim_shader_prog->set_uniform_float("boidMinSpeed", params->boid_min_speed);
    sim_shader_prog->set_uniform_float("viewRange", data.initialized_view_range);
    sim_shader_prog->set_uniform_float("viewCosine", params->view_cosine);
    sim_shader_prog->set_uniform_float("collisionAvoidanceStrength", params->collision_avoidance_strength);
    sim_shader_prog->set_uniform_float("acceleration", params->acceleration);
    sim_shader_prog->set_uniform_float("seperationFactor", params->seperation_factor);
    sim_shader_prog->set_uniform_float("alignmentFactor", params->alignment_factor);
    sim_shader_prog->set_uniform_float("cohesionFactor", params->cohesion_factor);
    sim_shader_prog->set_uniform_float("delta", delta);
    sim_shader_prog->set_uniform_vec3("cellSize", data.spatial_grid_cell_size);
    sim_shader_prog->set_uniform_uvec3("gridSize", data.spatial_grid_size);

    // Bind instances data input and output buffers
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, data.instances_BO_A);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, data.instances_BO_B);
    // Bind spatial grid data
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, data.spatial_grid_cells_BO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, data.spatial_grid_elements_BO);

    // Dispatch shader programs
    int dispatches = (data.initialized_boids + work_group_size - 1) / work_group_size;
    glDispatchCompute(dispatches, 1, 1);
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

    // Switch instance data buffers
    data.switch_instance_buffers();
}

/*---- Validation Functions used for debugging ----*/
uint32_t mortonEncode(uint32_t x, uint32_t y, uint32_t z) {
    uint32_t key = 0u;

    for (uint32_t i = 0; i < 10u; ++i) {
        uint32_t xBit = (x >> i) & 1u;
        uint32_t yBit = (y >> i) & 1u;
        uint32_t zBit = (z >> i) & 1u;

        uint32_t xPos = 3u * i;
        uint32_t yPos = xPos + 1u;
        uint32_t zPos = xPos + 2u;

        key |= xBit << xPos;
        key |= yBit << yPos;
        key |= zBit << zPos;
    }
    return key;
}

inline void validate_grid_key_shader(const BoidsData& data) {
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
    for (uint32_t i = 0; i < data.initialized_boids; i++) {
        // Check entries index being written correct
        auto entry = entries_buffer[i];
        if (entry.value != i) {
            std::cout << "Unexpected SortEntry value: " << entries_buffer[i].value << "\tExpected value: " << i << std::endl;
        }

        // Check that morton encoding is done correctly
        auto instance = instance_buffer[i];
        auto goodKey = mortonEncode(instance.grid_pos_x, instance.grid_pos_y, instance.grid_pos_z);
        if (entry.key != goodKey) {
            std::cerr << "Unexpected SortEntry key: " << entry.key << "\tExpected value: " << goodKey << std::endl;
        }
    }

    /*---- Cleanup ----*/
    free(instance_buffer);
    free(entries_buffer);
}

std::array<BoidsData::Histogram, BoidsData::grid_radix_passes> get_histograms(BoidsData::SortEntry* entries, uint32_t entriesCount) {
    auto hists = std::array<BoidsData::Histogram, BoidsData::grid_radix_passes>();
    uint32_t bitMask = 0;
    for (uint i = 0; i < BoidsData::grid_radix_bits; i++) {
        bitMask += 0x1 << i;
    }

    for (uint i = 0; i < entriesCount; i++) {
        // Add to local histogram value for each pass
        // First 32 bits
        for (uint px = 0u; px < (BoidsData::grid_radix_passes / 2u); px++) {
            uint shift = px * BoidsData::grid_radix_bits;
            // Determine bucket of value
            uint b = (entries[i].key >> shift) & bitMask;
            // Add 1 to bucket count
            hists[px].buckets[b] += 1;
        }
    }

    return hists;
}

inline void validate_grid_hist_shader(const BoidsData& data) {
    /*---- Copy data from GPU to CPU ----*/
    // Entries
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, data.spatial_grid_entries_A);
    BoidsData::SortEntry* entries_buffer = (BoidsData::SortEntry*)malloc(data.grid_entries_buffer_size);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, data.grid_entries_buffer_size, entries_buffer);
    // Histograms
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, data.spatial_grid_hist_BO);
    BoidsData::Histogram* histograms_buffer = (BoidsData::Histogram*)malloc(data.grid_hist_buffer_size);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, data.grid_hist_buffer_size, histograms_buffer);

    /*---- Validate Correct Data ----*/
    auto goodHists = get_histograms(entries_buffer, data.initialized_boids);
    for (uint p = 0; p < BoidsData::grid_radix_passes; p++) {
        uint shader_pass_total = 0;
        uint test_pass_total = 0;
        for (uint b = 0; b < (64 / BoidsData::grid_radix_bits); b++) {
            uint shader_bucket = histograms_buffer[p].buckets[b];
            uint test_bucket = goodHists[p].buckets[b];
            // Validate bucket
            if (test_bucket != shader_bucket) {
                std::cerr << "\033[31mBucket value: " << shader_bucket << "\tExpected value: " << test_bucket;
                std::cerr << "\tPass: " << p << "\tBucket: " << b << "\033[0m" << std::endl;
            }
            shader_pass_total += shader_bucket;
            test_pass_total += test_bucket;
        }
        // Validate pass histogram
        if (shader_pass_total != test_pass_total) {
            std::cerr << "\033[31mhist pass total: " << shader_pass_total << "\tExpected value: " << test_pass_total << "\t";
            std::cerr << "Pass: " << p << "\033[0m" << std::endl;
        }
    }

    /*---- Cleanup ----*/
    free(entries_buffer);
    free(histograms_buffer);
}

inline void validate_grid_radix_shader(const BoidsData& data) {
    // Entries
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, data.spatial_grid_entries_A);
    BoidsData::SortEntry* entries_buffer = (BoidsData::SortEntry*)malloc(data.grid_entries_buffer_size);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, data.grid_entries_buffer_size, entries_buffer);

    // Validate entries
    std::unordered_set<uint32_t> seen_values;
    uint32_t lastKey = 0u;
    for (uint i = 0; i < data.initialized_boids; i++) {
        BoidsData::SortEntry e = entries_buffer[i];
        // Check for double entry
        if (seen_values.contains(e.value)) {
            std::cerr << "\033[31mSort Double Value:\t" << e.value << "\033[0m" << std::endl;
        } else {
            seen_values.emplace(e.value);
        }
        // Check too high valueinitialized_boids(parameters->boids),
        if (e.value >= data.initialized_boids) {
            std::cerr << "\033[31mSort Too High Value:\t" << e.value << "\033[0m" << std::endl;
        }
        // Check order
        if (e.key < lastKey) {
            std::cerr << "\033[31mSort Unordered Keys:\t" << lastKey << "\t" << e.key << "\033[0m" << std::endl;
        }
        lastKey = e.key;
    }
    // Ensure all values are there
    for (uint32_t i = 0; i < data.initialized_boids; i++) {
        if (!seen_values.contains(i)) {
            std::cerr << "\033[31mSort Missing Value:\t" << i << "\033[0m" << std::endl;
        }
    }
}

uint getGridIndex(glm::uvec3 pos, glm::uvec3 grid_size) {
    return pos.x + pos.y * grid_size.x + pos.z * grid_size.x * grid_size.y;
}

inline void validate_grid_post_shader(const BoidsData& data) {
    /*---- Copy data from GPU to CPU ----*/
    // Cells Bufffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, data.spatial_grid_cells_BO);
    BoidsData::GridCell* grid_cells = (BoidsData::GridCell*)malloc(data.grid_cells_buffer_size);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, data.grid_cells_buffer_size, grid_cells);
    // Elements Bufffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, data.spatial_grid_elements_BO);
    uint32_t* elements_buffer = (uint32_t*)malloc(data.grid_elements_buffer_size);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, data.grid_elements_buffer_size, elements_buffer);
    // Instances buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, data.instances_BO_A);
    BoidsData::Instance* instances_buffer = (BoidsData::Instance*)malloc(data.instances_buffer_size);
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, data.instances_buffer_size, instances_buffer);

    // Check all grid cells
    uint total_instances = 0u;
    for (int x = 0; x < data.spatial_grid_size.x; x++) {
        for (int y = 0; y < data.spatial_grid_size.y; y++) {
            for (int z  = 0; z < data.spatial_grid_size.z; z++) {
                BoidsData::GridCell cell = grid_cells[getGridIndex(glm::uvec3(x, y, z), data.spatial_grid_size)];
                // Iterate all elements in cell
                for (uint i = cell.StartOffset; i < cell.EndOffset; i++) {
                    uint32_t element_ind = elements_buffer[i];
                    BoidsData::Instance instance = instances_buffer[element_ind];
                    total_instances++;

                    if (instance.grid_pos_x != (uint)x || instance.grid_pos_y != (uint)y || instance.grid_pos_z != (uint)z) {
                        std::cerr << "\033[31mElement in grid cell, does not have right grid pos:\t" << "\n";
                        std::cerr << "X Value:\t" << instance.grid_pos_x << "\tExpected:\t" << x << "\n";
                        std::cerr << "Y Value:\t" << instance.grid_pos_y << "\tExpected:\t" << y << "\n";
                        std::cerr << "Z value:\t" << instance.grid_pos_z << "\tExpected:\t" << z << "\033[0m" << std::endl;
                    }
                }
            }
        }
    }
    if (total_instances != data.initialized_boids) {
        std::cerr << "\033[31mElement count wrong:\t" << total_instances << "\tExpected value:\t" << data.initialized_boids << "\033[0m" << std::endl;
    }
}

}
