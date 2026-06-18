#include "primitives.hpp"
#include <cmath>
#include <glm/detail/qualifier.hpp>
#include <glm/exponential.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_float3x3.hpp>
#include <glm/ext/matrix_float4x3.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>


namespace ParticleSim::Render::Mesh {

std::shared_ptr<Mesh> generate_square_2sides(float width, float height) {
    auto mesh = std::make_shared<Mesh>();

    float wh = width * 0.5f;
    float hh = height * 0.5f;

    mesh->vertices = {
        // Back face
        {-wh,  hh, 0.0f},
        { wh,  hh, 0.0f},
        { wh, -hh, 0.0f},
        {-wh, -hh, 0.0f},
        // Front face
        {-wh,  hh, 0.0f},
        { wh,  hh, 0.0f},
        { wh, -hh, 0.0f},
        {-wh, -hh, 0.0f}
    };

    mesh->uvs = {
        // Back face
        {0.0f, 1.0f},
        {1.0f, 1.0f},
        {1.0f, 0.0f},
        {0.0f, 0.0f},
        // Front face
        {0.0f, 1.0f},
        {1.0f, 1.0f},
        {1.0f, 0.0f},
        {0.0f, 0.0f}
    };

    mesh->normals = {
        // Back face
        {0.0f, 0.0f, -1.0f},
        {0.0f, 0.0f, -1.0f},
        {0.0f, 0.0f, -1.0f},
        {0.0f, 0.0f, -1.0f},
        // Front face
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f}
    };

    mesh->indices = {
        0, 1, 2,
        0, 2, 3,
        6, 5, 4,
        7, 6, 4
    };

    return mesh;
}

std::shared_ptr<Mesh> generate_pyramid(float width, float length) {
    auto mesh = std::make_shared<Mesh>();
    float wh = width * 0.5f;

    mesh->vertices = {
        // Square face
        {-wh,  wh, 0.0f},
        { wh,  wh, 0.0f},
        { wh, -wh, 0.0f},
        {-wh, -wh, 0.0f},
        // Pyramid top
        {0.0f, 0.0f, length}
    };

    mesh->uvs = {
        // Square face
        {0.0f, 1.0f},
        {1.0f, 1.0f},
        {1.0f, 0.0f},
        {0.0f, 0.0f},
        // Pyramid top
        {0.0f, 0.0f}
    };

    mesh->normals = {
        // Square face
        {0.0f, 0.0f, -1.0f},
        {0.0f, 0.0f, -1.0f},
        {0.0f, 0.0f, -1.0f},
        {0.0f, 0.0f, -1.0f},
        // Pyramid top
        {0.0f, 0.0f, 1.0f}
    };

    mesh->indices = {
        // Floor
        2, 1, 0,
        3, 2, 0,
        // Walls
        4, 0, 1,
        4, 1, 2,
        4, 2, 3,
        4, 3, 0
    };

    return mesh;
}


std::shared_ptr<Mesh> generate_cone(float radius, float length, int subdivisions) {
    int circle_vertex_count = 4 * (subdivisions + 1);

    auto mesh = std::make_shared<Mesh>();

    mesh->vertices.reserve(circle_vertex_count + 2);
    mesh->uvs.reserve(circle_vertex_count + 2);
    mesh->normals.reserve(circle_vertex_count + 2);
    mesh->indices.reserve(circle_vertex_count * 6);

    // Bottom center
    mesh->vertices.emplace_back(0.0f, 0.0f, 0.0f);
    mesh->uvs.emplace_back(0.0f, 0.0f);
    mesh->normals.emplace_back(0.0f, 0.0f, -1.0f);

    double vertex_angle = (2.0 * glm::pi<double>()) / circle_vertex_count;

    // Bottom ring
    for (int i = 0; i < circle_vertex_count; ++i) {
        float x = std::sin(vertex_angle * i);
        float y = std::cos(vertex_angle * i);

        mesh->vertices.emplace_back(x * radius, y * radius, 0.0f);
        mesh->uvs.emplace_back(x * 0.5f + 0.5f, y * 0.5f + 0.5f);
        mesh->normals.emplace_back(0.0f, 0.0f, -1.0f);

        unsigned int curr = static_cast<unsigned int>(i + 1);
        unsigned int next = static_cast<unsigned int>(((i + 1) % circle_vertex_count) + 1);

        // Bottom face
        mesh->indices.push_back(0);
        mesh->indices.push_back(next);
        mesh->indices.push_back(curr);

        // Side face
        mesh->indices.push_back(static_cast<unsigned int>(circle_vertex_count + 1));
        mesh->indices.push_back(curr);
        mesh->indices.push_back(next);
    }

    // Tip
    mesh->vertices.emplace_back(0.0f, 0.0f, length);
    mesh->uvs.emplace_back(0.0f, 0.0f);
    mesh->normals.emplace_back(0.0f, 0.0f, 1.0f);

    return mesh;
}


}
