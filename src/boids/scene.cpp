#include "scene.hpp"
#include "boids/boids_sim.hpp"
#include "render/blend_mode.hpp"
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

    /*---- Materials ----*/
    auto floor_mat = std::make_shared<Render::Material::ColorMaterial>();
    auto trans_mat = std::make_shared<Render::Material::ColorMaterial>();
    trans_mat->color = glm::vec4(0.5, 0.5, 0.5, 0.2);
    trans_mat->blend_mode = Render::BLEND_MODE_TRANSLUCENT;
    floor_mat->color = glm::vec4(0.0, 1.0, 0.0, 1.0);
    auto invisible_mat = std::make_shared<Render::Material::ColorMaterial>();
    invisible_mat->color = glm::vec4(1.0, 1.0, 1.0, 0.0);
    invisible_mat->blend_mode = Render::BLEND_MODE_TRANSLUCENT;

    /*---- Floor and roof ----*/
    auto floor_roof_mesh = Render::Mesh::generate_square_2sides(scene_width, scene_length);
    // Floor
    auto floor = std::make_shared<Render::MeshInstance>(floor_roof_mesh);
    floor->material = floor_mat;
    floor->move_to(glm::vec3(0.0, -scene_height / 2, 0.0));
    floor->rotate(glm::radians(90.0), glm::vec3(1.0, 0.0, 0.0));
    renderables.push_back(floor);
    // Roof
    auto roof = std::make_shared<Render::MeshInstance>(floor_roof_mesh);
    roof->material = trans_mat;
    roof->move_to(glm::vec3(0.0, scene_height / 2, 0.0));
    roof->rotate(glm::radians(90.0), glm::vec3(1.0, 0.0, 0.0));
    renderables.push_back(roof);

    /*---- Walls ----*/
    // Wall meshes
    auto wall_mesh_fb = Render::Mesh::generate_square_2sides(scene_width, scene_height);
    auto wall_mesh_lr = Render::Mesh::generate_square_2sides(scene_length, scene_height);
    // Front wall
    auto wall_f = std::make_shared<Render::MeshInstance>(wall_mesh_fb);
    wall_f->material = invisible_mat;
    wall_f->move_to(glm::vec3(0.0, 0.0, scene_length/2));
    renderables.push_back(wall_f);
    // Back wall
    auto wall_b = std::make_shared<Render::MeshInstance>(wall_mesh_fb);
    wall_b->material = trans_mat;
    wall_b->move_to(glm::vec3(0.0, 0.0, -scene_length/2));
    renderables.push_back(wall_b);
    // Left wall
    auto wall_l = std::make_shared<Render::MeshInstance>(wall_mesh_lr);
    wall_l->material = trans_mat;
    wall_l->move_to(glm::vec3(-scene_width/2, 0.0, 0.0));
    wall_l->rotate(glm::radians(90.0), glm::vec3(0.0, 1.0, 0.0));
    renderables.push_back(wall_l);
    // Right wall
    auto wall_r = std::make_shared<Render::MeshInstance>(wall_mesh_lr);
    wall_r->material = trans_mat;
    wall_r->move_to(glm::vec3(scene_width/2, 0.0, 0.0));
    wall_r->rotate(glm::radians(90.0), glm::vec3(0.0, 1.0, 0.0));
    renderables.push_back(wall_r);

    /*---- Boids ----*/
    auto boid_mesh = Render::Mesh::generate_cone(0.2f, 1.0f, 3);
    auto bounds = glm::vec3(scene_width, scene_height, scene_length);
    auto boids_sim = std::make_shared<BoidsSim>(boid_mesh, 10000, bounds);
    boids_sim->move_to(-bounds * 0.5f);
    renderables.push_back(boids_sim);

    /*---- Scene Offset ----*/
    for (auto& r : renderables)
        r->move(scene_offset);

    return renderables;
}

}
