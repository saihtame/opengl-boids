#pragma once
#include "core/camera.hpp"
#include "render/renderer.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <memory>

namespace ParticleSim::App {

class App {
public:
    App();
    ~App();

    bool initialize();
    bool run();

private: // Constants
    const int window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

private: // Runtime properties
    // Window & context
    int window_width = 640;
    int window_height = 480;
    SDL_Window* win = nullptr;
    SDL_GLContext gl_context = nullptr;
    // Input handling
    bool quit = false;
    SDL_Event event;
    // Camera
    std::unique_ptr<Core::Camera> cam;
    std::unique_ptr<Render::Renderer> renderer;

private:
    inline void handle_inputs();
    inline void window_resized(float width, float height);
};

}
