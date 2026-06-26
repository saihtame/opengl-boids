#pragma once
#include <glad/gl.h>


namespace ParticleSim::Boids {

class BoidsShaderTimer {
public:
    BoidsShaderTimer();
    ~BoidsShaderTimer();

    void start_timer();
    double stop_timer();

private:
    GLuint queries[2];
};

}
