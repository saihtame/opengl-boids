#pragma once
#include "material.hpp"
#include "shaders/shader_program.hpp"
#include <glm/ext/vector_float4.hpp>
#include <memory>
#include <string>


namespace ParticleSim::Render::Material {

class ColorMaterial : public Material {
public:
    ColorMaterial();
    ~ColorMaterial() = default;

    void use(const glm::mat4& transform, const glm::mat4& view, const glm::mat4& projection) const;

public: // Properties
    glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

private:
    std::unique_ptr<Shaders::ShaderProgram> shader_program = nullptr;

private: // Constants
    const std::string frag_path = "shaders/render/color.frag";
    const std::string vertex_path = "shaders/render/color.vert";
};

}
