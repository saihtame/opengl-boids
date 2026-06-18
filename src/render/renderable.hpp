#pragma once
#include "core/object_3d.hpp"
#include "blend_mode.hpp"


namespace ParticleSim::Render {

class Renderable : public Core::Object3D{
public:
    virtual ~Renderable() = default;

    virtual void render(const glm::mat4& view, const glm::mat4& projection) const = 0;
    virtual BlendMode get_blend_mode() const = 0;
};

}
