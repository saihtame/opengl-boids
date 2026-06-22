#pragma once
#include "shader.hpp"
#include <glm/ext/matrix_float4x4.hpp>
#include <unordered_map>


namespace ParticleSim::Shaders {

class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram();

    void attach_shader(Shader& shader);
    void link();
    void use();
    unsigned int get_uniform_location(std::string uniform_name);
    void set_uniform_matrix(std::string uniform_name, glm::mat4 value);
    void set_uniform_vec4(std::string uniform_name, glm::vec4 value);
    void set_uniform_vec3(std::string uniform_name, glm::vec3 value);
    void set_uniform_ivec3(std::string uniform_name, glm::ivec3 value);
    void set_uniform_int(std::string uniform_name, int value);
    void set_uniform_uint(std::string uniform_name, unsigned int value);
    void set_uniform_float(std::string uniform_name, float value);

private:
    unsigned int program_id = 0;
    std::unordered_map<std::string, GLint> uniform_locations;
};

}
