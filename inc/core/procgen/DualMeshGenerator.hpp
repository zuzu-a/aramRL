#ifndef DUAL_MESH_GENERATOR_HPP
#define DUAL_MESH_GENERATOR_HPP

#include "MeshPrimitives.hpp" // For MeshData, Site, Triangle, Edge
#include "ElevationGenerator.hpp"
#include "BiomeGenerator.hpp"
#include "RiverGenerator.hpp"
#include "FeaturePlacer.hpp"      // Added FeaturePlacer
#include <vector>
#include <string> // For seed if passed as string, though ElevationGenerator takes int

// Forward declare EntityFactory
class EntityFactory;

namespace ProcGen {

class DualMeshGenerator {
public:
    DualMeshGenerator(EntityFactory& entityFactory); // Modified constructor

    // Populates triangles and edges in MeshData using Delaunay triangulation
    bool GenerateDelaunay(MeshData& meshData); 

    // Generates elevation and other map features
    void GenerateMapFeatures(MeshData& meshData, int64_t seed);


    // Placeholder for future Voronoi generation
    // bool GenerateVoronoi(MeshData& meshData);

private:
    EntityFactory& m_entityFactory; // Store reference to EntityFactory
    // Helper methods for Delaunay or Voronoi if needed
};

} // namespace ProcGen

#endif // DUAL_MESH_GENERATOR_HPP
