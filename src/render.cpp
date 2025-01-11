#include <rogue.hpp>

/*  - - - - - - - - - - - - - - */
//  RENDERING SYSTEM            //
/*  - - - - - - - - - - - - - - */

class RenderingSystem {
public:
    void addEntity(Entity entity) {
        renderableEntities.push_back(entity);
    }

    void removeEntity(Entity entity) {
        renderableEntities.erase(std::remove(renderableEntities.begin(), renderableEntities.end(), entity), renderableEntities.end());
    }

    void renderEntities(SDL_Renderer* renderer, EntityManager& entityManager, const Tileset& tileset) {
        for (auto entity : renderableEntities) {
            auto position = entityManager.getComponent<Position>(entity);
            auto renderable = entityManager.getComponent<Tile>(entity);

            if (position && renderable) {
                tileset.renderTile(renderer, renderable->tileID,
                                   static_cast<int>(position->x),
                                   static_cast<int>(position->y));
            }
        }
    }

private:
    SDL_Rect getTileRect(int tileID, int tileWidth, int tileHeight) const {
        int tilesPerRow = sheetWidth / tileWidth;
        int x = (tileID % tilesPerRow) * tileWidth;
        int y = (tileID / tilesPerRow) * tileHeight;
        return { x, y, tileWidth, tileHeight };
    }

    std::vector<Entity> renderableEntities;
};