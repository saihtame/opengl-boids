#pragma once
#include "boids/boids_params.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include <memory>


namespace ParticleSim::App {

class UI {
public:
    UI(SDL_Window* window, void* gl_context, std::shared_ptr<Boids::BoidsParams> boidsParams);
    ~UI();

    bool processEvent(SDL_Event* event);
    void render();

private:
    static constexpr const char* glsl_version = "#version 460";
    std::shared_ptr<Boids::BoidsParams> boidsParams;
};

}
