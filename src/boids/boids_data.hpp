#pragma once
#include "boids/boids_params.hpp"
#include "render/mesh/mesh_render_data.hpp"
#include <glm/ext/vector_float3.hpp>
#include <memory>


namespace ParticleSim::Boids {

class BoidsData : public Render::Mesh::MeshRenderData {
public:
    BoidsData(const std::shared_ptr<Render::Mesh::Mesh>& mesh, const std::shared_ptr<BoidsParams>& params);
    ~BoidsData();

    unsigned int instances_BO_A = 0;
    unsigned int instances_BO_B = 0;
    unsigned int spatial_grid_cells_BO = 0;
    unsigned int spatial_grid_elements_BO = 0;

    void switch_instance_buffers();

private:
    inline void set_attributes();

private:
    static constexpr unsigned int instance_size = 12; // Size in bytes of each instance
    static constexpr int grid_cell_size = 4; // Size in bytes of each grid cell
    static constexpr int grid_element_size = 12; // Size in bytes of each grid cell element
};

}
