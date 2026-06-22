#include "shader_program.hpp"
#include "shader.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


namespace ParticleSim::Shaders {

ShaderProgram::ShaderProgram() {
    program_id = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
    if (program_id != 0)
        glDeleteProgram(program_id);
}

void ShaderProgram::attach_shader(Shader& shader) {
    glAttachShader(program_id, shader.get_id());
}

void ShaderProgram::link() {
    int success;
    char info_log[512];
    glLinkProgram(program_id);
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program_id, 512, NULL, info_log);
        std::cerr << "Could not link shader program\n" << info_log << std::endl;
    }
}

void ShaderProgram::use() {
    glUseProgram(program_id);
}

unsigned int ShaderProgram::get_uniform_location(std::string uniform_name) {
    auto it = uniform_locations.find(uniform_name);
    if (it != uniform_locations.end())
        return it->second;

    unsigned int loc = glGetUniformLocation(program_id, uniform_name.c_str());
    uniform_locations[uniform_name] = loc;
    return loc;
}

void ShaderProgram::set_uniform_matrix(std::string uniform_name, glm::mat4 matrix) {
    unsigned int loc = get_uniform_location(uniform_name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::set_uniform_vec4(std::string uniform_name, glm::vec4 value) {
    unsigned int loc = get_uniform_location(uniform_name);
    glUniform4f(loc, value.x, value.y, value.z, value.w);
}

void ShaderProgram::set_uniform_vec3(std::string uniform_name, glm::vec3 value) {
    unsigned int loc = get_uniform_location(uniform_name);
    glUniform3f(loc, value.x, value.y, value.z);
}

void ShaderProgram::set_uniform_ivec3(std::string uniform_name, glm::ivec3 value) {
    unsigned int loc = get_uniform_location(uniform_name);
    glUniform3i(loc, value.x, value.y, value.z);
}

void ShaderProgram::set_uniform_int(std::string uniform_name, int value) {
    unsigned int loc = get_uniform_location(uniform_name);
    glUniform1i(loc, value);
}

void ShaderProgram::set_uniform_uint(std::string uniform_name, unsigned int value) {
    unsigned int loc = get_uniform_location(uniform_name);
    glUniform1ui(loc, value);
}

void ShaderProgram::set_uniform_float(std::string uniform_name, float value) {
    unsigned int loc = get_uniform_location(uniform_name);
    glUniform1f(loc, value);
}

}
