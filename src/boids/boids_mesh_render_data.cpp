#include "boids_mesh_render_data.hpp"
#include "render/mesh/mesh_render_data.hpp"


namespace ParticleSim::Boids {

BoidsMeshRenderData::BoidsMeshRenderData(const std::shared_ptr<Render::Mesh::Mesh>& mesh) 
    : Render::Mesh::MeshRenderData(mesh) {
}

BoidsMeshRenderData::~BoidsMeshRenderData() {
}

}
