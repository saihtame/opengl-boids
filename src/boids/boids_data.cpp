#include "boids_data.hpp"
#include "render/material/shader.hpp"
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/random.hpp>
#include <memory>


namespace ParticleSim::Boids {

BoidsData::BoidsData(const std::shared_ptr<Render::Mesh::Mesh>& mesh, const std::shared_ptr<BoidsParams>& parameters)
    : Render::Mesh::MeshRenderData(mesh), params(parameters) {
    // Prepare instances data
    std::vector<float> data;
    data.resize(params->boids * 8);
    for (int i = 0; i < params->boids; i++) {
        // Position data
        data[i * 8]     = glm::linearRand(1.0f, params->bounds.x - 1.0f);
        data[i * 8 + 1] = glm::linearRand(1.0f, params->bounds.y - 1.0f);
        data[i * 8 + 2] = glm::linearRand(1.0f, params->bounds.z - 1.0f);
        data[i * 8 + 3] = 0.0f; // Padding
        // Velocity data
        glm::vec3 vel = glm::normalize(glm::vec3(
            glm::linearRand(-1.0, 1.0),
            glm::linearRand(-1.0, 1.0),
            glm::linearRand(-1.0, 1.0)
        )) * glm::linearRand(params->boid_min_speed, params->boid_max_speed);
        data[i * 8 + 4] = vel.x;
        data[i * 8 + 5] = vel.y;
        data[i * 8 + 6] = vel.z;
        data[i * 8 + 7] = 0.0f; // Padding
    }

    // Create instances buffers
    glGenBuffers(1, &instances_BO_A);
    glGenBuffers(1, &instances_BO_B);
    // Copy data over to buffer A
    glBindBuffer(GL_ARRAY_BUFFER, instances_BO_A);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_DYNAMIC_COPY);
    // Reser space in buffer B
    glBindBuffer(GL_ARRAY_BUFFER, instances_BO_B);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), NULL, GL_DYNAMIC_COPY);

    // Set vao attributes
    set_attributes();
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
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(4);
    glVertexAttribDivisor(4, 1);
}

}
