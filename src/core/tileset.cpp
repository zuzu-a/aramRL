#include "inc/core/Tileset.hpp" // Adjust path as needed
#include <SDL_image.h>
#include <fstream>
#include <iostream> // For std::cerr

Tileset::Tileset() : 
    m_texture(nullptr), 
    m_tileWidth(0), 
    m_tileHeight(0), 
    m_rows(0), 
    m_columns(0), 
    m_tileCount(0) 
{}

Tileset::~Tileset() {
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
}

bool Tileset::loadTileSet(SDL_Renderer* renderer, const std::string& jsonFilePath, const std::string& texturePath) {
    // Clear previous data if any
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
    m_tileWidth = 0;
    m_tileHeight = 0;
    m_rows = 0;
    m_columns = 0;
    m_tileCount = 0;
    m_name = "";
    m_tileOffset = {0,0,0,0};

    // Load JSON
    std::ifstream json_file(jsonFilePath);
    if (!json_file.is_open()) {
        std::cerr << "Failed to open JSON file: " << jsonFilePath << std::endl;
        return false;
    }
    nlohmann::json tilesheet_data;
    try {
        json_file >> tilesheet_data;
    } catch (nlohmann::json::parse_error& e) {
        std::cerr << "Failed to parse JSON: " << e.what() << " in file " << jsonFilePath << std::endl;
        return false;
    }

    // Load texture
    SDL_Surface* surface = IMG_Load(texturePath.c_str());
    if (!surface) {
        std::cerr << "Failed to load texture: " << IMG_GetError() << " for file " << texturePath << std::endl;
        return false;
    }
    m_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!m_texture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        return false;
    }

    // Populate Tileset members from JSON
    m_name = tilesheet_data.value("name", "Unknown Tileset");
    m_tileWidth = tilesheet_data.value("tilewidth", 0);
    m_tileHeight = tilesheet_data.value("tileheight", 0);
    m_columns = tilesheet_data.value("columns", 0);
    m_tileCount = tilesheet_data.value("tilecount", 0);

    if (m_columns > 0 && m_tileCount > 0) {
        m_rows = (m_tileCount + m_columns - 1) / m_columns; // Calculate rows, rounding up
    } else if (tilesheet_data.contains("imageheight") && m_tileHeight > 0) {
         m_rows = tilesheet_data.value("imageheight", 0) / m_tileHeight;
         if (m_columns == 0 && tilesheet_data.contains("imagewidth") && m_tileWidth > 0) {
             m_columns = tilesheet_data.value("imagewidth", 0) / m_tileWidth;
         }
         if (m_tileCount == 0 && m_rows > 0 && m_columns > 0) {
            m_tileCount = m_rows * m_columns;
         }
    } else {
         std::cerr << "Warning: Could not determine rows for tileset " << m_name << ". Check JSON data for tilecount/columns or imageheight/tileheight." << std::endl;
    }


    if (tilesheet_data.contains("tileoffset")) {
        auto offset_data = tilesheet_data["tileoffset"];
        m_tileOffset.x = offset_data.value("x", 0);
        m_tileOffset.y = offset_data.value("y", 0);
    }
    
    // Example: Parsing individual tile properties (if your JSON has a "tiles" array)
    // if (tilesheet_data.contains("tiles") && tilesheet_data["tiles"].is_array()) {
    //     m_tileData.resize(m_tileCount); // Or based on actual size of "tiles" array
    //     for (const auto& tile_json : tilesheet_data["tiles"]) {
    //         TypeSize id = tile_json.value("id", TypeSize(-1));
    //         if (id < m_tileCount) {
    //             // Store properties, type, etc.
    //             // m_tileData[id] = tile_json; 
    //         }
    //     }
    // }

    return true;
}

Tileset::TypeTile Tileset::getTileSourceRect(TypeSize tileID) const {
    if (!isValidTileID(tileID) || m_columns == 0) { // Added m_columns check
        // Return a default/error rect or handle error appropriately
        std::cerr << "Warning: Invalid tileID " << tileID << " or columns is 0 in getTileSourceRect for tileset " << m_name << std::endl;
        return {0, 0, 0, 0}; 
    }
    TypeSize row = tileID / m_columns;
    TypeSize col = tileID % m_columns;
    return {
        m_tileOffset.x + static_cast<int>(col * m_tileWidth),
        m_tileOffset.y + static_cast<int>(row * m_tileHeight),
        m_tileWidth,
        m_tileHeight
    };
}
