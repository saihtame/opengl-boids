#pragma once
#include "mesh.hpp"
#include <memory>


namespace ParticleSim::Render::Mesh {

std::shared_ptr<Mesh> generate_square_2sides(float width, float height);
std::shared_ptr<Mesh> generate_pyramid(float width, float length);
std::shared_ptr<Mesh> generate_cone(float radius, float length, int subdivisions); 

}
