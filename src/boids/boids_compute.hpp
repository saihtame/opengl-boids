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
    std::shared_ptr<BoidsParams> params;
    unsigned int program_id = 0;
    std::unordered_map<std::string, unsigned int> uniform_locations;

private: // Constants
    const int work_group_size = 32;
    const int initialized_boids;
    const std::string shader_path = "shaders/compute/boids.comp";

private:
    std::string read_file(const std::string& path);
    int get_uniform_location(std::string uniform_name);
};

}
