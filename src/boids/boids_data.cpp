#include "boids_data.hpp"
#include <glad/gl.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_uint3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/random.hpp>
#include <memory>
#include <sys/types.h>


namespace ParticleSim::Boids {

BoidsData::BoidsData(const std::shared_ptr<Render::Mesh::Mesh>& mesh, const std::shared_ptr<BoidsParams>& params)
    : Render::Mesh::MeshRenderData(mesh) {
    initialized_boids = params->boids;
    spatial_grid_cell_size = glm::vec3(params->view_range, params->view_range, params->view_range);
    spatial_grid_size = glm::ivec3(
        glm::ceil(params->bounds.x / spatial_grid_cell_size.x),
        glm::ceil(params->bounds.y / spatial_grid_cell_size.y),
        glm::ceil(params->bounds.z / spatial_grid_cell_size.z));

    // Prepare initial instances data
    std::vector<Instance> data;
    data.resize(params->boids);
    for (int i = 0; i < params->boids; i++) {
        // Position data
        glm::vec3 pos = glm::vec3(
            glm::linearRand(1.0f, params->bounds.x - 1.0f),
            glm::linearRand(1.0f, params->bounds.y - 1.0f),
            glm::linearRand(1.0f, params->bounds.z - 1.0f)
        );
        data[i].pos_x       = pos.x;
        data[i].pos_y       = pos.y;
        data[i].pos_z       = pos.z;
        data[i].pos_padding = 0.0f; // Padding
        // Velocity data
        glm::vec3 vel = glm::normalize(glm::vec3(
            glm::linearRand(-1.0, 1.0),
            glm::linearRand(-1.0, 1.0),
            glm::linearRand(-1.0, 1.0)
        )) * glm::linearRand(params->boid_min_speed, params->boid_max_speed);
        data[i].vel_x       = vel.x;
        data[i].vel_y       = vel.y;
        data[i].vel_z       = vel.z;
        data[i].vel_padding = 0.0f; // Padding
        // grid cell
        glm::uvec3 grid_cell = glm::uvec3(glm::ceil(pos / params->view_range));
        data[i].grid_pos_x       = grid_cell.x;
        data[i].grid_pos_y       = grid_cell.y;
        data[i].grid_pos_z       = grid_cell.z;
        data[i].grid_pos_padding = 0.0f;
    }

    // Create instances buffers
    glGenBuffers(1, &instances_BO_A);
    glGenBuffers(1, &instances_BO_B);
    // Copy data over to buffer A
    glBindBuffer(GL_ARRAY_BUFFER, instances_BO_A);
    instances_buffer_size = data.size() * sizeof(Instance);
    glBufferData(GL_ARRAY_BUFFER, instances_buffer_size, data.data(), GL_DYNAMIC_COPY);
    // Reserve space in buffer B, without copying data to it
    glBindBuffer(GL_ARRAY_BUFFER, instances_BO_B);
    glBufferData(GL_ARRAY_BUFFER, instances_buffer_size, NULL, GL_DYNAMIC_COPY);
    // Set vao attributes
    set_vao_attributes();

    // Create spatial grid cells buffer
    int total_cells = spatial_grid_size.x + spatial_grid_size.y + spatial_grid_size.z;
    grid_cells_buffer_size = total_cells * sizeof(GridCell);
    glGenBuffers(1, &spatial_grid_cells_BO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, spatial_grid_cells_BO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, grid_cells_buffer_size, NULL, GL_DYNAMIC_COPY);

    // Create spatial grid elements buffer
    grid_elements_buffer_size = params->boids * sizeof(uint32_t);
    glGenBuffers(1, &spatial_grid_elements_BO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, spatial_grid_elements_BO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, grid_elements_buffer_size, NULL, GL_DYNAMIC_COPY);

    // Create spatial grid histogram buffer
    glGenBuffers(1, &spatial_grid_hist_BO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, spatial_grid_hist_BO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, grid_hist_buffer_size, NULL, GL_DYNAMIC_COPY);

    // Create the element entries buffers
    glGenBuffers(1, &spatial_grid_entries_A);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, spatial_grid_entries_A);
    grid_entries_buffer_size = params->boids * sizeof(SortEntry);
    glBufferData(GL_SHADER_STORAGE_BUFFER, grid_entries_buffer_size, NULL, GL_DYNAMIC_COPY);
    glGenBuffers(1, &spatial_grid_entries_B);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, spatial_grid_entries_B);
    glBufferData(GL_SHADER_STORAGE_BUFFER, grid_entries_buffer_size, NULL, GL_DYNAMIC_COPY);
}

BoidsData::~BoidsData() {
    glDeleteBuffers(1, &instances_BO_A);
    glDeleteBuffers(1, &instances_BO_B);
    glDeleteBuffers(1, &spatial_grid_cells_BO);
    glDeleteBuffers(1, &spatial_grid_elements_BO);
    glDeleteBuffers(1, &spatial_grid_hist_BO);
    glDeleteBuffers(1, &spatial_grid_entries_A);
    glDeleteBuffers(1, &spatial_grid_entries_B);
    instances_BO_A = 0;
    instances_BO_B = 0;
    spatial_grid_cells_BO = 0;
    spatial_grid_elements_BO = 0;
    spatial_grid_hist_BO = 0;
    spatial_grid_entries_A = 0;
    spatial_grid_entries_B = 0;
}

void BoidsData::switch_instance_buffers() {
    // Switch buffers
    unsigned int tmp = instances_BO_A;
    instances_BO_A = instances_BO_B;
    instances_BO_B = tmp;

    // Reset VAO attributes
    set_vao_attributes();
}

void BoidsData::switch_entries_buffers() {
    unsigned int tmp = spatial_grid_entries_A;
    spatial_grid_entries_A = spatial_grid_entries_B;
    spatial_grid_entries_B = tmp;
}

void BoidsData::set_vao_attributes() {
    // Bind VAO
    glBindVertexArray(VAO);
    // Bind instance buffer
    glBindBuffer(GL_ARRAY_BUFFER, instances_BO_A);
    // Set attributes
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Instance), (void*)0);
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Instance), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(4);
    glVertexAttribDivisor(4, 1);
}

}
