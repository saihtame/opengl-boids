#include "boids_data.hpp"
#include "render/material/shader.hpp"
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/random.hpp>


namespace ParticleSim::Boids {

BoidsData::BoidsData(const std::shared_ptr<Render::Mesh::Mesh>& mesh, int boids, glm::vec3 bounds)
    : Render::Mesh::MeshRenderData(mesh) {
    // Prepare instances data
    std::vector<float> data;
    data.resize(boids * 8);
    for (int i = 0; i < boids; i++) {
        // Position data
        data[i * 8]     = glm::linearRand(1.0f, bounds.x - 1.0f);
        data[i * 8 + 1] = glm::linearRand(1.0f, bounds.y - 1.0f);
        data[i * 8 + 2] = glm::linearRand(1.0f, bounds.z - 1.0f);
        data[i * 8 + 3] = 0.0f; // Padding
        // Velocity data
        glm::vec3 vel = glm::normalize(glm::vec3(
            glm::linearRand(-1.0, 1.0),
            glm::linearRand(-1.0, 1.0),
            glm::linearRand(-1.0, 1.0)
        ));
        data[i * 8 + 4] = vel.x;
        data[i * 8 + 5] = vel.y;
        data[i * 8 + 6] = vel.z;
        data[i * 8 + 7] = 0.0f; // Padding
    }
    // Bind VAO
    glBindVertexArray(VAO);
    // Create instances buffer
    glGenBuffers(1, &instances_BO);
    glBindBuffer(GL_ARRAY_BUFFER, instances_BO);
    // Copy data over to buffer
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_DYNAMIC_COPY);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);  
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(4);
    glVertexAttribDivisor(4, 1);  
}

BoidsData::~BoidsData() {
    glDeleteBuffers(1, &instances_BO);
    instances_BO = 0;
}

}
