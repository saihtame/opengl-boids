#include "mesh.hpp"
#include <glad/gl.h>
#include <glm/geometric.hpp>


namespace ParticleSim::Render::Mesh {

void Mesh::generate_normals() {
    // Calculate face normal
    int faces = indices.size() / 3;
    int vertices_c = vertices.size();
    normals.clear();
    normals.resize(vertices_c, glm::vec3(0.0f, 0.0f, 0.0f));
    for (int i = 0; i < faces; i++) {
        // Get indices
        int ia = indices[i*3];
        int ib = indices[i*3+1];
        int ic = indices[i*3+2];
        // Get vertices
        glm::vec3 a = vertices[ia];
        glm::vec3 b = vertices[ib];
        glm::vec3 c = vertices[ic];
        // Calculate side vectors
        auto ab = b-a;
        auto ac = c-a;
        // Calculate normal
        glm::vec3 normal = glm::cross(ab, ac);
        // Add normal to vertex normals
        normals[ia] += normal;
        normals[ib] += normal;
        normals[ic] += normal;
    }

    // Calculate vertex normals
    for (int i = 0; i < vertices_c; i++) {
        if (glm::dot(normals[i], normals[i]) == 0.0f) continue;
        normals[i] = glm::normalize(normals[i]);
    }
}

}
