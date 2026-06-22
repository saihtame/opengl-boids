#include "ui.hpp"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"
#include <SDL3/SDL_video.h>


namespace ParticleSim::App {


UI::UI(SDL_Window* window, void* gl_context, std::shared_ptr<Boids::BoidsParams> boidsParams)
    : boidsParams(boidsParams) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)
 
    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

UI::~UI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

bool UI::processEvent(SDL_Event* event) {
    ImGui_ImplSDL3_ProcessEvent(event);
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    return (io.WantCaptureKeyboard || io.WantCaptureMouse);
}

void UI::render() {
    if (boidsParams == nullptr) return;

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Boids Simulation Parameters", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    /*---- Initial Parameters ----*/
    ImGui::Text("Initial Parameters (Requires simulation restart)");
    ImGui::SliderInt("Boids in simulation", &boidsParams->boids, 32, 100000);
    ImGui::SliderFloat("Bounds Width", &boidsParams->bounds.x, 32.0f, 1024.0f);
    ImGui::SliderFloat("Bounds Length", &boidsParams->bounds.z, 32.0f, 1024.0f);
    ImGui::SliderFloat("Bounds Height", &boidsParams->bounds.y, 32.0f, 1024.0f);
    ImGui::SliderFloat("View Range", &boidsParams->view_range, 1.0f, 128.0f);
    if (ImGui::Button("Restart Simulation")) {
        boidsParams->should_restart = true;
    }

    /*---- Runtime Parameters ----*/
    ImGui::Text("Runtime Parameters");
    ImGui::SliderFloat("Max Speed", &boidsParams->boid_max_speed, boidsParams->boid_min_speed, 64.0f);
    ImGui::SliderFloat("Min Speed", &boidsParams->boid_min_speed, 0.1f, boidsParams->boid_max_speed);
    float view_degrees = boidsParams->get_view_degrees();
    if (ImGui::SliderFloat("View Angle", &view_degrees, 1.0f, 360.0f)) {
        boidsParams->set_view_cosine(view_degrees);
    }
    ImGui::SliderFloat("Acceleration", &boidsParams->acceleration, 1.0f, 64.0f);
    ImGui::SliderFloat("Collision Avoidance Factor", &boidsParams->collision_avoidance_strength, 0.0f, 64.0f);
    ImGui::SliderFloat("Seperation Factor", &boidsParams->seperation_factor, 0.01f, 64.0f);
    ImGui::SliderFloat("Alignment Factor", &boidsParams->alignment_factor, 0.01f, 64.0f);
    ImGui::SliderFloat("Cohesion Factor", &boidsParams->cohesion_factor, 0.01f, 64.0f);

    // Frame counter
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();

    // Render frame
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

}
