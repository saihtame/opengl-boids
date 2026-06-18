#include <iostream>
#include <string>
#include "cubemap.hpp"
#include "material/shader.hpp"
#include <stb_image.h>
#include <memory>

namespace ParticleSim::Render {

CubeMap::CubeMap() {
}

void CubeMap::initialize(std::string px, std::string nx, std::string py, std::string ny, std::string pz, std::string nz) {
    float vertices[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    // Create VAO
    glGenVertexArrays(1, &VAO);
    // Bind VAO
    glBindVertexArray(VAO);
    // Generate VBO
    glGenBuffers(1, &VBO);
    // Bind VBO buffer to buffer array
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Copy vertices to buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Create and enable vertex attribues
    // Vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Create buffer
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex_id);
    // Load cube map sides
    load_texture(px, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
    load_texture(nx, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
    load_texture(py, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
    load_texture(ny, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
    load_texture(pz, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
    load_texture(nz, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
    // Set texture attributes
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  
    // Load shaders
    Material::Shader vertex_shader(vertex_shader_path, GL_VERTEX_SHADER);
    Material::Shader frag_shader(frag_shader_path, GL_FRAGMENT_SHADER);
    // Link to program
    shader_program = std::make_unique<Material::ShaderProgram>();
    shader_program->attach_shader(vertex_shader);
    shader_program->attach_shader(frag_shader);
    shader_program->link();
}

void CubeMap::render(const glm::mat4& view, const glm::mat4& projection) {
    if (VBO == 0 || VAO == 0 || tex_id == 0) {
        std::cerr << "CubeMap not initialized" << std::endl;
        return;
    }

    // Remove translation from view matrix
    glm::mat4 view_no_translate = glm::mat4(glm::mat3(view));

    if (shader_program != nullptr) {
        shader_program->use();
        shader_program->set_uniform_matrix("view", view_no_translate);
        shader_program->set_uniform_matrix("projection", projection);
        shader_program->set_uniform_int("skybox", 0);
    }

    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex_id);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

inline void CubeMap::load_texture(std::string path, GLenum target) {
    // Load image
    int width, height, channel_c;
    unsigned char* img_data = stbi_load(path.c_str(), &width, &height, &channel_c, 3);
    if (!img_data) {
        std::cerr << "Failed to load image " << path << std::endl;
        stbi_image_free(img_data);
        return;
    }
    glTexImage2D(target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
    stbi_image_free(img_data);
}

}
