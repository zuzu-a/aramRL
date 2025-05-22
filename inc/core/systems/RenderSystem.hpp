#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include <entt/entt.hpp>
// #include "inc/core/render.hpp" // Forward declare or include Renderer
// Renderer class is forward declared below

// Forward declare Renderer
class Renderer; 

class RenderSystem {
public:
    RenderSystem();
    ~RenderSystem();

    // Note: Renderer reference is passed here.
    // Alternatively, Renderer could be a constructor argument if RenderSystem is an Engine member.
    void Update(entt::registry& registry, Renderer& renderer, float deltaTime /* for interpolation later */);
};

#endif // RENDER_SYSTEM_HPP
