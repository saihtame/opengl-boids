#pragma once
#include "boids/boids_data.hpp"
#include "shaders/shader_program.hpp"
#include <memory>
#include <string>


namespace ParticleSim::Boids {

class BoidsCompute {
public:
    BoidsCompute(const std::shared_ptr<BoidsParams>& params);
    ~BoidsCompute() = default;

    void compute(float delta, BoidsData& data);

private: // Runtime variables
    const unsigned int initialized_boids;
    const std::shared_ptr<BoidsParams> params;
    std::unique_ptr<Shaders::ShaderProgram> cell_key_shader_prog;
    std::unique_ptr<Shaders::ShaderProgram> grid_sort_shader_prog;
    std::unique_ptr<Shaders::ShaderProgram> sim_shader_prog;

private: // Constants
    static constexpr int work_group_size = 32; // Work group x size defined in shaders
    static constexpr int radix_passes = 16;    // 64 bit key, 4 bits per pass, gives 16 radix passes
    // Paths to shaders files
    const std::string shader_cell_key_path = "shaders/compute/boids_cell_key.comp";
    const std::string shader_grid_sort_path = "shaders/compute/boids_grid_sort.comp";
    const std::string shader_sim_path = "shaders/compute/boids_sim.comp";

private:
    inline void cell_grid_key_shader(const BoidsData& data);
    inline void run_grid_sort_shader(const BoidsData& data);
    inline void run_sim_shader(float delta, const BoidsData& data);
};

}
