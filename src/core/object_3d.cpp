#include "object_3d.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>


namespace ParticleSim::Core {

void Object3D::move_to(const glm::vec3& new_pos) {
    world_pos = new_pos;
    update_model_trans();
}

void Object3D::move(const glm::vec3& change) {
    move_to(world_pos + change);
}

void Object3D::rotate(float rotation, const glm::vec3& axis) {
    glm::quat delta = glm::angleAxis(rotation, glm::normalize(axis));
    orientation = glm::normalize(delta * orientation);
    update_model_trans();
}

}
