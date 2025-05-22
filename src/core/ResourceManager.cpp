#include "inc/core/ResourceManager.hpp" // Adjust path
#include <SDL.h>        // For SDL_DestroyTexture etc.
#include <SDL_image.h>  // For IMG_LoadTexture
#include <iostream>     // For error messages
#include <fstream>      // For Tileset JSON loading

ResourceManager::ResourceManager() : m_renderer(nullptr) {}

ResourceManager::~ResourceManager() {
    // Release resources
    for (auto& pair : m_textures) {
        if (pair.second) {
            SDL_DestroyTexture(pair.second);
        }
    }
    m_textures.clear();
    m_tilesets.clear(); // Tileset destructor should handle its internal cleanup (but not SDL_Texture it doesn't own)
    // Similarly for fonts, sounds etc.
}

void ResourceManager::Initialize(SDL_Renderer* renderer) {
    m_renderer = renderer;
}

SDL_Texture* ResourceManager::LoadTexture(const std::string& filePath, const std::string& alias) {
    if (!m_renderer) {
        std::cerr << "ResourceManager not initialized with a renderer!" << std::endl;
        return nullptr;
    }
    if (TextureExists(alias)) {
        std::cerr << "Texture alias '" << alias << "' already exists. Returning existing." << std::endl;
        return m_textures[alias];
    }

    SDL_Texture* texture = IMG_LoadTexture(m_renderer, filePath.c_str());
    if (!texture) {
        std::cerr << "Failed to load texture '" << filePath << "': " << IMG_GetError() << std::endl;
        return nullptr;
    }
    m_textures[alias] = texture;
    return texture;
}

SDL_Texture* ResourceManager::GetTexture(const std::string& alias) {
    if (!TextureExists(alias)) {
        std::cerr << "Texture with alias '" << alias << "' not found." << std::endl;
        return nullptr;
    }
    return m_textures[alias];
}
 
bool ResourceManager::TextureExists(const std::string& alias) {
    return m_textures.count(alias) > 0;
}

// bool ResourceManager::TextureExists(const std::string& alias) { // Removed non-const
//     return m_textures.count(alias) > 0;
// }

bool ResourceManager::LoadTileset(const std::string& jsonPath, const std::string& textureAlias, const std::string& tilesetAlias) {
    if (!m_renderer) { // Although Tileset::loadDataFromJson doesn't directly use it, RM needs it for GetTexture
         std::cerr << "ResourceManager not initialized with a renderer!" << std::endl;
         return false;
    }
    if (TilesetExists(tilesetAlias)) {
        std::cerr << "Tileset alias '" << tilesetAlias << "' already exists." << std::endl;
        return false; // Or return true and don't reload
    }

    SDL_Texture* texture = GetTexture(textureAlias);
    if (!texture) {
        std::cerr << "Texture '" << textureAlias << "' not found for Tileset '" << tilesetAlias << "'." << std::endl;
        return false;
    }

    Tileset tileset; // Create new Tileset instance
    // Modified loadTileSet (now loadDataFromJson) will take SDL_Texture*
    if (!tileset.loadDataFromJson(jsonPath, texture)) { 
        std::cerr << "Failed to load data for Tileset '" << tilesetAlias << "' from JSON '" << jsonPath << "'." << std::endl;
        return false;
    }
    
    m_tilesets[tilesetAlias] = tileset; // Store the loaded tileset
    return true;
}

Tileset* ResourceManager::GetTileset(const std::string& alias) {
    if (!TilesetExists(alias)) {
        std::cerr << "Tileset with alias '" << alias << "' not found." << std::endl;
        return nullptr;
    }
    return &m_tilesets[alias]; // Return pointer to the stored Tileset
}

bool ResourceManager::TilesetExists(const std::string& alias) {
    return m_tilesets.count(alias) > 0;
}

// bool ResourceManager::TilesetExists(const std::string& alias) { // Removed non-const
//     return m_tilesets.count(alias) > 0;
// }
