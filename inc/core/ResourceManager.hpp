#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <map>
#include <string>
// #include <SDL.h>        // Moved to .cpp or more specific headers
// #include <SDL_image.h>  // Moved to .cpp
#include "Tileset.hpp"   // For Tileset objects

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

    // Tileset management
    // Tileset objects themselves will be stored.
    // The Tileset class will be modified to take an SDL_Texture* instead of loading it.
    bool LoadTileset(const std::string& jsonPath, const std::string& textureAlias, const std::string& tilesetAlias);
    Tileset* GetTileset(const std::string& alias); // Return pointer to allow modification if needed, or const ref

private:
    SDL_Renderer* m_renderer = nullptr; // Store renderer for creating textures

    std::map<std::string, SDL_Texture*> m_textures;
    std::map<std::string, Tileset> m_tilesets; 
    // std::map<std::string, TTF_Font*> m_fonts;
    // std::map<std::string, Mix_Chunk*> m_soundEffects;
    
    // Helper to prevent duplicate loads if not desired, or just overwrite
    bool TextureExists(const std::string& alias) const;
    bool TilesetExists(const std::string& alias) const;
};

#endif // RESOURCE_MANAGER_HPP
