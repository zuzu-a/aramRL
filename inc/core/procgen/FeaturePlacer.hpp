#ifndef FEATURE_PLACER_HPP
#define FEATURE_PLACER_HPP

#include "MeshPrimitives.hpp"
#include "inc/core/EntityFactory.hpp" // For creating entities
#include <entt/entt.hpp>             // For entt::registry (passed via EntityFactory or directly)
#include "external/simplex_noise/SimplexNoise.h" // For randomizing placement
#include <cstdint> // For int64_t

namespace ProcGen {
    class FeaturePlacer {
    public:
        FeaturePlacer(EntityFactory& factory, int64_t seed); // Changed seed to int64_t

        // Places features based on site biomes and other rules
        void PlaceFeatures(const MeshData& meshData);

    private:
        EntityFactory& m_entityFactory;
        SimplexNoise m_noise; // For adding some variation to placement
        // int m_seed; // Seed is used by m_noise, not needed as separate member

        // Example helper:
        // bool ShouldPlaceTree(const Site& site);
        // bool ShouldPlaceRock(const Site& site);
    };
} // namespace ProcGen
#endif // FEATURE_PLACER_HPP
