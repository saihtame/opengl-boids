#include "app/app.hpp"
#include "app/timer.hpp"
#include "boids/boids_params.hpp"
#include "boids/scene.hpp"
#include "render/renderable.hpp"
#include "render/renderer.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_timer.h>
#include <glad/gl.h>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <iostream>
#include <memory>


namespace ParticleSim::App {

void GLAPIENTRY MessageCallback(GLenum,
                 GLenum type,
                 GLuint,
                 GLenum severity,
                 GLsizei,
                 const GLchar* message,
                 const void*) {
    // Skip notifications
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;
    // Print error
    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}

App::App() {
    setup_window();

    // Create renderer
    renderer = std::make_unique<Render::Renderer>();

    // Create camera
    cam = std::make_shared<Core::Camera>();
    // Move camera
    cam->move_to(glm::vec3(0.0, 0.0, -5.0));
    add_object(cam);

    // Create skybox
    auto skybox = std::make_shared<Render::CubeMap>();
    skybox->initialize("assets/skybox/px.png",
                       "assets/skybox/nx.png",
                       "assets/skybox/py.png",
                       "assets/skybox/ny.png",
                       "assets/skybox/pz.png",
                       "assets/skybox/nz.png");
    renderer->skybox = skybox;

    window_resized(window_width, window_height);

    // create boid scene
    boidsParams = std::make_shared<Boids::BoidsParams>();
    boidsScene = std::make_shared<Boids::Scene>(boidsParams);
    add_renderable(boidsScene);

    // Create UI
    ui = std::make_unique<UI>();
}

App::~App() {
    if (gl_context!= nullptr)
        SDL_GL_DestroyContext(gl_context);
    if (win != nullptr)
        SDL_DestroyWindow(win);
    SDL_Quit();
}

bool App::run() {
    // Draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    // Enable face culling
    glEnable(GL_CULL_FACE);  
    // Enable mouse capture
    SDL_SetWindowRelativeMouseMode(win, true);

    // Delta time
    Timer timer;

    while (!quit) {
        handle_inputs();

        // Get delta time from timer
        double delta = timer.get_delta();

        // Update objects
        for (auto& obj : objects)
            obj->update(delta);

        // Clear the screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Draw screen
        renderer->render(cam.get());

        SDL_GL_SwapWindow(win);
    }

    SDL_SetWindowRelativeMouseMode(win, false);
    return true;
}

void App::add_object(std::shared_ptr<Core::Object3D> obj) {
    objects.push_back(obj);
}

void App::add_renderable(std::shared_ptr<Render::Renderable> obj) {
    add_object(obj);
    renderer->add_renderable(obj);
}

inline void App::setup_window() {
    SDL_Init(SDL_INIT_VIDEO);

    // Set OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create window
    win = SDL_CreateWindow("Particle Sim", window_width, window_height, window_flags);
    if (win == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        std::exit(1);
    }

    // Create OpenGL context
    gl_context = SDL_GL_CreateContext(win);
    if (gl_context == nullptr) {
        std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
        std::exit(1);
    }

    // One of these is correct, depending on how your GLAD files were generated:
    if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        std::exit(1);
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
}

inline void App::handle_inputs() {
    while (SDL_PollEvent(&event)) {
        // Handle quit event
        if (event.type == SDL_EVENT_QUIT) {
            quit = true;
        }
        // Handlw window resize
        if (event.type == SDL_EVENT_WINDOW_RESIZED) {
            window_resized(event.window.data1, event.window.data2);
        } else if (event.type == SDL_EVENT_MOUSE_MOTION) {
            cam->rotate_yaw_pitch(-event.motion.xrel, -event.motion.yrel);
        } else if (event.type == SDL_EVENT_MOUSE_WHEEL) {
            if (event.wheel.direction == SDL_MOUSEWHEEL_NORMAL) {
                cam->zoom(-event.wheel.y);
            } else {
                cam->zoom(event.wheel.y);
            }
        }
    }
}

inline void App::window_resized(float width, float height) {
    window_width = width;
    window_height = height;
    glViewport(0, 0, width, height);
    renderer->render_width = width;
    renderer->render_height = height;
}

}
