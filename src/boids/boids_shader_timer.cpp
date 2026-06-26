#include "boids_shader_timer.hpp"


namespace ParticleSim::Boids {

BoidsShaderTimer::BoidsShaderTimer() {
    glGenQueries(2, queries);
}

BoidsShaderTimer::~BoidsShaderTimer() {
    glDeleteQueries(2, queries);
}

void BoidsShaderTimer::start_timer() {
    glQueryCounter(queries[0], GL_TIMESTAMP);
}

double BoidsShaderTimer::stop_timer() {
    glQueryCounter(queries[1], GL_TIMESTAMP);

    // Wait for query to complete
    int available = 0;
    while (!available) {
        glGetQueryObjectiv(queries[1], GL_QUERY_RESULT_AVAILABLE, &available);
    }

    uint64_t start_time, stop_time;
    glGetQueryObjectui64v(queries[0], GL_QUERY_RESULT, &start_time);
    glGetQueryObjectui64v(queries[1], GL_QUERY_RESULT, &stop_time);

    double elapsed_ms = (stop_time - start_time) / 1e6;
    return elapsed_ms;
}

}
