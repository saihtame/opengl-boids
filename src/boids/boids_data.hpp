#pragma once
#include "render/mesh/mesh_render_data.hpp"
#include <glm/ext/vector_float3.hpp>
#include <memory>


namespace ParticleSim::Boids {

struct BoidsData : public Render::Mesh::MeshRenderData {
public:
    BoidsData(const std::shared_ptr<Render::Mesh::Mesh>& mesh, int boids, glm::vec3 bounds);
    ~BoidsData();

    unsigned int instances_BO_A = 0;
    unsigned int instances_BO_B = 0;

    void switch_instance_buffers();

private:
    inline void set_attributes();
};

}
