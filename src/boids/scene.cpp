#include "scene.hpp"
#include "boids/boids_params.hpp"
#include "boids/boids_sim.hpp"
#include "render/material/color_material.hpp"
#include "render/mesh/primitives.hpp"
#include "render/mesh_instance.hpp"
#include "render/renderable.hpp"
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/trigonometric.hpp>
#include <memory>
#include <vector>


namespace ParticleSim::Boids {

std::vector<std::shared_ptr<Render::Renderable>> get_scene() {
    auto renderables = std::vector<std::shared_ptr<Render::Renderable>>();

    /*---- Floor ----*/
    auto floor_mat = std::make_shared<Render::Material::ColorMaterial>();
    floor_mat->color = glm::vec4(0.05f, 0.65f, 0.05f, 1.0f);
    auto floor_roof_mesh = Render::Mesh::generate_square_2sides(scene_width, scene_length);
    auto floor = std::make_shared<Render::MeshInstance>(floor_roof_mesh);
    floor->material = floor_mat;
    floor->move_to(glm::vec3(0.0, -scene_height * 0.7f, 0.0));
    floor->rotate(glm::radians(90.0), glm::vec3(1.0, 0.0, 0.0));
    renderables.push_back(floor);

    /*---- Boids ----*/
    auto boid_mesh = Render::Mesh::generate_cone(0.2f, 1.0f, 3);
    auto bounds = glm::vec3(scene_width, scene_height, scene_length);
    BoidsParams params;
    params.bounds = bounds;
    auto boids_sim = std::make_shared<BoidsSim>(boid_mesh, params);
    boids_sim->move_to(-bounds / 2.0f);
    renderables.push_back(boids_sim);

    /*---- Scene Offset ----*/
    for (auto& r : renderables)
        r->move(scene_offset);

    return renderables;
}

}
