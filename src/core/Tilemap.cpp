#include "inc/core/Tilemap.hpp" // Adjust path as needed
#include <iostream> // For potential error messages in LoadFromFile or bounds checks

Tilemap::Tilemap() : m_width(0), m_height(0) {}

Tilemap::Tilemap(int width, int height) : m_width(width), m_height(height) {
    if (m_width > 0 && m_height > 0) {
        m_tiles.assign(m_height, std::vector<int>(m_width, 0)); // Fill with tile ID 0
    } else {
        // Handle invalid dimensions if necessary, or let it be an empty map
        m_width = 0;
        m_height = 0;
    }
}

Tilemap::~Tilemap() {}

bool Tilemap::LoadFromFile(const std::string& filePath) {
    // For now, this is a stub that initializes a default map.
    // In the future, this would parse a file (e.g., Tiled JSON map file).
    std::cout << "Tilemap::LoadFromFile called with: " << filePath << " (Using default map)" << std::endl;

    m_width = 25;  // Example: 800px / 32px tile width
    m_height = 19; // Example: 600px / 32px tile height (actually 18.75, so 19 for full coverage)
    
    m_tiles.assign(m_height, std::vector<int>(m_width, 0)); // Fill with tile ID 0 (e.g., grass)

    // Create a simple border with tile ID 1 (e.g., wall)
    for (int x = 0; x < m_width; ++x) {
        if (m_height > 0) { // Ensure height is valid before accessing
            m_tiles[0][x] = 1; // Top row
            if (m_height > 1) m_tiles[m_height - 1][x] = 1; // Bottom row
        }
    }
    for (int y = 0; y < m_height; ++y) {
         if (m_width > 0) { // Ensure width is valid before accessing
            m_tiles[y][0] = 1; // Left column
            if (m_width > 1) m_tiles[y][m_width - 1] = 1; // Right column
         }
    }
    return true;
}

int Tilemap::GetTileID(int x, int y) const {
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
        return m_tiles[y][x];
    }
    // std::cerr << "Tilemap::GetTileID: Coordinates (" << x << "," << y << ") are out of bounds." << std::endl;
    return -1; // Or some other indicator for out-of-bounds/error
}

void Tilemap::SetTileID(int x, int y, int tileID) {
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
        m_tiles[y][x] = tileID;
    } else {
        // std::cerr << "Tilemap::SetTileID: Coordinates (" << x << "," << y << ") are out of bounds." << std::endl;
    }
}

bool Tilemap::IsWalkable(int x, int y) const {
    int tileID = GetTileID(x, y);
    if (tileID == -1) return false; // Out of bounds is not walkable
    
    // Example logic: tile ID 0 is walkable, anything else is not.
    // This should be configurable or based on tile properties in a real game.
    return tileID == 0; 
}
