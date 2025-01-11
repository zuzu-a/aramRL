#include <rogue.hpp>

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

class Tilemap {
public:
    Tilemap(int width, int height) : width(width), height(height) {
        tiles.resize(height, std::vector<int>(width, 0)); // Initialize map
    }

    bool isWalkable(int x, int y) const {
        if (x < 0 || x >= width || y < 0 || y >= height) return false;
        return tiles[y][x] == 1; // Example: 1 is walkable, 0 is not
    }

    int getTileID(int x, int y) const {
        return tiles[y][x];
    }

    void setTileID(int x, int y, int tileID) {
        tiles[y][x] = tileID;
    }

private:
    int width, height;
    std::vector<std::vector<int>> tiles;
};