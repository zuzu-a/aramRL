#ifndef SYSTEMS_H
#define SYSTEMS_H

#include <rogue.hpp>


class MovementSystem {
public:
    void update(Position& position, const Input& input);
};

class TileMapSystem {
public:
    void update(TileMapComponent& map);
    bool isWalkable(int x, int y, const TileMapComponent& map) const;
};

class RenderingSystem {
public:
    void renderTile(SDL_Renderer* renderer, const SDL_Texture* tilesheet, int tileID, int x, int y, int tileWidth, int tileHeight) const;
};

#endif