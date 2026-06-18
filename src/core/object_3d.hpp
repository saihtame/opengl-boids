#pragma once
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/quaternion.hpp>


namespace ParticleSim::Core {

class Object3D {
public:
    virtual void update(float) {};
    virtual void move_to(const glm::vec3& new_pos);
    void move(const glm::vec3& change);
    virtual void rotate(float angle, const glm::vec3& axis);
    glm::vec3 forward() const { return orientation * glm::vec3(0.0f, 0.0f, -1.0f); }
    glm::vec3 right()   const { return orientation * glm::vec3(1.0f, 0.0f,  0.0f); }
    glm::vec3 up()      const { return orientation * glm::vec3(0.0f, 1.0f,  0.0f); }

public:
    glm::vec3 world_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::quat orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

protected:
    glm::mat4 model_trans;

protected:
    inline void update_model_trans() {
        model_trans = glm::translate(glm::mat4(1.0f), world_pos) // Position
                    * glm::mat4_cast(orientation);             // Rotation
        model_trans = glm::scale(model_trans, scale);            // Scale
    }
};

}
