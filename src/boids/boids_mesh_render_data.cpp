#include "boids_mesh_render_data.hpp"
#include "render/material/shader.hpp"
#include "render/mesh/mesh_render_data.hpp"
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/random.hpp>


namespace ParticleSim::Boids {

BoidsMeshRenderData::BoidsMeshRenderData(const std::shared_ptr<Render::Mesh::Mesh>& mesh, int boids, glm::vec3 bounds)
    : Render::Mesh::MeshRenderData(mesh) {
    // Prepare instances data
    std::vector<float> data;
    data.resize(boids * 6, 0.0);
    for (int i = 0; i < boids; i++) {
        // Position data
        data[i * 6]     = glm::linearRand(1.0f, bounds.x - 1.0f);
        data[i * 6 + 1] = glm::linearRand(1.0f, bounds.y - 1.0f);
        data[i * 6 + 2] = glm::linearRand(1.0f, bounds.z - 1.0f);
        // Velocity data
        glm::vec3 vel = glm::sphericalRand(1.0f);
        data[i * 6 + 3] = vel.x;
        data[i * 6 + 4] = vel.y;
        data[i * 6 + 5] = vel.z;
    }
    // Bind VAO
    glBindVertexArray(VAO);
    // Create instances buffer
    glGenBuffers(1, &instances_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, instances_VBO);
    // Copy data over to buffer
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);  
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(4);
    glVertexAttribDivisor(4, 1);  
}

BoidsMeshRenderData::~BoidsMeshRenderData() {
    glDeleteBuffers(1, &instances_VBO);
    instances_VBO = 0;
}

}
