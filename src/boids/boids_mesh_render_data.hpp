#pragma once
#include "render/mesh/mesh_render_data.hpp"
#include <glm/ext/vector_float3.hpp>
#include <memory>


namespace ParticleSim::Boids {

struct BoidsMeshRenderData : public Render::Mesh::MeshRenderData {
    BoidsMeshRenderData(const std::shared_ptr<Render::Mesh::Mesh>& mesh, int boids, glm::vec3 pos_offset);
    ~BoidsMeshRenderData();

public:
    unsigned int instances_VBO = 0;
};

}
