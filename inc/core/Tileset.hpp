#ifndef TILESET_HPP
#define TILESET_HPP

#include <string>
#include <vector>
#include <map>
#include <SDL.h> // For SDL_Rect, SDL_Texture
#include <nlohmann/json.hpp> // For JSON parsing

// Forward declare SDL_Renderer if only pointers are used in header
struct SDL_Renderer;

class Tileset {
public:
    using TypeSize = std::size_t;
    using TypeTile = SDL_Rect; // Represents the source rect of a tile in the texture
    using TypeName = std::string;

    Tileset(); // Default constructor
    ~Tileset(); // Destructor to free texture

    bool loadTileSet(SDL_Renderer* renderer, const std::string& jsonFilePath, const std::string& texturePath);

    // Get the source rectangle for a tile ID
    TypeTile getTileSourceRect(TypeSize tileID) const; 
    
    SDL_Texture* getTexture() const noexcept { return m_texture; }
    
    TypeSize getColumns() const noexcept { return m_columns; }
    TypeSize getRows() const noexcept { return m_rows; }
    int getTileWidth() const noexcept { return m_tileWidth; }
    int getTileHeight() const noexcept { return m_tileHeight; }
    int getTileCount() const noexcept { return m_tileCount; }
    
    bool isValidTileID(TypeSize tileID) const noexcept { return tileID < m_tileCount; }

    // Placeholder for accessing tile properties if needed later
    // const nlohmann::json& getTileProperties(TypeSize tileID) const;

private:
    SDL_Texture* m_texture = nullptr;
    SDL_Rect m_tileOffset = {0,0,0,0}; // Offset for drawing tiles, if specified in JSON

    int m_tileWidth = 0;
    int m_tileHeight = 0;
    TypeSize m_rows = 0;
    TypeSize m_columns = 0;
    int m_tileCount = 0; // Total number of tiles in the tileset

    std::string m_name;

    // Store individual tile data if tiles have specific properties, types, etc.
    // For now, we assume tiles are simply indexed from 0 to tilecount-1.
    // std::vector<nlohmann::json> m_tileData; // To store properties for each tile
    
    // Optional: for named/typed tiles, if needed
    // std::map<TypeName, TypeSize> m_namedTiles;
    // std::multimap<TypeName, TypeSize> m_typedTiles;
};

#endif // TILESET_HPP