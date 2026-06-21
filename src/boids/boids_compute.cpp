#include "boids_compute.hpp"
#include "shaders/shader.hpp"
#include "shaders/shader_program.hpp"
#include <glad/gl.h>
#include <glm/ext/vector_float3.hpp>
#include <memory>


namespace ParticleSim::Boids {

BoidsCompute::BoidsCompute(const std::shared_ptr<BoidsParams>& parameters)
    : initialized_boids(parameters->boids), params(parameters) {
    // Create simulation shader
    Shaders::Shader sim_shader(shader_sim_path, GL_COMPUTE_SHADER);
    sim_shader_prog = std::make_unique<Shaders::ShaderProgram>();
    sim_shader_prog->attach_shader(sim_shader);
    sim_shader_prog->link();
}

BoidsCompute::~BoidsCompute() {
}

void BoidsCompute::compute(float delta, BoidsData& data) {
    // Simulation shader
    sim_shader_prog->use();
    set_uniforms(delta);

    // Set instances data input uniform
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, data.instances_BO_A);
    // Set instances data output uniform
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, data.instances_BO_B);

    // Dispatch shader program
    int dispatches = (initialized_boids + 31) / 32;
    glDispatchCompute(dispatches, 1, 1);
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

    // Switch instance data buffers
    data.switch_instance_buffers();
}

void BoidsCompute::set_uniforms(float delta) {
    sim_shader_prog->set_uniform_int("boidCount", initialized_boids);
    sim_shader_prog->set_uniform_vec3("bounds", params->bounds);
    sim_shader_prog->set_uniform_float("boidMaxSpeed", params->boid_max_speed);
    sim_shader_prog->set_uniform_float("boidMinSpeed", params->boid_min_speed);
    sim_shader_prog->set_uniform_float("viewRange", params->view_range);
    sim_shader_prog->set_uniform_float("viewCosine", params->view_cosine);
    sim_shader_prog->set_uniform_float("collisionAvoidanceStrength", params->collision_avoidance_strength);
    sim_shader_prog->set_uniform_float("acceleration", params->acceleration);
    sim_shader_prog->set_uniform_float("seperationFactor", params->seperation_factor);
    sim_shader_prog->set_uniform_float("alignmentFactor", params->alignment_factor);
    sim_shader_prog->set_uniform_float("cohesionFactor", params->cohesion_factor);
    sim_shader_prog->set_uniform_float("delta", delta);
}

}
