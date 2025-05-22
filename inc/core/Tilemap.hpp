#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <vector>
#include <string> // For file path in load method
#include "inc/core/procgen/MeshPrimitives.hpp" // Added for ProcGen::MeshData

class Tilemap {
public:
    Tilemap(); // Default constructor
    Tilemap(int width, int height); // Constructor from tiles.cpp
    ~Tilemap();

    bool LoadFromFile(const std::string& filePath); // Placeholder for loading map data
    int GetTileID(int x, int y) const;
    void SetTileID(int x, int y, int tileID);
    bool IsWalkable(int x, int y) const; // Example method
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

    // Populates the tilemap grid based on ProcGen::MeshData
    void PopulateFromMeshData(const ProcGen::MeshData& meshData, int gridWidth, int gridHeight);

private:
    int m_width = 0;
    int m_height = 0;
    std::vector<std::vector<int>> m_tiles; // tile IDs
    // Add other map properties as needed (e.g., tileset name used)
};

#endif // TILEMAP_HPP
