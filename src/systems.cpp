#include <systems.hpp>

/*  - - - - - - - - - - - - - - */
//  MOVEMENT SYSTEM             //
/*  - - - - - - - - - - - - - - */


class MovementSystem {
public:
    void update(Position& position, const Input& input) {
        if (input.moveUp) position.y--;
        if (input.moveDown) position.y++;
        if (input.moveLeft) position.x--;
        if (input.moveRight) position.x++;
    }
};

/*  - - - - - - - - - - - - - - */
//  TILESET SYSTEM              //
/*  - - - - - - - - - - - - - - */


class TilesetSystem {
    public:
        Tileset(SDL_Renderer *renderer, const std::string& filePath, int tileWidth, int tileHeight) : tileWidth(tileWidth), tileHeight(tileHeight) {
            // Load tilesheet's texture.
            SDL_Surface *surface = IMG_Load(filePath.c_str());

            if (!surface) {
                throw std::runtime_error("Failed to load tilesheet: " + std::string(SDL_GetError()));
            }

            tilesheet = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);

            // Retrieve dimensions of tilesheet.
            SDL_QueryTexture(tilesheet, nullptr, nullptr, &sheetWidth, &sheetHeight);
        }

        ~Tileset() {
            SDL_DestroyTexture(tilesheet);
        }

        // Calculate the source rectangle for a tile.
        SDL_Rect getTileRect(int tileID) const {
            int tilesPerRow = sheetWidth / tileWidth;
            int x = (tileID % tilesPerRow) * tileWidth;
            int y = (tileID / tilesPerRow) * tileHeight;

            return { x, y, tileWidth, tileHeight };
        }

        // Render a tile on the screen.
        void renderTile(SDL_Renderer *renderer, int tileID, int x, int y) const {
            SDL_Rect src = getTileRect(tileID);
            SDL_Rect dest = { x, y, tileWidth, tileHeight };
            SDL_RenderCopy(renderer, tilesheet, &src, &dest);
        }

        int getTileWidth()  const { return tileWidth;  }
        int getTileHeight() const { return tileHeight; }
    
    private: 
        SDL_Texture *tilesheet;
        int         tileWidth;
        int         tileHeight;
        int         sheetWidth;
        int         sheetHeight;
};

/*  - - - - - - - - - - - - - - */
//  TILEMAP SYSTEM              //
/*  - - - - - - - - - - - - - - */

class TilemapSystem {
public:
    void update(TileMapComponent& map) {
        // Logic to update the map based on game rules, player actions, etc.
    }

    bool isWalkable(int x, int y, const TileMapComponent& map) {
        if (x < 0 || x >= map.width || y < 0 || y >= map.height) return false;
        return map.tiles[y][x] == 1; // Walkable tile
    }
};

/*  - - - - - - - - - - - - - - */
//  RENDERING SYSTEM            //
/*  - - - - - - - - - - - - - - */

class RenderingSystem {
public:
    void renderTile(SDL_Renderer* renderer, const SDL_Texture* tilesheet, int tileID, int x, int y, int tileWidth, int tileHeight) const {
        SDL_Rect src = getTileRect(tileID, tileWidth, tileHeight);
        SDL_Rect dest = { x, y, tileWidth, tileHeight };
        SDL_RenderCopy(renderer, tilesheet, &src, &dest);
    }

private:
    SDL_Rect getTileRect(int tileID, int tileWidth, int tileHeight) const {
        int tilesPerRow = sheetWidth / tileWidth;
        int x = (tileID % tilesPerRow) * tileWidth;
        int y = (tileID / tilesPerRow) * tileHeight;
        return { x, y, tileWidth, tileHeight };
    }
};