#pragma once
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>


namespace ParticleSim::Render::Mesh {

class Mesh {
public:
    void generate_normals();

public: // Public properties
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
};

}
