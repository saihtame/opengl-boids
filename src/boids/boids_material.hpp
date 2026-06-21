#pragma once
#include "boids/boids_params.hpp"
#include "render/material/material.hpp"
#include "shaders/shader_program.hpp"
#include <memory>
#include <string>


namespace ParticleSim::Boids {

class BoidsMaterial : public Render::Material::Material {
public:
    BoidsMaterial(const std::shared_ptr<BoidsParams>& parameters);
    ~BoidsMaterial() = default;

    virtual void use(const glm::mat4& transform, const glm::mat4& view, const glm::mat4& projection) const;

private:
    std::unique_ptr<Shaders::ShaderProgram> shader_program;

private:
    const std::string fragment_shader_path = "shaders/render/boids.frag";
    const std::string vertex_shader_path = "shaders/render/boids.vert";
    std::shared_ptr<BoidsParams> params;
};
}
