#pragma once
#include "boids/boids_compute.hpp"
#include "boids/boids_material.hpp"
#include "boids/boids_data.hpp"
#include "render/mesh/mesh.hpp"
#include "render/renderable.hpp"
#include <glm/ext/vector_float3.hpp>
#include <memory>


namespace ParticleSim::Boids {

class BoidsSim: public Render::Renderable {
public:
    BoidsSim(const std::shared_ptr<Render::Mesh::Mesh>& value);
    ~BoidsSim() = default;

    void update(float delta);
    virtual void render(const glm::mat4& view, const glm::mat4& projection) const;
    virtual Render::BlendMode get_blend_mode() const;
    void set_mesh();

private:
    std::unique_ptr<BoidsMaterial> material;
    std::unique_ptr<BoidsData> data;
    std::unique_ptr<BoidsCompute> compute;
    int boids = 5000;
    float boid_speed = 10.0f;
    glm::vec3 bounds = glm::vec3(128.0f, 64.0f, 128.0f);
    glm::ivec3 grid_size = glm::ivec3(64, 32, 64);
    glm::vec3 grid_box_size = glm::vec3(2.0f, 2.0f, 2.0f);
};

}
