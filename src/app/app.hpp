#pragma once
#include "app/ui.hpp"
#include "core/camera.hpp"
#include "core/object_3d.hpp"
#include "render/renderable.hpp"
#include "render/renderer.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <memory>

namespace ParticleSim::App {

class App {
public:
    App();
    ~App();

    bool run();

    void add_object(std::shared_ptr<Core::Object3D> obj);
    void add_renderable(std::shared_ptr<Render::Renderable> obj);

private: // Constants
    const int window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

private: // Runtime properties
    // Window & context
    int window_width = 640*2;
    int window_height = 480*2;
    SDL_Window* win = nullptr;
    SDL_GLContext gl_context = nullptr;
    // Input handling
    bool quit = false;
    SDL_Event event;
    // Camera
    std::shared_ptr<Core::Camera> cam;
    // Objects
    std::vector<std::shared_ptr<Core::Object3D>> objects;
    std::unique_ptr<Render::Renderer> renderer;
    std::unique_ptr<UI> ui;

private:
    inline void setup_window();
    inline void handle_inputs();
    inline void window_resized(float width, float height);
};

}
