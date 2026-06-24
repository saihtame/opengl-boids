#pragma once
#include "boids/boids_params.hpp"
#include "render/mesh/mesh_render_data.hpp"
#include <cstdint>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_int3.hpp>
#include <memory>


namespace ParticleSim::Boids {

class BoidsData : public Render::Mesh::MeshRenderData {
public:
    BoidsData(const std::shared_ptr<Render::Mesh::Mesh>& mesh, const std::shared_ptr<BoidsParams>& params);
    ~BoidsData();

    void switch_instance_buffers();
    void switch_entries_buffers();

public: // Buffer ids
    // The unique data for each boid resides in instance_BO_A.
    // New data is written into instance_BO_B, after which the buffers are swapped.
    unsigned int instances_BO_A = 0;
    unsigned int instances_BO_B = 0;
    // spatial_grid_cells_BO contains offsets for where in spatial_grid_elements_BO grid cells start and end
    unsigned int spatial_grid_cells_BO = 0;
    // spatial_grid_cells_BO contains indices to instance data in instances_BO.
    unsigned int spatial_grid_elements_BO = 0;
    // spatial_grid_hist buffer contains histograms for each pass the radix sort does
    unsigned int spatial_grid_hist_BO = 0;
    // Spatial grid entries contain a key value pair for each instance.
    // key = instance pos morton encoded to uint64
    // value = instance index
    // Double buffering is used for the radix sort
    unsigned int spatial_grid_entries_A = 0;
    unsigned int spatial_grid_entries_B = 0;
    // The calculated size of the spatial grid where boids are laid out.
    // It is calculated using the params->bounds divided int cells of params->view_range size.
    glm::ivec3 spatial_grid_size;

public: // Structs representing the sizes of the structs in the shaders
    struct Instance {
        float pos_x;
        float pos_y;
        float pos_z;
        float pos_padding;
        float vel_x;
        float vel_y;
        float vel_z;
        float vel_padding;
        uint grid_pos_x;
        uint grid_pos_y;
        uint grid_pos_z;
        uint grid_pos_padding;
    };
    static_assert(sizeof(Instance) == 48);

    struct SortEntry {
        uint32_t key[2];
        uint32_t value;
    };
    static_assert(sizeof(SortEntry) == 12);

    struct Histogram {
        uint32_t buckets[16];
    };
    static_assert(sizeof(Histogram) == 16*sizeof(uint32_t));

    struct GridCell {
        uint32_t StartOffset;
        uint32_t EndOffset;
    };
    static_assert(sizeof(GridCell) == 8);

public: // Buffer sizes.
    unsigned int instances_buffer_size = 0;
    unsigned int grid_cells_buffer_size = 0;
    unsigned int grid_elements_buffer_size = 0;
    unsigned int grid_entries_buffer_size = 0;

private:
    inline void set_vao_attributes();

public:
    // The amount of bits each pass of the radix shader processes
    static constexpr uint32_t grid_radix_bits = 4;
    // The amount of passes the radix shader needs to do
    static constexpr uint32_t grid_radix_passes = 64 / grid_radix_bits;
    // The amount of buckets, for each radix pass
    static constexpr uint32_t grid_radix_buckets = 1 << grid_radix_bits; // 16
    // The total size of the histogram buffer in bytes
    static constexpr uint32_t grid_hist_buffer_size = grid_radix_passes * sizeof(Histogram);
};

}
