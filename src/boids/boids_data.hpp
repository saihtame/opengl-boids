#pragma once
#include "boids/boids_params.hpp"
#include "render/mesh/mesh_render_data.hpp"
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_int3.hpp>
#include <memory>


namespace ParticleSim::Boids {

class BoidsData : public Render::Mesh::MeshRenderData {
public:
    BoidsData(const std::shared_ptr<Render::Mesh::Mesh>& mesh, const std::shared_ptr<BoidsParams>& params);
    ~BoidsData();

    // The unique data for each boid resides in instance_BO_A.
    // New data is written into instance_BO_B, after which the buffers are swapped.
    unsigned int instances_BO_A = 0;
    unsigned int instances_BO_B = 0;
    // spatial_grid_cells_BO contains offsets for where in spatial_grid_elements_BO grid cells start and end
    unsigned int spatial_grid_cells_BO = 0;
    // spatial_grid_cells_BO contains indices to instance data in instances_BO.
    unsigned int spatial_grid_elements_BO = 0;
    // Spatial grid entries contain a key value pair for each instance.
    // key = instance pos morton encoded to uint64
    // value = instance index
    // Double buffering is used for the radix sort
    unsigned int spatial_grid_entries_A = 0;
    unsigned int spatial_grid_entries_B = 0;
    // The calculated size of the spatial grid where boids are laid out.
    // It is calculated using the params->bounds divided int cells of params->view_range size.
    glm::ivec3 spatial_grid_size;

    void switch_instance_buffers();

private:
    inline void set_attributes();

public:
    static constexpr unsigned int instance_size = 48;    // Size in bytes of each instance
    static constexpr unsigned int instance_float_size = instance_size / sizeof(float);
    static constexpr unsigned int grid_cell_size = 8;    // Size in bytes of each grid cell (uint start, uint end)
    static constexpr unsigned int grid_element_size = 4; // Size in bytes of each grid cell element (uint value)
    static constexpr unsigned int grid_entry_size = 12;  // Size in bytes of each sorting entry (key = 8 bytes, value = 4)
};

}
