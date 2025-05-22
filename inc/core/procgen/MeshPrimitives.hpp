#ifndef MESH_PRIMITIVES_HPP
#define MESH_PRIMITIVES_HPP

#include "inc/core/components/vector2d.hpp" // For Vector2D
#include "inc/core/procgen/PointGenerator.hpp" // For AABB (move AABB here if preferred)
#include <vector>
#include <array> // For std::array

namespace ProcGen {

    // Represents a point site for Voronoi regions / Delaunay vertices
    struct Site {
        int id;
        Vector2D point;
        bool isBoundary = false;
        float elevation = 0.0f; // e.g., -1.0 (deep water) to 1.0 (peak)
        bool isWater = false;
        float rainfall = 0.0f;    // e.g., 0.0 (dry) to 1.0 (wet)
        float moisture = 0.0f;   // Derived from rainfall and proximity to water
        enum class BiomeType { 
            DEEP_WATER, SHALLOW_WATER, BEACH, GRASSLAND, FOREST, ROCKY, SNOW, // Add more as needed
            UNKNOWN 
        };
        BiomeType biome = BiomeType::UNKNOWN;
        float riverFlow = 0.0f; // Accumulated flow. >0 indicates a river/stream.
        int downslopeSiteId = -1; // ID of the site this one flows to, -1 if none or water body
        // Add other site-specific data later (e.g., isMountainPeakSeed)
    };

    // Represents a triangle in the Delaunay triangulation
    struct Triangle {
        int id;
        std::array<int, 3> siteIds; // Indices into the Sites vector
        // std::array<int, 3> adjacentTriangleIds; // -1 if no adjacent
        // Vector2D circumcenter; // Will be calculated later
    };

    // Represents an edge in the mesh (connecting two sites, and part of two triangles)
    struct Edge {
        int id;
        int siteIdA, siteIdB;
        // int triangleIdA, triangleIdB; // One side is part of two triangles
    };
    
    // A simplified representation of a Voronoi region/cell
    struct Region {
        int id; // Corresponds to a Site ID
        Vector2D centroid; // The original site point
        std::vector<Vector2D> vertices; // Polygon vertices of the Voronoi cell (derived from triangle circumcenters later)
        std::vector<int> neighborRegionIds;
    };

    // Main Mesh Data structure (simplified for now)
    struct MeshData {
        std::vector<Site> sites;
        std::vector<Triangle> triangles; // Will be empty if Delaunay is not run
        std::vector<Edge> edges;         // Will be empty if Delaunay is not run
        std::vector<Region> regions;       // Initially, one region per site, vertices empty
        AABB bounds; // AABB is defined in PointGenerator.hpp for now
        std::vector<std::pair<int, int>> riverSegments; // Optional: stores river segments {fromSiteId, toSiteId}
    };

} // namespace ProcGen
#endif // MESH_PRIMITIVES_HPP
