#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <map>
#include <string>
// #include <SDL.h>        // Moved to .cpp or more specific headers
// #include <SDL_image.h>  // Moved to .cpp
#include "Tileset.hpp"   // For Tileset objects
#include "inc/core/content/CharacterDefinition.hpp"
#include "inc/core/content/ItemDefinition.hpp"
#include "inc/core/content/TilesetDefinition.hpp"
// Add others as created (e.g., MonsterDefinition)


// Forward declarations for SDL types
struct SDL_Renderer;
struct SDL_Texture;

// Potentially: #include "Font.hpp", "Sound.hpp" in the future

class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();

    // Call this once SDL_Renderer is available
    void Initialize(SDL_Renderer* renderer); // SDL_Renderer* used here

    // Texture management
    SDL_Texture* LoadTexture(const std::string& filePath, const std::string& alias);
    SDL_Texture* GetTexture(const std::string& alias);

    // Tileset management (using TilesetDefinition)
    bool LoadTilesetFromDefinition(const std::string& tilesetDefAlias); // New method
    Tileset* GetTileset(const std::string& alias); // Existing, but now refers to tilesets loaded via definition


    // Character Definitions
    bool LoadCharacterDefinition(const std::string& filePath, const std::string& alias);
    Content::CharacterDefinition* GetCharacterDefinition(const std::string& alias);

    // Item Definitions
    bool LoadItemDefinition(const std::string& filePath, const std::string& alias);
    Content::ItemDefinition* GetItemDefinition(const std::string& alias);

    // Tileset Definitions
    bool LoadTilesetDefinition(const std::string& filePath, const std::string& alias);
    Content::TilesetDefinition* GetTilesetDefinition(const std::string& alias);

private:
    SDL_Renderer* m_renderer = nullptr; // Store renderer for creating textures

    std::map<std::string, SDL_Texture*> m_textures;
    std::map<std::string, Tileset> m_tilesets; // Tilesets loaded and ready to use
    std::map<std::string, Content::CharacterDefinition> m_characterDefinitions;
    std::map<std::string, Content::ItemDefinition> m_itemDefinitions;
    std::map<std::string, Content::TilesetDefinition> m_tilesetDefinitions;
    // std::map<std::string, TTF_Font*> m_fonts;
    // std::map<std::string, Mix_Chunk*> m_soundEffects;
    
    // Helper to prevent duplicate loads if not desired, or just overwrite
    bool TextureExists(const std::string& alias) const;
    bool TilesetExists(const std::string& alias) const; // For loaded Tileset instances
    // Add Exists checks for definitions if needed, e.g. CharacterDefinitionExists
};

#endif // RESOURCE_MANAGER_HPP
