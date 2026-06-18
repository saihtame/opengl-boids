#pragma once
#include <string>


namespace ParticleSim::Render::Material {

class Texture {
public:
    Texture(std::string path);

    void use();

private:
    int width;
    int height;
    int channel_c;
    unsigned int texture_id;
};

}
