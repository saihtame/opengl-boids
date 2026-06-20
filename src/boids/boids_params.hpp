#pragma once
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>


namespace ParticleSim::Boids {

struct BoidsParams {
    // Boids Sim initialization parameters
    // Requires a simulation restart to take effect
    glm::vec3 bounds = glm::vec3(128.0f, 64.0f, 128.0f);
    int boids = 30000;
    // Boids Sim runtime parameters
    // Can be modified while running the sim
    float boid_max_speed = 20.0f;
    float boid_min_speed = 10.0f;
    float view_range = 20.0f;
    float view_cosine = -0.5f; // 90 degree view angle
    float collision_avoidance_strength = 10.0f;
    float acceleration = 8.0;
    float seperationFactor = 3.0f;
    float alignmentFactor = 0.40f;
    float cohesionFactor = 0.1f;

    void set_view_cosine(float degrees) {
        view_cosine = glm::cos(glm::radians(double(degrees / 2)));
    }

    float get_view_degrees() {
        return glm::degrees(glm::acos(view_cosine)) * 2;
    }
};

}
