#include "color_material.hpp"


namespace ParticleSim::Render::Material {

ColorMaterial::ColorMaterial() {
    Shaders::Shader vertex_shader(vertex_path, GL_VERTEX_SHADER);
    Shaders::Shader fragment_shader(frag_path, GL_FRAGMENT_SHADER);
    shader_program = std::make_unique<Shaders::ShaderProgram>();
    shader_program->attach_shader(vertex_shader);
    shader_program->attach_shader(fragment_shader);
    shader_program->link();
}

void ColorMaterial::use(const glm::mat4& transform, const glm::mat4& view, const glm::mat4& projection) const {
    shader_program->use();
    shader_program->set_uniform_matrix("model", transform);
    shader_program->set_uniform_matrix("view", view);
    shader_program->set_uniform_matrix("projection", projection);
    shader_program->set_uniform_vec4("color", color);
}

}
