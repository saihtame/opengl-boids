#pragma once
#include "boids/boids_data.hpp"
#include <string>


namespace ParticleSim::Boids {

class BoidsCompute {
public:
    BoidsCompute(int boids, glm::vec3 bounds);
    ~BoidsCompute();

    void compute(float boidSpeed, BoidsData& data);

private:
    glm::vec3 bounds;
    unsigned int program_id = 0;
    int dispatches = 0;
    int boids = 0;
    std::unordered_map<std::string, unsigned int> uniform_locations;

private:
    const std::string shader_path = "shaders/compute/boids.comp";

private:
    std::string read_file(const std::string& path);
    int get_uniform_location(std::string uniform_name);
};

}
