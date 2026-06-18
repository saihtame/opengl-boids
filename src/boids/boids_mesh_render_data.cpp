#include "boids_mesh_render_data.hpp"
#include "render/material/shader.hpp"
#include "render/mesh/mesh_render_data.hpp"
#include <glm/ext/vector_float3.hpp>


namespace ParticleSim::Boids {

BoidsMeshRenderData::BoidsMeshRenderData(const std::shared_ptr<Render::Mesh::Mesh>& mesh, int boids, glm::vec3 pos_offset)
    : Render::Mesh::MeshRenderData(mesh) {
    // Prepare instances data
    std::vector<float> data;
    data.resize(boids * 6, 0.0);
    glm::vec3 instance_pos = -pos_offset * ((float)boids / 2) + pos_offset * 0.5f;
    for (int i = 0; i < boids; i++) {
        // Position data
        data[i * 6] = instance_pos.x;
        data[i * 6 + 1] = instance_pos.y;
        data[i * 6 + 2] = instance_pos.z;
        // Velocity data remains 0
        instance_pos += pos_offset;
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
