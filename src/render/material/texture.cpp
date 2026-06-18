#include "texture.hpp"
#include "shader.hpp"
#include <iostream>
#include <stb_image.h>


namespace ParticleSim::Render::Material {

Texture::Texture(std::string path) {
    // Load data
    stbi_set_flip_vertically_on_load(true);
    unsigned char* img_data = stbi_load(path.c_str(), &width, &height, &channel_c, 4);
    // Check for error loading image
    if (!img_data) {
        std::cerr << "Failed to load image " << path << std::endl;
    } else {
        // Determine image format
        // Generate open gl texture
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_data);
        // Set texture repeating
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // Set bi-linear texture filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Generate mipmaps for texture
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    // Free data
    stbi_image_free(img_data);
}

void Texture::use() {
    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, texture_id);
}

}
