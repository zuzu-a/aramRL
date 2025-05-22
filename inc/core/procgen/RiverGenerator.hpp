#ifndef RIVER_GENERATOR_HPP
#define RIVER_GENERATOR_HPP

#include "MeshPrimitives.hpp"
#include <vector> // For sorting sites
#include <cstdint> // For int64_t

namespace ProcGen {
    class RiverGenerator {
    public:
        RiverGenerator(int64_t seed); // Seed for any randomness if needed

        // Assigns riverFlow and downslopeSiteId to sites
        void GenerateRivers(MeshData& meshData, float minFlowForRiver = 0.01f);

    private:
        int64_t m_seed; // Changed to int64_t to match constructor

        // Helper to find the "lowest" neighbor for a given site (simplified)
        int FindDownslopeNeighbor(const Site& currentSite, const MeshData& meshData);
    };
} // namespace ProcGen
#endif // RIVER_GENERATOR_HPP
