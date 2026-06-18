#include "mesh.hpp"
#include <glad/gl.h>


namespace ParticleSim::Render::Mesh {


Mesh::Mesh() {
}

Mesh::~Mesh() {
    delete_buffers();
}

void Mesh::set_data(void* data, unsigned int data_size, unsigned int* indices, int indices_size) {
    this->indices_size = indices_size;

    // Delete existing buffers, if any exist
    delete_buffers();

    // Create VAO
    glGenVertexArrays(1, &VAO);
    // Bind VAO
    glBindVertexArray(VAO);
    // Generate VBO
    glGenBuffers(1, &VBO);
    // Bind VBO buffer to the VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Copy data to buffer
    glBufferData(GL_ARRAY_BUFFER, data_size, data, GL_STATIC_DRAW);

    // Create EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // Copy indices data to EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices_size, indices, GL_STATIC_DRAW);

    // Create and enable vertex attribues
    // Vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // UVs
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Normals
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void Mesh::render() {
    // Bind VAO
    glBindVertexArray(VAO);
    // Draw
    glDrawElements(GL_TRIANGLES, indices_size, GL_UNSIGNED_INT, 0);
}

inline void Mesh::delete_buffers() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
    VBO = 0;
    EBO = 0;
    VAO = 0;
}

}
