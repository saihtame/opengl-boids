#pragma once
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>


namespace ParticleSim::Boids {

struct BoidsParams {
    int boids = 2048;
    glm::vec3 bounds = glm::vec3(128.0f, 64.0f, 128.0f);
    float boid_max_speed = 25.0f;
    float boid_min_speed = 10.0f;
    float view_range = 20.0f;
    float view_cosine = 0.2f; // 90 degree view angle
    float acceleration = 2.0;
    float seperationFactor = 3.0f;
    float alignmentFactor = 1.0f;
    float cohesionFactor = 0.8f;

    void set_view_cosine(float degrees) {
        view_cosine = glm::cos(glm::radians(degrees));
    }
};

}
