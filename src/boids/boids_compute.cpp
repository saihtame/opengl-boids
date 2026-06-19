#include "boids_compute.hpp"
#include "render/material/shader.hpp"
#include <glm/ext/vector_float3.hpp>
#include <iostream>
#include <fstream>


namespace ParticleSim::Boids {

BoidsCompute::BoidsCompute(int boids, glm::vec3 bounds) : bounds(bounds), boids(boids) {
    dispatches = (boids + 31) / 32;

    // Load shader file
    auto shader_str = read_file(shader_path);
    const char* shader_src = shader_str.c_str();

    // Create and compile gl shader
    unsigned int shader_id = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(shader_id, 1, &shader_src, NULL);
    glCompileShader(shader_id);

    // Create shader program
    program_id = glCreateProgram();
    glAttachShader(program_id, shader_id);
    glLinkProgram(program_id);

    // Delete shader
    glDeleteShader(shader_id);
}

BoidsCompute::~BoidsCompute() {
    glDeleteProgram(program_id);
}

void BoidsCompute::compute(float delta, BoidsData& data) {
    // Bind shader program
    glUseProgram(program_id);

    // Set boid speed uniform
    auto loc = get_uniform_location("boidSpeed");
    glUniform1f(loc, 10.0f);

    // Set bounds uniform
    loc = get_uniform_location("bounds");
    glUniform3f(loc, bounds.x, bounds.y, bounds.z);

    // Set boids uniform
    loc = get_uniform_location("boidCount");
    glUniform1i(loc, boids);

    // Set view range uniform
    loc = get_uniform_location("viewRange");
    glUniform1f(loc, 30.0f);

    // Set view angle uniform
    loc = get_uniform_location("viewCosine");
    glUniform1f(loc, 0.0f);

    // Set delta uniform
    loc = get_uniform_location("delta");
    glUniform1f(loc, delta);

    // Set steering force uniform
    loc = get_uniform_location("steeringForce");
    glUniform1f(loc, 1.0f);

    // Set instances data input uniform
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, data.instances_BO_A);

    // Set instances data output uniform
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, data.instances_BO_B);

    // Dispatch shader program
    glDispatchCompute(dispatches, 1, 1);
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

    // Switch instance data buffers
    data.switch_instance_buffers();
}

std::string BoidsCompute::read_file(const std::string& path) {
    std::ifstream shader_file(path, std::ios::in | std::ios::binary);
    if (!shader_file.is_open()) {
        std::cerr << "Failed to open shader file: " << path << std::endl;
        return {};
    }

    return std::string(
        std::istreambuf_iterator<char>(shader_file),
        std::istreambuf_iterator<char>()
    );
}

int BoidsCompute::get_uniform_location(std::string uniform_name) {
    auto it = uniform_locations.find(uniform_name);
    if (it != uniform_locations.end())
        return it->second;

    unsigned int loc = glGetUniformLocation(program_id, uniform_name.c_str());
    uniform_locations[uniform_name] = loc;
    return loc;
}

}
