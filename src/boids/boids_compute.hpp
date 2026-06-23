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
    std::unique_ptr<Shaders::ShaderProgram> spatial_key_shader_prog;
    std::unique_ptr<Shaders::ShaderProgram> spatial_hist_shader_prog;
    std::unique_ptr<Shaders::ShaderProgram> spatial_sort_shader_prog;
    std::unique_ptr<Shaders::ShaderProgram> spatial_post_shader_prog;
    std::unique_ptr<Shaders::ShaderProgram> sim_shader_prog;

private: // Constants
    static constexpr int work_group_size = 32; // Work group x size defined in shaders
    // Paths to shaders files
    const std::string spatial_key_shader_path = "shaders/compute/boids_spatial_key.comp";
    const std::string spatial_hist_shader_path = "shaders/compute/boids_spatial_hist.comp";
    const std::string spatial_radix_shader_path = "shaders/compute/boids_spatial_radix.comp";
    const std::string spatial_post_shader_path = "shaders/compute/boids_spatial_radix.comp";
    const std::string shader_sim_path = "shaders/compute/boids_sim.comp";

private:
    inline void run_grid_key_shader(const BoidsData& data);
    inline void run_grid_hist_shader(const BoidsData& data);
    inline void run_grid_sort_shader(BoidsData& data);
    inline void run_grid_post_shader(BoidsData& data);
    inline void run_sim_shader(float delta, const BoidsData& data);
};

}
