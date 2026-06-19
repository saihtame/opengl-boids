#include <SDL3/SDL_keyboard.h>
#include "camera.hpp"
#include "core/object_3d.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>


namespace ParticleSim::Core {

Camera::Camera() {
    update_view();
}

void Camera::update(float delta) {
    const bool *state = SDL_GetKeyboardState(NULL);
    glm::vec3 camera_movement = glm::vec3(0.0f, 0.0f, 0.0f);
    if (state[SDL_SCANCODE_W]) {
        camera_movement += forward();
    }
    if (state[SDL_SCANCODE_S]) {
        camera_movement -= forward();
    }
    if (state[SDL_SCANCODE_A]) {
        camera_movement -= right();
    }
    if (state[SDL_SCANCODE_D]) {
        camera_movement += right();
    }
    if (state[SDL_SCANCODE_SPACE]) {
        camera_movement += up();
    }
    if (state[SDL_SCANCODE_LSHIFT]) {
        camera_movement -= up();
    }

    move(camera_movement * delta * speed);
}

void Camera::move_to(const glm::vec3& new_pos) {
    Object3D::move_to(new_pos);
    update_view();
}

void Camera::rotate(float angle, const glm::vec3& axis) {
    Object3D::rotate(angle, axis);
    update_view();
}

void Camera::rotate_yaw_pitch(float yaw, float pitch) {
    float yaw_delta   = glm::radians(yaw * sensitivity);
    float pitch_delta = glm::radians(pitch * sensitivity);

    glm::vec3 f = glm::normalize(forward());

    float current_pitch = std::asin(glm::clamp(f.y, -1.0f, 1.0f));
    float max_pitch = glm::radians(89.0f);

    float target_pitch = glm::clamp(
        current_pitch + pitch_delta,
        -max_pitch,
        max_pitch
    );

    float clamped_pitch_delta = target_pitch - current_pitch;

    Object3D::rotate(yaw_delta, glm::vec3(0.0f, 1.0f, 0.0f));
    Object3D::rotate(clamped_pitch_delta, right());
}

void Camera::zoom(float amount) {
    fov = glm::clamp(fov + amount, min_fov, max_fov);
}

glm::mat4 Camera::get_projection(float view_width, float view_height) const {
    return glm::perspective(glm::radians(fov), (float)(view_width/view_height), 0.1f, 300.0f);
}

inline void Camera::update_view() {
    view = glm::lookAt(world_pos, world_pos + forward(), glm::vec3(0.0f, 1.0f, 0.0f));
}

}
