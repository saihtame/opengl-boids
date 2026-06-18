#pragma once
#include "render/renderable.hpp"
#include <glm/ext/vector_float3.hpp>
#include <memory>
#include <vector>


namespace ParticleSim::Boids {

std::vector<std::shared_ptr<Render::Renderable>> get_scene();

constexpr float scene_width = 50;
constexpr float scene_length = 50;
constexpr float scene_height = 30;
constexpr glm::vec3 scene_offset = glm::vec3(0.0, 0.0, -70.0);

}
