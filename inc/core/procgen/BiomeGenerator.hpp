#ifndef BIOME_GENERATOR_HPP
#define BIOME_GENERATOR_HPP

#include "MeshPrimitives.hpp"
#include "external/simplex_noise/SimplexNoise.h" // Or your noise library path

namespace ProcGen {
    class BiomeGenerator {
    public:
        BiomeGenerator(int64_t seed); // Changed seed type to int64_t for SimplexNoise

        // Assigns rainfall, moisture, and biome to each site
        void AssignBiomes(MeshData& meshData, float baseRainfall = 0.5f, float windAngleDeg = 0.0f);

    private:
        SimplexNoise m_noise; 
        // int m_seed; // Seed is used by m_noise, not needed as separate member typically

        // Helper to determine biome based on elevation, moisture, etc.
        Site::BiomeType DetermineBiome(float elevation, float moisture, bool isWater);
    };
} // namespace ProcGen
#endif // BIOME_GENERATOR_HPP
