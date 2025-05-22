#ifndef TILE_SYSTEM_HPP
#define TILE_SYSTEM_HPP

// #include <entt/entt.hpp> // No longer needed if not accessing registry directly
#include "inc/core/Tilemap.hpp"
#include "inc/core/Tileset.hpp"

// Forward declare Renderer if it's passed to the system
class Renderer; 

class TileSystem {
public:
    TileSystem();
    ~TileSystem();

    // TileSystem might need access to the renderer to draw tiles
    void Update(const Tilemap& tilemap, const Tileset& tilesetResource, Renderer& renderer, float deltaTime);
};

#endif // TILE_SYSTEM_HPP
