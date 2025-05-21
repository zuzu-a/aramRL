#ifndef TILE_SYSTEM_HPP
#define TILE_SYSTEM_HPP

#include <entt/entt.hpp>

// Forward declare Renderer if it's passed to the system
class Renderer; 

class TileSystem {
public:
    TileSystem();
    ~TileSystem();

    // TileSystem might need access to the renderer to draw tiles
    void Update(entt::registry& registry, Renderer& renderer, float deltaTime);
    // Or, if it only updates tile states/components:
    // void Update(entt::registry& registry, float deltaTime);
};

#endif // TILE_SYSTEM_HPP
