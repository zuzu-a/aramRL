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

void Tilemap::PopulateFromMeshData(const ProcGen::MeshData& meshData, int gridWidth, int gridHeight) {
    if (gridWidth <= 0 || gridHeight <= 0) {
        std::cerr << "Tilemap::PopulateFromMeshData: Invalid grid dimensions." << std::endl;
        return;
    }

    m_width = gridWidth;
    m_height = gridHeight;
    m_tiles.assign(m_height, std::vector<int>(m_width, 3)); // Default to GRASSLAND (Tile ID 3)

    if (meshData.sites.empty() || meshData.bounds.GetWidth() <= 0 || meshData.bounds.GetHeight() <= 0) {
        // std::cerr << "Tilemap::PopulateFromMeshData: No sites or invalid meshData.bounds to populate from." << std::endl;
        return; // Keep default filled map
    }

    for (const auto& site : meshData.sites) {
        // Normalize site position to [0, 1] within meshData.bounds
        float normX = (site.point.x - meshData.bounds.min.x) / meshData.bounds.GetWidth();
        float normY = (site.point.y - meshData.bounds.min.y) / meshData.bounds.GetHeight();

        // Map normalized position to grid coordinates
        int gx = static_cast<int>(normX * gridWidth);
        int gy = static_cast<int>(normY * gridHeight);

        // Clamp coordinates to be within grid bounds
        gx = std::max(0, std::min(gx, gridWidth - 1));
        gy = std::max(0, std::min(gy, gridHeight - 1));

        int tileID = 3; // Default to GRASSLAND
        switch (site.biome) {
            case ProcGen::Site::BiomeType::DEEP_WATER:    tileID = 0; break;
            case ProcGen::Site::BiomeType::SHALLOW_WATER: tileID = 1; break;
            case ProcGen::Site::BiomeType::BEACH:         tileID = 2; break;
            case ProcGen::Site::BiomeType::GRASSLAND:     tileID = 3; break;
            case ProcGen::Site::BiomeType::FOREST:        tileID = 4; break; // Base forest tile
            case ProcGen::Site::BiomeType::ROCKY:         tileID = 5; break;
            case ProcGen::Site::BiomeType::SNOW:          tileID = 6; break;
            case ProcGen::Site::BiomeType::UNKNOWN:       // Fallthrough to default
            default:                                      tileID = 3; break; 
        }
        m_tiles[gy][gx] = tileID;
    }
}
