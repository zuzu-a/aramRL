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
    m_characterDefinitions.clear();
    m_itemDefinitions.clear();
    m_tilesetDefinitions.clear();
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

// bool ResourceManager::TilesetExists(const std::string& alias) const { // Already const
//     return m_tilesets.count(alias) > 0;
// }

// LoadTileset was the old name for loading the actual Tileset object.
// New method LoadTilesetFromDefinition handles loading based on a TilesetDefinition.
bool ResourceManager::LoadTilesetFromDefinition(const std::string& tilesetDefAlias) {
    Content::TilesetDefinition* def = GetTilesetDefinition(tilesetDefAlias);
    if (!def) {
        std::cerr << "LoadTilesetFromDefinition: TilesetDefinition '" << tilesetDefAlias << "' not found." << std::endl;
        return false;
    }

    SDL_Texture* texture = GetTexture(def->textureAlias);
    if (!texture) {
        // Attempt to load the texture if not already loaded by RM - assuming texture path is relative to some asset dir
        // For this example, we assume the textureAlias in TilesetDefinition IS the texture path for simplicity
        // Or, more robustly, TilesetDefinition could also store the texture's file path.
        // Let's assume textureAlias IS the filePath for LoadTexture for now if not found.
        std::cout << "LoadTilesetFromDefinition: Texture '" << def->textureAlias 
                  << "' not found. Attempting to load it using alias as path." << std::endl;
        texture = LoadTexture(def->textureAlias, def->textureAlias); // Use alias as path and alias
        if (!texture) {
             std::cerr << "LoadTilesetFromDefinition: Failed to load texture '" << def->textureAlias 
                       << "' for TilesetDefinition '" << tilesetDefAlias << "'." << std::endl;
            return false;
        }
    }
    
    // The alias for the *loaded Tileset object* will be the same as the *TilesetDefinition's alias*.
    if (TilesetExists(tilesetDefAlias)) {
        std::cerr << "LoadTilesetFromDefinition: Tileset object with alias '" << tilesetDefAlias << "' already exists." << std::endl;
        return true; // Already loaded, consider it a success.
    }

    Tileset tileset;
    if (!tileset.loadDataFromJson(def->jsonPath, texture)) {
        std::cerr << "LoadTilesetFromDefinition: Failed to load data for Tileset (from def '" << tilesetDefAlias 
                  << "') from JSON '" << def->jsonPath << "'." << std::endl;
        return false;
    }
    
    m_tilesets[tilesetDefAlias] = tileset; // Store the loaded tileset using the definition's alias
    std::cout << "Successfully loaded Tileset '" << tilesetDefAlias << "' from definition." << std::endl;
    return true;
}


// CharacterDefinition Implementations
bool ResourceManager::LoadCharacterDefinition(const std::string& filePath, const std::string& alias) {
    if (m_characterDefinitions.count(alias)) {
        std::cerr << "CharacterDefinition alias '" << alias << "' already exists." << std::endl;
        return false;
    }
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open CharacterDefinition file: " << filePath << std::endl;
        return false;
    }
    nlohmann::json json_data;
    try {
        file >> json_data;
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Failed to parse CharacterDefinition JSON: " << e.what() << " in file " << filePath << std::endl;
        return false;
    }

    Content::CharacterDefinition def;
    def.sourceFile = filePath;
    if (!Content::CharacterDefinition::FromJson(json_data, def)) {
        std::cerr << "Failed to process CharacterDefinition from JSON data in file " << filePath << std::endl;
        return false;
    }
    m_characterDefinitions[alias] = def;
    return true;
}

Content::CharacterDefinition* ResourceManager::GetCharacterDefinition(const std::string& alias) {
    if (m_characterDefinitions.count(alias)) {
        return &m_characterDefinitions[alias];
    }
    std::cerr << "CharacterDefinition with alias '" << alias << "' not found." << std::endl;
    return nullptr;
}

// ItemDefinition Implementations
bool ResourceManager::LoadItemDefinition(const std::string& filePath, const std::string& alias) {
    if (m_itemDefinitions.count(alias)) {
        std::cerr << "ItemDefinition alias '" << alias << "' already exists." << std::endl;
        return false;
    }
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open ItemDefinition file: " << filePath << std::endl;
        return false;
    }
    nlohmann::json json_data;
    try {
        file >> json_data;
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Failed to parse ItemDefinition JSON: " << e.what() << " in file " << filePath << std::endl;
        return false;
    }

    Content::ItemDefinition def;
    def.sourceFile = filePath;
    if (!Content::ItemDefinition::FromJson(json_data, def)) {
        std::cerr << "Failed to process ItemDefinition from JSON data in file " << filePath << std::endl;
        return false;
    }
    m_itemDefinitions[alias] = def;
    return true;
}

Content::ItemDefinition* ResourceManager::GetItemDefinition(const std::string& alias) {
    if (m_itemDefinitions.count(alias)) {
        return &m_itemDefinitions[alias];
    }
    std::cerr << "ItemDefinition with alias '" << alias << "' not found." << std::endl;
    return nullptr;
}

// TilesetDefinition Implementations
bool ResourceManager::LoadTilesetDefinition(const std::string& filePath, const std::string& alias) {
    if (m_tilesetDefinitions.count(alias)) {
        std::cerr << "TilesetDefinition alias '" << alias << "' already exists." << std::endl;
        return false;
    }
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open TilesetDefinition file: " << filePath << std::endl;
        return false;
    }
    nlohmann::json json_data;
    try {
        file >> json_data;
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Failed to parse TilesetDefinition JSON: " << e.what() << " in file " << filePath << std::endl;
        return false;
    }

    Content::TilesetDefinition def;
    def.sourceFile = filePath;
    if (!Content::TilesetDefinition::FromJson(json_data, def)) {
        std::cerr << "Failed to process TilesetDefinition from JSON data in file " << filePath << std::endl;
        return false;
    }
    m_tilesetDefinitions[alias] = def;
    return true;
}

Content::TilesetDefinition* ResourceManager::GetTilesetDefinition(const std::string& alias) {
    if (m_tilesetDefinitions.count(alias)) {
        return &m_tilesetDefinitions[alias];
    }
    std::cerr << "TilesetDefinition with alias '" << alias << "' not found." << std::endl;
    return nullptr;
}
