#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include "../blend_mode.hpp"


namespace ParticleSim::Render::Material {

class Material {
public:
    ~Material() = default;

    virtual void use(const glm::mat4& transform, const glm::mat4& view, const glm::mat4& projection) const = 0;

    BlendMode blend_mode = BLEND_MODE_OPAQUE;
};

}
