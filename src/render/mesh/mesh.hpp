#pragma once


namespace ParticleSim::Render::Mesh {

class Mesh {
public:
    Mesh();
    ~Mesh();

    void set_data(void* data, unsigned int data_size, unsigned int* indices, int indices_size);
    void render();

private:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
    unsigned int indices_size = 0;

private:
    inline void delete_buffers();
};

}
