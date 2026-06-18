#include "mesh_instance.hpp"
#include "render/blend_mode.hpp"
#include "render/mesh/mesh.hpp"
#include "render/mesh/mesh_render_data.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glad/gl.h>
#include <memory>


namespace ParticleSim::Render {

MeshInstance::MeshInstance() {
    update_model_trans();
}

void MeshInstance::set_mesh(std::shared_ptr<Mesh::Mesh>& value) {
    if (value== nullptr) return;

    render_data = std::make_unique<Mesh::MeshRenderData>(value);
}

void MeshInstance::render(const glm::mat4& view, const glm::mat4& projection) const {
    if (render_data == nullptr || material == nullptr) {
        return;
    }

    material->use(model_trans, view, projection);

    // Bind VAO
    glBindVertexArray(render_data->VAO);
    // Render
    glDrawElements(GL_TRIANGLES, render_data->indices_size, GL_UNSIGNED_INT, 0);
}

BlendMode MeshInstance::get_blend_mode() const {
    if (material == nullptr)
        return BLEND_MODE_NONE;

    return material->blend_mode;
}

}
