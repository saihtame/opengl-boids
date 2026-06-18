#pragma once
#include "boids/boids_material.hpp"
#include "boids/boids_mesh_render_data.hpp"
#include "render/mesh/mesh.hpp"
#include "render/renderable.hpp"
#include <memory>


namespace ParticleSim::Boids {

class BoidsSim: public Render::Renderable {
public:
    BoidsSim(const std::shared_ptr<Render::Mesh::Mesh>& value, int boids, glm::vec3 bounds);
    ~BoidsSim() = default;

    void update(float delta);
    virtual void render(const glm::mat4& view, const glm::mat4& projection) const;
    virtual Render::BlendMode get_blend_mode() const;
    void set_mesh();

private:
    std::unique_ptr<BoidsMaterial> material;
    std::unique_ptr<BoidsMeshRenderData> render_data;
    int boids;
    glm::vec3 bounds;
};

}
