#ifndef TILESET_HPP
#define TILESET_HPP

#include <string>
#include <vector>
#include <map>
// #include <SDL.h> // Moved to .cpp or specific SDL headers if possible
// #include <nlohmann/json.hpp> // Moved to .cpp

// Forward declarations
struct SDL_Rect;
struct SDL_Texture;
// struct SDL_Renderer; // Removed, was not used

class Tileset {
public:
    using TypeSize = std::size_t;
    using TypeTile = SDL_Rect; // SDL_Rect needs to be defined if used directly like this.
                               // If SDL.h is not included, then TypeTile cannot be SDL_Rect.
                               // For now, assume SDL.h is still needed due to TypeTile = SDL_Rect.
                               // Let's revert this part and keep SDL.h but remove nlohmann/json.hpp
    // Let's try again with this logic:
    // Keep SDL.h for SDL_Rect as it's part of the public interface via TypeTile.
    // Remove nlohmann/json.hpp
    // Remove unused forward declaration for SDL_Renderer.
    
    // Revised plan:
    // Keep <SDL.h> because SDL_Rect is used by value in the public interface (getTileSourceRect)
    // and as a member (m_tileOffset). SDL_Texture* is a member.
    // Move <nlohmann/json.hpp> to the .cpp file.
    // Remove unused `struct SDL_Renderer;`
    // No, the TypeTile is a return type, its definition is needed.
    // And m_tileOffset is a member variable. So SDL.h stays.
    // Only nlohmann/json.hpp can be moved.
    
    // Corrected plan for this turn:
    // Remove <nlohmann/json.hpp> from header, add to cpp.
    // Remove unused `struct SDL_Renderer;` from header.
    // Keep <SDL.h> in header because SDL_Rect is used by value.
    
    // So, the original includes, but nlohmann/json.hpp is removed.
    // And SDL_Renderer forward declaration is removed.
    
    // Actual change for this attempt:
    // Remove nlohmann/json.hpp from this header.
    // Remove forward declaration of SDL_Renderer.
    // The rest of the SDL types are fine being pulled by SDL.h for now.
#include <SDL.h> // For SDL_Rect, SDL_Texture

// Forward declare SDL_Renderer if only pointers are used in header
// struct SDL_Renderer; // Removed as it's not used

class Tileset {
public:
    using TypeSize = std::size_t;
    using TypeTile = SDL_Rect; // Represents the source rect of a tile in the texture
    using TypeName = std::string;

    Tileset(); // Default constructor
    ~Tileset(); // Destructor no longer frees texture (owned by ResourceManager)

    bool loadDataFromJson(const std::string& jsonFilePath, SDL_Texture* tileSheetTexture);

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