#include "primitives.hpp"
#include <glm/detail/qualifier.hpp>
#include <glm/exponential.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_float3x3.hpp>
#include <glm/ext/matrix_float4x3.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <vector>


namespace ParticleSim::Render::Mesh {

std::shared_ptr<Mesh> generate_cube(float width, float length, float height) {
    auto mesh = std::make_shared<Mesh>();

    float hw = width  * 0.5f;
    float hl = length * 0.5f;
    float hh = height * 0.5f;

    float vertices[] = {
        // position        // UVs
        // Front (+Z)
        -hw, -hh,  hl,     0.0f, 0.0f,
         hw, -hh,  hl,     1.0f, 0.0f,
         hw,  hh,  hl,     1.0f, 1.0f,
        -hw,  hh,  hl,     0.0f, 1.0f,
        // Back (-Z)
         hw, -hh, -hl,     0.0f, 0.0f,
        -hw, -hh, -hl,     1.0f, 0.0f,
        -hw,  hh, -hl,     1.0f, 1.0f,
         hw,  hh, -hl,     0.0f, 1.0f,
        // Left (-X)
        -hw, -hh, -hl,     0.0f, 0.0f,
        -hw, -hh,  hl,     1.0f, 0.0f,
        -hw,  hh,  hl,     1.0f, 1.0f,
        -hw,  hh, -hl,     0.0f, 1.0f,
        // Right (+X)
         hw, -hh,  hl,     0.0f, 0.0f,
         hw, -hh, -hl,     1.0f, 0.0f,
         hw,  hh, -hl,     1.0f, 1.0f,
         hw,  hh,  hl,     0.0f, 1.0f,
        // Top (+Y)
        -hw,  hh,  hl,     0.0f, 0.0f,
         hw,  hh,  hl,     1.0f, 0.0f,
         hw,  hh, -hl,     1.0f, 1.0f,
        -hw,  hh, -hl,     0.0f, 1.0f,
        // Bottom (-Y)
        -hw, -hh, -hl,     0.0f, 0.0f,
         hw, -hh, -hl,     1.0f, 0.0f,
         hw, -hh,  hl,     1.0f, 1.0f,
        -hw, -hh,  hl,     0.0f, 1.0f
    };

    unsigned int indices[] = {
         0,  1,  2,   2,  3,  0,   // Front
         4,  5,  6,   6,  7,  4,   // Back
         8,  9, 10,  10, 11,  8,   // Left
        12, 13, 14,  14, 15, 12,   // Right
        16, 17, 18,  18, 19, 16,   // Top
        20, 21, 22,  22, 23, 20    // Bottom
    };

    auto data = generate_normals(vertices, sizeof(vertices) / sizeof(float), indices, sizeof(indices) / sizeof(indices[0]));
    mesh->set_data(data.data(), data.size() * sizeof(float), indices, sizeof(indices) / sizeof(indices[0]));

    return mesh;
}

std::shared_ptr<Mesh> generate_square(float width, float height) {
    auto mesh = std::make_shared<Mesh>();
    float wh = width / 2;
    float hh = height / 2;

    float vertices[] = {
        // Vertices      // UVs       // Normals
        -wh, hh, 0.0f,   0.0f, 1.0f,  0.0f, 0.0f, 1.0f,
        wh, hh, 0.0f,    1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
        wh, -hh, 0.0f,   1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
        -wh, -hh, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f
    };
    unsigned int indices[] = {
        2, 1, 0,
        3, 2, 0 
    };

    mesh->set_data(vertices, sizeof(vertices) / sizeof(float), indices, sizeof(indices) / sizeof(unsigned int));
    return mesh;
}

std::shared_ptr<Mesh> generate_square_2sides(float width, float height) {
    auto mesh = std::make_shared<Mesh>();
    float wh = width / 2;
    float hh = height / 2;

    float vertices[] = {
        // Vertices      // UVs       // Normals
        // Back face
        -wh, hh, 0.0f,   0.0f, 1.0f,  0.0f, 0.0f, -1.0f,
        wh, hh, 0.0f,    1.0f, 1.0f,  0.0f, 0.0f, -1.0f,
        wh, -hh, 0.0f,   1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
        -wh, -hh, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
        // Front face
        -wh, hh, 0.0f,   0.0f, 1.0f,  0.0f, 0.0f, 1.0f,
        wh, hh, 0.0f,    1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
        wh, -hh, 0.0f,   1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
        -wh, -hh, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f
    };
    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3,
        6, 5, 4,
        7, 6, 4
    };

