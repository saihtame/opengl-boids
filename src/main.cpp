#include "app/app.hpp"
#include "boids/scene.hpp"


int main() {
    // Init application
    ParticleSim::App::App application;

    // Get boid scene
    auto boid_scene_renderables = ParticleSim::Boids::get_scene();
    for (auto& ren : boid_scene_renderables)
        application.add_renderable(ren);

    // Run
    if (!application.run())
        return 1;

    return 0;
}
