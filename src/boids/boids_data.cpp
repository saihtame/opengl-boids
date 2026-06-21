#include "boids_data.hpp"
#include <glad/gl.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_int3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/random.hpp>
#include <memory>


namespace ParticleSim::Boids {

BoidsData::BoidsData(const std::shared_ptr<Render::Mesh::Mesh>& mesh, const std::shared_ptr<BoidsParams>& params)
    : Render::Mesh::MeshRenderData(mesh) {
    // Prepare instances data
    std::vector<float> data;
    data.resize(params->boids * instance_size);
    for (int i = 0; i < params->boids; i++) {
        // Position data
        glm::vec3 pos = glm::vec3(
            glm::linearRand(1.0f, params->bounds.x - 1.0f),
            glm::linearRand(1.0f, params->bounds.y - 1.0f),
            glm::linearRand(1.0f, params->bounds.z - 1.0f)
        );
        data[i * instance_size]     = pos.x;
        data[i * instance_size + 1] = pos.y;
        data[i * instance_size + 2] = pos.z;
        data[i * instance_size + 3] = 0.0f; // Padding
        // Velocity data
        glm::vec3 vel = glm::normalize(glm::vec3(
            glm::linearRand(-1.0, 1.0),
            glm::linearRand(-1.0, 1.0),
            glm::linearRand(-1.0, 1.0)
        )) * glm::linearRand(params->boid_min_speed, params->boid_max_speed);
        data[i * instance_size + 4] = vel.x;
        data[i * instance_size + 5] = vel.y;
        data[i * instance_size + 6] = vel.z;
        data[i * instance_size + 7] = 0.0f; // Padding
        // grid cell
        glm::ivec3 grid_cell = glm::ivec3(pos / params->view_range);
        data[i * instance_size + 8] = grid_cell.x;
        data[i * instance_size + 9] = grid_cell.y;
        data[i * instance_size + 10] = grid_cell.z;
        data[i * instance_size + 11] = 0.0f;
    }

    // Create instances buffers
    glGenBuffers(1, &instances_BO_A);
    glGenBuffers(1, &instances_BO_B);
    // Copy data over to buffer A
    glBindBuffer(GL_ARRAY_BUFFER, instances_BO_A);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_DYNAMIC_COPY);
    // Reserve space in buffer B, without copying data to it
    glBindBuffer(GL_ARRAY_BUFFER, instances_BO_B);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), NULL, GL_DYNAMIC_COPY);

    // Set vao attributes
    set_attributes();

    // Create spatial grid cells buffer
    glm::ivec3 grid_size = glm::ivec3(
        params->bounds.x / params->view_range + 1,
        params->bounds.y / params->view_range + 1,
        params->bounds.z / params->view_range + 1);
    glGenBuffers(1, &spatial_grid_cells_BO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, spatial_grid_cells_BO);
    //glBufferData(GL_SHADER_STORAGE_BUFFER, );
}

BoidsData::~BoidsData() {
    glDeleteBuffers(1, &instances_BO_A);
    glDeleteBuffers(1, &instances_BO_B);
    instances_BO_A = 0;
    instances_BO_B = 0;
}

void BoidsData::switch_instance_buffers() {
    // Switch buffers
    unsigned int tmp = instances_BO_A;
    instances_BO_A = instances_BO_B;
    instances_BO_B = tmp;

    // Reset VAO attributes
    set_attributes();
}

void BoidsData::set_attributes() {
    // Bind VAO
    glBindVertexArray(VAO);
    // Bind instance buffer
    glBindBuffer(GL_ARRAY_BUFFER, instances_BO_A);
    // Set attributes
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, instance_size * sizeof(float), (void*)0);
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, instance_size * sizeof(float), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(4);
    glVertexAttribDivisor(4, 1);
}

}
