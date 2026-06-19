#pragma once
#include "boids/boids_params.hpp"
#include "render/mesh/mesh_render_data.hpp"
#include <glm/ext/vector_float3.hpp>
#include <memory>


namespace ParticleSim::Boids {

struct BoidsData : public Render::Mesh::MeshRenderData {
public:
    BoidsData(const std::shared_ptr<Render::Mesh::Mesh>& mesh, const BoidsParams& params);
    ~BoidsData();

    const BoidsParams& params;
    unsigned int instances_BO_A = 0;
    unsigned int instances_BO_B = 0;

    void switch_instance_buffers();

private:
    inline void set_attributes();
};

}
