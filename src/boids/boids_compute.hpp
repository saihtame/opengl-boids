#pragma once
#include "boids/boids_data.hpp"
#include <memory>
#include <string>


namespace ParticleSim::Boids {

class BoidsCompute {
public:
    BoidsCompute(const std::shared_ptr<BoidsParams>& params);
    ~BoidsCompute();

    void compute(float delta, BoidsData& data);

private: // Runtime variables
    const int initialized_boids;
    const std::shared_ptr<BoidsParams> params;
    unsigned int program_id = 0;
    std::unordered_map<std::string, unsigned int> uniform_locations;

private: // Constants
    static constexpr int work_group_size = 32;
    const std::string shader_cell_key_path = "shaders/compute/boids_cell_key.comp";
    const std::string shader_grid_sort_path = "shaders/compute/boids_grid_sort.comp";
    const std::string shader_sim_path = "shaders/compute/boids_sim.comp";

private:
    std::string read_file(const std::string& path);
    int get_uniform_location(std::string uniform_name);
    void set_uniforms(float delta);
};

}
