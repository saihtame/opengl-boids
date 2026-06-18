#pragma once
#include "render/mesh/mesh.hpp"
#include <memory>


namespace ParticleSim::Render::Mesh {

struct MeshRenderData {
    MeshRenderData(const std::shared_ptr<Mesh>& mesh);
    ~MeshRenderData();

    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
    unsigned int indices_size = 0;
};

}
