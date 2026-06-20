#include "boids_material.hpp"
#include "boids/boids_params.hpp"


namespace ParticleSim::Boids {

BoidsMaterial::BoidsMaterial(const std::shared_ptr<BoidsParams>& params) : params(params) {
    Render::Material::Shader vertex_shader(vertex_shader_path, GL_VERTEX_SHADER);
    Render::Material::Shader fragment_shader(fragment_shader_path, GL_FRAGMENT_SHADER);
    shader_program = std::make_unique<Render::Material::ShaderProgram>();
    shader_program->attach_shader(vertex_shader);
    shader_program->attach_shader(fragment_shader);
    shader_program->link();
}

void BoidsMaterial::use(const glm::mat4& transform, const glm::mat4& view, const glm::mat4& projection) const {
    shader_program->use();
    shader_program->set_uniform_matrix("model", transform);
    shader_program->set_uniform_matrix("view", view);
    shader_program->set_uniform_matrix("projection", projection);
    shader_program->set_uniform_float("boidMaxSpeed", params->boid_max_speed);
}

}
