#include "inc/core/procgen/FeaturePlacer.hpp"
#include "inc/core/procgen/MeshPrimitives.hpp" // For Site, MeshData
#include "inc/core/content/CharacterDefinition.hpp" // For ComponentData (though not directly used in this simplified version)
#include "inc/core/components/vector2d.hpp"
#include <iostream> // For debug

namespace ProcGen {

// Seed for SimplexNoise is int64_t
FeaturePlacer::FeaturePlacer(EntityFactory& factory, int64_t seed) 
    : m_entityFactory(factory), m_noise(seed) {}

void FeaturePlacer::PlaceFeatures(const MeshData& meshData) {
    if (meshData.sites.empty()) {
        // std::cerr << "FeaturePlacer: No sites provided for feature placement." << std::endl;
        return;
    }

    // Noise parameters for placement variation
    float tree_density_noise_freq = 0.1f;
    float rock_density_noise_freq = 0.05f;
    float position_offset_scale = 5.0f; // Max units of offset
    float position_noise_freq = 10.0f;

    for (const auto& site : meshData.sites) {
        if (site.isWater) {
            continue; // Skip water sites
        }

        // Tree Placement Logic
        bool placeTree = false;
        if (site.biome == Site::BiomeType::FOREST) {
            // Higher chance in forest, e.g., 70% base chance modulated by noise
            if ((m_noise.Evaluate(site.point.x * tree_density_noise_freq, site.point.y * tree_density_noise_freq) + 1.0) * 0.5 > 0.3) { // Noise [0,1] > 0.3 (70% chance)
                placeTree = true;
            }
        } else if (site.biome == Site::BiomeType::GRASSLAND) {
            // Lower chance in grassland, e.g., 20% base chance modulated by noise
            if ((m_noise.Evaluate(site.point.x * tree_density_noise_freq + 5.0, site.point.y * tree_density_noise_freq + 5.0) + 1.0) * 0.5 > 0.8) { // Noise [0,1] > 0.8 (20% chance)
                placeTree = true;
            }
        }

        if (placeTree) {
            float offsetX = static_cast<float>((m_noise.Evaluate(site.point.x * position_noise_freq, site.point.y * position_noise_freq)) * position_offset_scale);
            float offsetY = static_cast<float>((m_noise.Evaluate(site.point.y * position_noise_freq, site.point.x * position_noise_freq)) * position_offset_scale);
            Vector2D treePosition = {site.point.x + offsetX, site.point.y + offsetY};
            m_entityFactory.CreateItem("tree_pine", treePosition); // Using CreateItem for now
            // std::cout << "Placed tree_pine at: " << treePosition.x << ", " << treePosition.y << std::endl;
        }

        // Rock Placement Logic
        bool placeRock = false;
        if (site.biome == Site::BiomeType::ROCKY) {
            // Higher chance in rocky biome, e.g., 50% base chance modulated by noise
            if ((m_noise.Evaluate(site.point.x * rock_density_noise_freq + 10.0, site.point.y * rock_density_noise_freq + 10.0) + 1.0) * 0.5 > 0.5) { // Noise [0,1] > 0.5 (50% chance)
                placeRock = true;
            }
        } else if (site.biome == Site::BiomeType::GRASSLAND) {
            // Very low chance in grassland, e.g., 10% base chance modulated by noise
             if ((m_noise.Evaluate(site.point.x * rock_density_noise_freq + 15.0, site.point.y * rock_density_noise_freq + 15.0) + 1.0) * 0.5 > 0.9) { // Noise [0,1] > 0.9 (10% chance)
                placeRock = true;
            }
        }
        
        if (placeRock) {
            float offsetX = static_cast<float>((m_noise.Evaluate(site.point.x * position_noise_freq + 20.0, site.point.y * position_noise_freq + 20.0)) * position_offset_scale);
            float offsetY = static_cast<float>((m_noise.Evaluate(site.point.y * position_noise_freq + 20.0, site.point.x * position_noise_freq + 20.0)) * position_offset_scale);
            Vector2D rockPosition = {site.point.x + offsetX, site.point.y + offsetY};
            m_entityFactory.CreateItem("rock_large", rockPosition); // Using CreateItem for now
            // std::cout << "Placed rock_large at: " << rockPosition.x << ", " << rockPosition.y << std::endl;
        }
    }
}

} // namespace ProcGen
