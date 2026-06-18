#include "renderer.hpp"
#include "render/renderable.hpp"
#include <algorithm>


namespace ParticleSim::Render {

void Renderer::add_renderable(std::shared_ptr<Renderable> renderable) {
    BlendMode bm = renderable->get_blend_mode();
 
    if (bm == BLEND_MODE_OPAQUE)
        opaques.push_back(renderable);
    else if (bm == BLEND_MODE_TRANSLUCENT)
        translucents.push_back(renderable);
}

void Renderer::render(const Core::Camera* camera) const {
    // Get view and projection from camera
    const glm::mat4& view = camera->view;
    const glm::mat4 projection = camera->get_projection(render_width, render_height);

    // Draw opaque renderers
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    for (const auto& mesh_renderer : opaques) {
        mesh_renderer->render(view, projection);
    }

    // Draw skybox
    if (skybox != nullptr) {
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);
        skybox->render(view, projection);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
    }

    // Draw transparent renderers
    auto translucents_sorted = sort_translucents(camera);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    for (const auto& draw : translucents_sorted) {
        draw.second->render(view, projection);
    }
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

inline std::vector<std::pair<float, Renderable*>> Renderer::sort_translucents(const Core::Camera* camera) const {
    std::vector<std::pair<float, Renderable*>> sorted;
    // Calculate distances
    sorted.reserve(translucents.size());
    for (auto& r : translucents) {
        float distance = glm::dot(r->world_pos - camera->world_pos, camera->forward());
        sorted.push_back({distance, r.get()});
    }
    // Sort by distances
    std::sort(sorted.begin(), sorted.end(),
        [](const std::pair<float, Renderable*>& a, const std::pair<float, Renderable*>& b) {
            return a.first> b.first;
        });
    return sorted;
}

}
