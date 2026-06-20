#include "scene.hpp"
#include "boids/boids_params.hpp"
#include "boids/boids_sim.hpp"
#include "render/material/color_material.hpp"
#include "render/mesh/primitives.hpp"
#include "render/mesh_instance.hpp"
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/trigonometric.hpp>
#include <memory>


namespace ParticleSim::Boids {

Scene::Scene(const std::shared_ptr<BoidsParams>& params) : params(params) {
    reset_scene();
}

void Scene::update(float delta) {
    if (sim != nullptr)
        sim->update(delta);
}

void Scene::render(const glm::mat4& view, const glm::mat4& projection) const {
    if (floor != nullptr)
        floor->render(view, projection);

    if (sim != nullptr)
        sim->render(view, projection);
}

void Scene::reset_scene() {
    glm::vec3 offset = glm::vec3(0.0f, 0.0f, -params->bounds.z);

    /*---- Floor ----*/
    auto floor_mat = std::make_shared<Render::Material::ColorMaterial>();
    floor_mat->color = glm::vec4(0.05f, 0.65f, 0.05f, 1.0f);
    auto floor_roof_mesh = Render::Mesh::generate_square_2sides(params->bounds.x, params->bounds.z);
    floor = std::make_unique<Render::MeshInstance>(floor_roof_mesh);
    floor->material = floor_mat;
    floor->move_to(glm::vec3(0.0, -params->bounds.y* 0.7f, 0.0) + offset);
    floor->rotate(glm::radians(90.0), glm::vec3(1.0, 0.0, 0.0));

    /*---- Boids ----*/
    auto boid_mesh = Render::Mesh::generate_cone(0.2f, 1.0f, 2);
    sim = std::make_unique<BoidsSim>(boid_mesh, params);
    sim->move_to(-params->bounds / 2.0f + offset);
}

}
