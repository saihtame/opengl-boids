#pragma once
#include "mesh.hpp"
#include <memory>
#include <vector>


namespace ParticleSim::Render::Mesh {

std::shared_ptr<Mesh> generate_cube(float width, float length, float height);
std::shared_ptr<Mesh> generate_square(float width, float height);
std::shared_ptr<Mesh> generate_square_2sides(float width, float height);
std::shared_ptr<Mesh> generate_ico_sphere(float radius, unsigned int subdivisions);
std::vector<float> generate_normals(float* data, unsigned int data_size, unsigned int* indices, unsigned int indices_size);

}
