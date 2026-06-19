#pragma once
#include "render/mesh/mesh_render_data.hpp"
#include <glm/ext/vector_float3.hpp>
#include <memory>


namespace ParticleSim::Boids {

struct BoidsData : public Render::Mesh::MeshRenderData {
    BoidsData(const std::shared_ptr<Render::Mesh::Mesh>& mesh, int boids, glm::vec3 bounds);
    ~BoidsData();

public:
    unsigned int instances_BO = 0;
};

}
