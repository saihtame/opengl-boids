#pragma once
#include "core/object_3d.hpp"
#include <glm/detail/qualifier.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>


namespace ParticleSim::Core {

class Camera : public Object3D {
public:
    Camera();

    virtual void update(float delta);
    virtual void move_to(const glm::vec3& new_pos);
    virtual void rotate(float angle, const glm::vec3& axis);
    void rotate_yaw_pitch(float yaw, float pitch);
    void zoom(float amount);
    glm::mat4 get_projection(float view_width, float view_height) const;

public:
    float fov = 45.0;
    glm::mat4 view;

private:
    const float speed = 16.0;
    const float sensitivity = 0.2;
    const float max_fov = 45.0f;
    const float min_fov = 1.0f;

private:
    bool first_rotation = true;

private:
    inline void update_view();
};

}
