#pragma once
#include "boids/boids_compute.hpp"
#include "boids/boids_material.hpp"
#include "boids/boids_data.hpp"
#include "boids/boids_params.hpp"
#include "render/mesh/mesh.hpp"
#include "render/renderable.hpp"
#include <glm/ext/vector_float3.hpp>
#include <memory>


namespace ParticleSim::Boids {

class BoidsSim: public Render::Renderable {
public:
    BoidsSim(const std::shared_ptr<Render::Mesh::Mesh>& mesh, const std::shared_ptr<BoidsParams>& params);
    ~BoidsSim() = default;

    void update(float delta);
    virtual void render(const glm::mat4& view, const glm::mat4& projection) const;
    virtual Render::BlendMode get_blend_mode() const;
    void set_mesh();

private:
    int boids_initialized = 0;
    std::unique_ptr<BoidsMaterial> material;
    std::unique_ptr<BoidsData> data;
    std::unique_ptr<BoidsCompute> compute;
    std::shared_ptr<BoidsParams> params;
};

}
