#pragma once
#include "core/camera.hpp"
#include "cubemap.hpp"
#include "renderable.hpp"
#include <memory>
#include <utility>
#include <vector>


namespace ParticleSim::Render {

class Renderer {
public:
    void add_renderable(const std::shared_ptr<Renderable> renderable);

    virtual void render(const Core::Camera* camera) const;

public: // Public properties
    std::shared_ptr<CubeMap> skybox;
    int render_width, render_height;

private: // Lists
    std::vector<std::shared_ptr<Renderable>> opaques;
    std::vector<std::shared_ptr<Renderable>> translucents;

private: // Helper methods
    inline std::vector<std::pair<float, Renderable*>> sort_translucents(const Core::Camera* camera) const;
};

}
