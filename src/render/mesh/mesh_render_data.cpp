#include "mesh_render_data.hpp"
#include <glad/gl.h>


namespace ParticleSim::Render::Mesh {

MeshRenderData::MeshRenderData(const std::shared_ptr<Mesh>& mesh) {
    // mesh->vertices == std::vector<glm::vec3>
    // mesh->normals == std::vector<glm::vec3>
    // mesh->uvs == std::vector<glm::vec2>
    // mesh->indices == std::vector<glm::vec2>
    assert(mesh->vertices.size() == mesh->uvs.size());
    assert(mesh->vertices.size() == mesh->normals.size());

    indices_size = mesh->indices.size();
    // Create VAO
    glGenVertexArrays(1, &VAO);
    // Bind VAO
    glBindVertexArray(VAO);
    // Generate VBO
    glGenBuffers(1, &VBO);
    // Bind VBO buffer to the VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Copy data to buffer
    // Interleave data
    std::vector<float> vertex_data;
    vertex_data.resize(mesh->vertices.size() * 8);
    for (unsigned long i = 0; i < mesh->vertices.size(); i++) {
        vertex_data[i * 8] = mesh->vertices[i].x;
        vertex_data[i * 8 + 1] = mesh->vertices[i].y;
        vertex_data[i * 8 + 2] = mesh->vertices[i].z;
        vertex_data[i * 8 + 3] = mesh->uvs[i].x;
        vertex_data[i * 8 + 4] = mesh->uvs[i].y;
        vertex_data[i * 8 + 5] = mesh->normals[i].x;
        vertex_data[i * 8 + 6] = mesh->normals[i].y;
        vertex_data[i * 8 + 7] = mesh->normals[i].z;
    }

    // Copy vertex data over to buffer
    glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(float), vertex_data.data(), GL_STATIC_DRAW);

    // Create EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // Copy indices data to EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->indices.size(), mesh->indices.data(), GL_STATIC_DRAW);

    // Create and enable vertex attribues
    unsigned int stride = 8 * sizeof(float);
    // Vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    // UVs
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Normals
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

MeshRenderData::~MeshRenderData() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

}
