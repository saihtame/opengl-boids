#pragma once
#include <string>
#include <glad/gl.h>


namespace ParticleSim::Shaders {

class Shader {
public:
    Shader(std::string path, GLenum shader_type);
    ~Shader();
    unsigned int get_id();

private:
    unsigned int shader_id = 0;

private:
    std::string read_file(std::string path);
};

}
