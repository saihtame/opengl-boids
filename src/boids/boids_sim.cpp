#include "boids_sim.hpp"
#include <memory>


namespace ParticleSim::Boids {

BoidsSim::BoidsSim() {
    update_model_trans();
    material = std::make_unique<BoidsMaterial>();
}

void BoidsSim::set_mesh(std::shared_ptr<Render::Mesh::Mesh>& value) {
    render_data = std::make_unique<BoidsMeshRenderData>(value);
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
    glDrawElements(GL_TRIANGLES, render_data->indices_size, GL_UNSIGNED_INT, 0);
}

Render::BlendMode BoidsSim::get_blend_mode() const {
    if (material == nullptr)
        return Render::BLEND_MODE_NONE;

    return material->blend_mode;
}


}
