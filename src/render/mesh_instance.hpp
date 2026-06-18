#pragma once
#include "mesh/mesh.hpp"
#include "render/mesh/mesh_render_data.hpp"
#include "renderable.hpp"
#include "material/material.hpp"
#include <memory>


namespace ParticleSim::Render {

class MeshInstance : public Renderable {
public:
    MeshInstance(const std::shared_ptr<Mesh::Mesh>& mesh);
    ~MeshInstance() = default;

    virtual void render(const glm::mat4& view, const glm::mat4& projection) const;
    virtual BlendMode get_blend_mode() const;

public: // Public properties
    std::shared_ptr<Material::Material> material = nullptr;

private:
    std::unique_ptr<Mesh::MeshRenderData> render_data;

private:
    inline void delete_buffers();
};

}
