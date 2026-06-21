#include "shader.hpp"
#include <iostream>
#include <fstream>


namespace ParticleSim::Shaders {

Shader::Shader(std::string path, GLenum shader_type) {
    // Load shader
    std::string shader_str = read_file(path);
    const char* shader_source = shader_str.c_str();

    // Compile shader
    shader_id = glCreateShader(shader_type);
    glShaderSource(shader_id, 1, &shader_source, NULL);
    glCompileShader(shader_id);

    // Check for errors
    int success;
    char info_log[512];
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader_id, 512, NULL, info_log);
        std::cerr << "Failed to compile shader " << path << "\n" << info_log << std::endl;
    }
}

Shader::~Shader() {
    // Delete shader
    if (shader_id != 0) {
        glDeleteShader(shader_id);
    }
}

unsigned int Shader::get_id() {
    return shader_id;
}

std::string Shader::read_file(std::string path) {
    std::ifstream shader_file(path, std::ios::in | std::ios::binary);
    if (!shader_file.is_open()) {
        std::cerr << "Failed to open shader file: " << path << std::endl;
        return {};
    }

    return std::string(
        std::istreambuf_iterator<char>(shader_file),
        std::istreambuf_iterator<char>()
    );
}

}