    mesh->set_data(vertices, sizeof(vertices), indices, sizeof(indices) / sizeof(unsigned int));
    return mesh;
}

static glm::vec2 get_ico_uvs(glm::vec3 pos) {
    glm::vec3 p = glm::normalize(pos);

    const float PI = glm::pi<float>();

    float u = 0.5f + glm::atan(p.z, p.x) / (2.0f * PI);
    float v = 0.5f - glm::asin(glm::clamp(p.y, -1.0f, 1.0f)) / PI;

    return glm::vec2(u, v);
}

std::shared_ptr<Mesh> generate_ico_sphere(float radius, unsigned int subdivisions) {
    const float phi = (1.0f + glm::sqrt(5.0f)) * 0.5f;

    // Generate icosahedron
    std::vector<glm::vec3> positions = {
        {-1,  phi,  0}, // 0
        { 1,  phi,  0}, // 1
        {-1, -phi,  0}, // 2
        { 1, -phi,  0}, // 3
        { 0, -1,  phi}, // 4
        { 0,  1,  phi}, // 5
        { 0, -1, -phi}, // 6
        { 0,  1, -phi}, // 7
        { phi,  0, -1}, // 8
        { phi,  0,  1}, // 9
        {-phi,  0, -1}, // 10
        {-phi,  0,  1}  // 11
    };

    for (auto& p : positions) {
        p = glm::normalize(p) * radius;
    }

    std::vector<unsigned int> indices = {
        0, 11, 5,
        0, 5, 1,
        0, 1, 7,
        0, 7, 10,
        0, 10, 11,
        1, 5, 9,
        5, 11, 4,
        11, 10, 2,
        10, 7, 6,
        7, 1, 8,
        3, 9, 4,
        3, 4, 2,
        3, 2, 6,
        3, 6, 8,
        3, 8, 9,
        4, 9, 5,
        2, 4, 11,
        6, 2, 10,
        8, 6, 7,
        9, 8, 1
    };

    // Subdivide to ico sphere
    for (unsigned int s = 0; s < subdivisions; ++s) {
        std::unordered_map<uint64_t, unsigned int> midpoint_cache;
        std::vector<unsigned int> new_indices;
        new_indices.reserve(indices.size() * 4);

        // Subidivide
        auto get_midpoint = [&](unsigned int a, unsigned int b) -> unsigned int {
            unsigned int lo = std::min(a, b);
            unsigned int hi = std::max(a, b);
            uint64_t key = (static_cast<uint64_t>(lo) << 32) | hi;

            auto it = midpoint_cache.find(key);
            if (it != midpoint_cache.end()) {
                return it->second;
            }

            glm::vec3 mid = glm::normalize((positions[a] + positions[b]) * 0.5f) * radius;

            unsigned int index = static_cast<unsigned int>(positions.size());
            positions.push_back(mid);
            midpoint_cache[key] = index;
            return index;
        };

        for (size_t i = 0; i < indices.size(); i += 3) {
            unsigned int i0 = indices[i + 0];
            unsigned int i1 = indices[i + 1];
            unsigned int i2 = indices[i + 2];

            unsigned int a = get_midpoint(i0, i1);
            unsigned int b = get_midpoint(i1, i2);
            unsigned int c = get_midpoint(i2, i0);

            new_indices.push_back(i0); new_indices.push_back(a);  new_indices.push_back(c);
            new_indices.push_back(i1); new_indices.push_back(b);  new_indices.push_back(a);
            new_indices.push_back(i2); new_indices.push_back(c);  new_indices.push_back(b);
            new_indices.push_back(a);  new_indices.push_back(b);  new_indices.push_back(c);
        }

        indices = std::move(new_indices);
    }

    struct VertexKey {
        unsigned int pos_index;
        unsigned char seam_variant; // 0 = normal u, 1 = wrapped u (+1)

        bool operator==(const VertexKey& other) const {
            return pos_index == other.pos_index && seam_variant == other.seam_variant;
        }
    };

    struct VertexKeyHash {
        size_t operator()(const VertexKey& k) const {
            return (static_cast<size_t>(k.pos_index) * 1315423911u) ^ static_cast<size_t>(k.seam_variant);
        }
    };

    std::unordered_map<VertexKey, unsigned int, VertexKeyHash> vertex_map;
    std::vector<float> vertices;
    std::vector<unsigned int> final_indices;

    vertices.reserve(positions.size() * 5);
    final_indices.reserve(indices.size());

    auto append_vertex = [&](unsigned int pos_index, unsigned char seam_variant, const glm::vec2& uv) -> unsigned int {
        VertexKey key{pos_index, seam_variant};

        auto it = vertex_map.find(key);
        if (it != vertex_map.end()) {
            return it->second;
        }

        const glm::vec3& p = positions[pos_index];
        unsigned int out_index = static_cast<unsigned int>(vertices.size() / 5);

        vertices.push_back(p.x);
        vertices.push_back(p.y);
        vertices.push_back(p.z);
        vertices.push_back(uv.x);
        vertices.push_back(uv.y);

        vertex_map.emplace(key, out_index);
        return out_index;
    };

    for (size_t i = 0; i < indices.size(); i += 3) {
        unsigned int tri[3] = {
            indices[i + 0],
            indices[i + 1],
            indices[i + 2]
        };

        glm::vec2 uv[3] = {
            get_ico_uvs(positions[tri[0]]),
            get_ico_uvs(positions[tri[1]]),
            get_ico_uvs(positions[tri[2]])
        };

        unsigned char seam_variant[3] = {0, 0, 0};

        float min_u = std::min({uv[0].x, uv[1].x, uv[2].x});
        float max_u = std::max({uv[0].x, uv[1].x, uv[2].x});

        if ((max_u - min_u) > 0.5f) {
            for (int k = 0; k < 3; ++k) {
                if (uv[k].x < 0.5f) {
                    uv[k].x += 1.0f;
                    seam_variant[k] = 1;
                }
            }
        }

        final_indices.push_back(append_vertex(tri[0], seam_variant[0], uv[0]));
        final_indices.push_back(append_vertex(tri[1], seam_variant[1], uv[1]));
        final_indices.push_back(append_vertex(tri[2], seam_variant[2], uv[2]));
    }

    auto mesh = std::make_shared<Mesh>();
    auto data = generate_normals(vertices.data(), vertices.size(), final_indices.data(), static_cast<unsigned int>(final_indices.size()));
    mesh->set_data(
        data.data(),
        data.size() * sizeof(float),
        final_indices.data(),
        static_cast<int>(final_indices.size())
    );

    return mesh;
}

