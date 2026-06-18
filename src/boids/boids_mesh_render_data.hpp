#pragma once
#include "render/mesh/mesh_render_data.hpp"
#include <memory>


namespace ParticleSim::Boids {

struct BoidsMeshRenderData : public Render::Mesh::MeshRenderData {
    BoidsMeshRenderData(const std::shared_ptr<Render::Mesh::Mesh>& mesh);
    ~BoidsMeshRenderData();

public:
    unsigned int instances_VBO = 0;
};

}
