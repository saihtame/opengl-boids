#include "boids_sim.hpp"
#include "render/material/shader.hpp"
#include <glm/ext/vector_float3.hpp>
#include <memory>


namespace ParticleSim::Boids {

BoidsSim::BoidsSim(const std::shared_ptr<Render::Mesh::Mesh>& value) {
    update_model_trans();
    material = std::make_unique<BoidsMaterial>();
    if (value != nullptr)
        data = std::make_unique<BoidsData>(value, boids, bounds);
    compute = std::make_unique<BoidsCompute>(boids, bounds);
}

void BoidsSim::update(float delta) {
    if (compute == nullptr || data == nullptr)
        return;

    compute->compute(boid_speed * delta, *data);
}

void BoidsSim::render(const glm::mat4& view, const glm::mat4& projection) const {
    if (data == nullptr || material == nullptr) {
        return;
    }

    // Select material
    material->use(model_trans, view, projection);

    // Render boids
    // Bind VAO
    glBindVertexArray(data->VAO);
    // Render
    glDrawElementsInstanced(GL_TRIANGLES, data->indices_size, GL_UNSIGNED_INT, 0, boids);
}

Render::BlendMode BoidsSim::get_blend_mode() const {
    if (material == nullptr)
        return Render::BLEND_MODE_NONE;

    return material->blend_mode;
}

}
