#pragma once
#include "renderable.hpp"
#include "mesh/mesh.hpp"
#include "material/material.hpp"
#include <memory>


namespace ParticleSim::Render {

class MeshRenderer : public Renderable {
public:
    MeshRenderer();
    ~MeshRenderer() = default;

    virtual void render(const glm::mat4& view, const glm::mat4& projection) const;
    virtual BlendMode get_blend_mode() const;

public: // Public properties
    std::shared_ptr<Mesh::Mesh> mesh = nullptr;
    std::shared_ptr<Material::Material> material = nullptr;
};

}
