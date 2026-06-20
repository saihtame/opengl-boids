#pragma once
#include "boids/boids_sim.hpp"
#include "render/mesh_instance.hpp"
#include <glm/ext/vector_float3.hpp>
#include <memory>


namespace ParticleSim::Boids {

class Scene : public Render::Renderable {
public:
    Scene(const std::shared_ptr<BoidsParams>& params);

    virtual void update(float delta);
    virtual void render(const glm::mat4& view, const glm::mat4& projection) const;
    virtual Render::BlendMode get_blend_mode() const { return Render::BlendMode::BLEND_MODE_OPAQUE; };
    void reset_scene();

private:
    std::shared_ptr<BoidsParams> params;
    std::unique_ptr<Render::MeshInstance> floor;
    std::unique_ptr<BoidsSim> sim;
};

}
