#include "boids_sim.hpp"
#include "render/material/shader.hpp"
#include <glm/ext/vector_float3.hpp>
#include <memory>


namespace ParticleSim::Boids {

BoidsSim::BoidsSim(const std::shared_ptr<Render::Mesh::Mesh>& value, int boids, glm::vec3 bounds)
    : boids(boids), bounds(bounds) {
    update_model_trans();
    material = std::make_unique<BoidsMaterial>();
    if (value != nullptr)
        render_data = std::make_unique<BoidsMeshRenderData>(value, boids, bounds);
}

void BoidsSim::update(float) {

}

void BoidsSim::render(const glm::mat4& view, const glm::mat4& projection) const {
    if (render_data == nullptr || material == nullptr) {
        return;
    }

    material->use(model_trans, view, projection);

    // Bind VAO
    glBindVertexArray(render_data->VAO);
    // Render
    glDrawElementsInstanced(GL_TRIANGLES, render_data->indices_size, GL_UNSIGNED_INT, 0, boids);
}

Render::BlendMode BoidsSim::get_blend_mode() const {
    if (material == nullptr)
        return Render::BLEND_MODE_NONE;

    return material->blend_mode;
}


}