std::vector<float> generate_normals(float* data, unsigned int data_size, unsigned int* indices, unsigned int indices_size) {
    // Calculate face normal
    int faces = indices_size / 3;
    int vertices = data_size / 5;
    std::vector<glm::vec3> vertex_normals;
    vertex_normals.resize(vertices, glm::vec3(0.0f, 0.0f, 0.0f));
    std::vector<int> vertex_face_count;
    vertex_face_count.resize(vertices, 0);
    for (int i = 0; i < faces; i++) {
        // Get indices
        int ia = indices[i*3];
        int ib = indices[i*3+1];
        int ic = indices[i*3+2];
        // Get vertices
        glm::vec3 a = glm::vec3(data[ia*5], data[ia*5+1], data[ia*5+2]);
        glm::vec3 b = glm::vec3(data[ib*5], data[ib*5+1], data[ib*5+2]);
        glm::vec3 c = glm::vec3(data[ic*5], data[ic*5+1], data[ic*5+2]);
        // Calculate side vectors
        auto ab = b-a;
        auto ac = c-a;
        // Calculate normal
        glm::vec3 normal = glm::cross(ab, ac);
        // Add normal to vertex normals
        vertex_normals[ia] += normal;
        vertex_normals[ib] += normal;
        vertex_normals[ic] += normal;
        // Increment 1 to face count
        vertex_face_count[ia] += 1;
        vertex_face_count[ib] += 1;
        vertex_face_count[ic] += 1;
    }

    // Calculate vertex normals
    std::vector<float> new_data;
    new_data.resize(vertices * 8);
    for (int i = 0; i < vertices; i++) {
        // Copy old data to new data
        // Vertex data
        new_data[i*8] = data[i*5];
        new_data[i*8+1] = data[i*5+1];
        new_data[i*8+2] = data[i*5+2];
        // UV data
        new_data[i*8+3] = data[i*5+3];
        new_data[i*8+4] = data[i*5+4];
        // Calculate vertex normal
        // Skip vertices not connected to any faces
        if (!vertex_face_count[i]) continue;
        glm::vec3 normal = glm::normalize(vertex_normals[i]);
        new_data[i*8+5] = normal.x;
        new_data[i*8+6] = normal.y;
        new_data[i*8+7] = normal.z;
    }

    return new_data;
}

}
