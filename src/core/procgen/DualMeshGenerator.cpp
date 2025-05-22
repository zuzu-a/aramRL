#include "inc/core/procgen/DualMeshGenerator.hpp"
#include "external/delaunator-cpp/delaunator.hpp" // Adjust path as needed
#include "inc/core/EntityFactory.hpp" // For EntityFactory
#include <iostream> // For debug

namespace ProcGen {

DualMeshGenerator::DualMeshGenerator(EntityFactory& entityFactory) 
    : m_entityFactory(entityFactory) {}

bool DualMeshGenerator::GenerateDelaunay(MeshData& meshData) {
    if (meshData.sites.empty()) {
        // std::cerr << "DualMeshGenerator: No sites provided to generate Delaunay triangulation." << std::endl;
        return false;
    }

    // Convert sites to a flat vector of doubles for delaunator-cpp
    std::vector<double> coords;
    coords.reserve(meshData.sites.size() * 2);
    for (const auto& site : meshData.sites) {
        coords.push_back(static_cast<double>(site.point.x));
        coords.push_back(static_cast<double>(site.point.y));
    }

    if (coords.size() < 6) { // Need at least 3 points (6 coordinates)
        // std::cerr << "DualMeshGenerator: Not enough unique points to form a triangle." << std::endl;
        // Even if delaunator handles this, our MeshData might expect actual triangles.
        meshData.triangles.clear();
        meshData.edges.clear();
        return true; // Technically not an error, just no triangles.
    }

    try {
        delaunator::Delaunator d(coords);

        meshData.triangles.clear();
        meshData.triangles.reserve(d.triangles.size() / 3);
        for (size_t i = 0; i < d.triangles.size(); i += 3) {
            Triangle tri;
            tri.id = static_cast<int>(i / 3);
            tri.siteIds[0] = static_cast<int>(d.triangles[i]);
            tri.siteIds[1] = static_cast<int>(d.triangles[i + 1]);
            tri.siteIds[2] = static_cast<int>(d.triangles[i + 2]);
            meshData.triangles.push_back(tri);
        }

        // Edges can be derived from triangles or halfedges if needed.
        // The current delaunator.hpp placeholder does not fully populate halfedges robustly.
        // For now, we'll leave edges empty or derive them simply.
        meshData.edges.clear(); 
        // TODO: Populate edges based on triangles or d.halfedges.
        // A common way is to iterate triangles and add unique edges.
        // For example, for each triangle (s1, s2, s3):
        //   Add edge (s1,s2), (s2,s3), (s3,s1) if not already added (e.g., using a set of pairs)

    } catch (const std::exception& e) {
        // std::cerr << "DualMeshGenerator: Error during Delaunay triangulation: " << e.what() << std::endl;
        return false;
    }

    return true;
}

// bool DualMeshGenerator::GenerateVoronoi(MeshData& meshData) {
//     // Placeholder for Voronoi logic (typically uses Delaunay circumcenters)
//     std::cerr << "DualMeshGenerator: Voronoi generation not yet implemented." << std::endl;
//     return false;
// }

void DualMeshGenerator::GenerateMapFeatures(MeshData& meshData, int64_t seed) {
    if (meshData.sites.empty()) {
        // std::cerr << "DualMeshGenerator: No sites provided for map feature generation." << std::endl;
        return;
    }

    // 1. Assign Elevation
    ElevationGenerator elevationGen(seed); // Use the provided seed
    // Example islandShapeFactor, can be parameterized if needed
    // The value 2.0f here is an exponent for the island shaping function in ElevationGenerator.
    // The value 0.7f was a weight in the prompt's example of combining noise and island shape.
    // Let's use 2.0f as the exponent for a nice round island.
    elevationGen.AssignElevation(meshData, 2.0f); 

    // 2. Assign Biomes (Rainfall, Moisture, BiomeType)
    BiomeGenerator biomeGen(seed + 1); // Use a different seed or offset for variety
    biomeGen.AssignBiomes(meshData); // Uses default baseRainfall (0.5f)

    // 3. Generate Rivers
    RiverGenerator riverGen(seed + 2); // Use yet another seed
    riverGen.GenerateRivers(meshData);

    // 4. Place Features
    FeaturePlacer featurePlacer(m_entityFactory, seed + 3); // Use EntityFactory member
    featurePlacer.PlaceFeatures(meshData);

    // TODO: Further steps like smoothing, etc.
}

} // namespace ProcGen
