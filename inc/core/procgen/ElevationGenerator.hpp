#ifndef ELEVATION_GENERATOR_HPP
#define ELEVATION_GENERATOR_HPP

#include "MeshPrimitives.hpp" // For ProcGen::MeshData, ProcGen::Site
#include "external/simplex_noise/SimplexNoise.h" // For SimplexNoise
#include <string> // For seed (though SimplexNoise takes int64_t)
#include <vector> // For potential future use with multiple noise layers

namespace ProcGen {
    class ElevationGenerator {
    public:
        ElevationGenerator(int64_t seed); // SimplexNoise takes int64_t

        // Assigns elevation to each site in MeshData
        // Modifies site.elevation (new field to be added to Site struct)
        // and site.isWater (new field to be added to Site struct)
        void AssignElevation(MeshData& meshData, float islandShapeFactor = 0.7f, float waterLevel = 0.0f);

    private:
        SimplexNoise m_noise; 

        // Helper function for island shape (e.g., radial gradient)
        // nx, ny should be normalized to [-0.5, 0.5] where (0,0) is center
        float GetIslandShapeModifier(float nx_centered, float ny_centered, float islandShapeFactor);
    };
} // namespace ProcGen
#endif // ELEVATION_GENERATOR_HPP
