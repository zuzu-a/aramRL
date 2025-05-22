#include "inc/core/systems/TileSystem.hpp" // Adjust path
#include "inc/core/render.hpp" // For Renderer class
#include "inc/core/Tilemap.hpp"
#include "inc/core/Tileset.hpp"
#include <iostream>

TileSystem::TileSystem() {}
TileSystem::~TileSystem() {}

void TileSystem::Update(const Tilemap& tilemap, const Tileset& tilesetResource, Renderer& renderer, float deltaTime) {
    if (tilesetResource.getTexture() == nullptr) {
        // std::cerr << "TileSystem::Update: Tileset texture is null." << std::endl;
        return;
    }

    int tileWidth = tilesetResource.getTileWidth();
    int tileHeight = tilesetResource.getTileHeight();

    if (tileWidth <= 0 || tileHeight <= 0) {
        // std::cerr << "TileSystem::Update: Invalid tile dimensions from tileset." << std::endl;
        return;
    }

    for (int y = 0; y < tilemap.GetHeight(); ++y) {
        for (int x = 0; x < tilemap.GetWidth(); ++x) {
            int tileID = tilemap.GetTileID(x, y);
            if (tileID < 0) continue; // Skip invalid/empty tiles

            SDL_Rect srcRect = tilesetResource.getTileSourceRect(static_cast<Tileset::TypeSize>(tileID));
            if (srcRect.w == 0 || srcRect.h == 0) {
                // std::cerr << "TileSystem::Update: Invalid source rect for tileID " << tileID << std::endl;
                continue; // Skip if source rect is invalid
            }

            SDL_Rect destRect = { x * tileWidth, y * tileHeight, tileWidth, tileHeight };
            
            // Default tint (opaque white)
            SDL_Color tint = {255, 255, 255, 255};

            renderer.DrawTexture(tilesetResource.getTexture(), srcRect, destRect, 0.0, nullptr, SDL_FLIP_NONE, tint);
        }
    }
}
