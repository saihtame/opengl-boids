#include "mesh_instance.hpp"
#include "render/blend_mode.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/gtc/quaternion.hpp>


namespace ParticleSim::Render {

MeshInstance::MeshInstance() {
    update_model_trans();
}

void MeshInstance::render(const glm::mat4& view, const glm::mat4& projection) const {
    if (mesh == nullptr) {
        return;
    }

    if (material == nullptr)
        return;

    material->use(model_trans, view, projection);

    mesh->render();
}

BlendMode MeshInstance::get_blend_mode() const {
    if (material == nullptr)
        return BLEND_MODE_NONE;

    return material->blend_mode;
}

}
