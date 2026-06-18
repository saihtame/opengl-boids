#pragma once
#include "material/shader_program.hpp"
#include "glad/gl.h"
#include <memory>
#include <string>


namespace ParticleSim::Render {

class CubeMap {
public:
    CubeMap();
    ~CubeMap() = default;

    void initialize(std::string px, std::string nx, std::string py, std::string ny, std::string pz, std::string nz);
    void render(const glm::mat4& view, const glm::mat4& projection);

private:
    const std::string vertex_shader_path = "shaders/render/cubemap.vert";
    const std::string frag_shader_path = "shaders/render/cubemap.frag";

private:
    std::unique_ptr<Material::ShaderProgram> shader_program;
    unsigned int tex_id = 0;
    unsigned int VAO, VBO;

private:
    inline void load_texture(std::string path, GLenum target);
};

}
