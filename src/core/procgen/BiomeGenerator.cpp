#include "inc/core/procgen/BiomeGenerator.hpp"
#include "inc/core/procgen/MeshPrimitives.hpp" // For Site, MeshData
#include <cmath>     // For std::abs, std::pow
#include <iostream>  // For debug (optional)
#include <algorithm> // For std::clamp (C++17) or manual clamping

namespace ProcGen {

// Constructor: Initialize noise generator with the seed.
// Seed type for SimplexNoise is int64_t.
BiomeGenerator::BiomeGenerator(int64_t seed) : m_noise(seed) {}


Site::BiomeType BiomeGenerator::DetermineBiome(float elevation, float moisture, bool isWater) {
    if (isWater) {
        return (elevation < -0.4f) ? Site::BiomeType::DEEP_WATER : Site::BiomeType::SHALLOW_WATER; // Adjusted deep water threshold
    } else { // Land
        if (elevation > 0.75f) { // High mountains
            return (moisture < 0.2f) ? Site::BiomeType::ROCKY : Site::BiomeType::SNOW; // Dry high = rocky, wet high = snow
        }
        if (elevation > 0.5f) { // Mountains / Hills
             if (moisture < 0.25f) return Site::BiomeType::ROCKY;
             if (moisture > 0.5f) return Site::BiomeType::FOREST; // Forested hills
             return Site::BiomeType::GRASSLAND; // Grassy hills
        }
        // Lower elevations
        if (moisture < 0.15f) return Site::BiomeType::BEACH; // Very dry low areas near water (or deserts if not near water)
                                                            // True BEACH biome often needs proximity to water.
                                                            // For now, this is a simplification.
        if (moisture > 0.6f) return Site::BiomeType::FOREST;
        if (moisture > 0.25f) return Site::BiomeType::GRASSLAND;
        
        return Site::BiomeType::GRASSLAND; // Default land biome
    }
}

// Assigns rainfall, moisture, and biome to each site
void BiomeGenerator::AssignBiomes(MeshData& meshData, float baseRainfall, float windAngleDeg) {
    if (meshData.sites.empty()) return;

    // --- 1. Assign Rainfall ---
    // Simple model: baseRainfall + noise.
    // More complex would involve wind direction, rain shadows from mountains.
    // For now, noise adds variation.
    float rainfall_noise_freq = 2.5f; // Controls "frequency" of rainfall noise patterns
    float rainfall_noise_strength = 0.4f; // How much noise affects baseRainfall (range [0,1])

    for (auto& site : meshData.sites) {
        if (site.isWater) {
            site.rainfall = baseRainfall; // Or higher for water bodies, e.g., 1.0f
            continue;
        }
        // Normalize site coordinates for consistent noise input, e.g. to [0,1] or larger for tiling
        // Assuming site.point.x/y are in world coordinates.
        // Scale them to make noise features appear at a reasonable size on the map.
        // The scale factor depends on average map size and desired feature size.
        float norm_x = site.point.x / (meshData.bounds.GetWidth() * 0.1f); // Example scaling
        float norm_y = site.point.y / (meshData.bounds.GetHeight() * 0.1f);

        double noise_val = m_noise.Evaluate(norm_x * rainfall_noise_freq, norm_y * rainfall_noise_freq); // Output is roughly [-1, 1]
        
        float rainfall_mod = static_cast<float>(noise_val * rainfall_noise_strength); // e.g. noise in [-0.4, 0.4]
        site.rainfall = baseRainfall + rainfall_mod;

        // Clamp rainfall to [0, 1]
        site.rainfall = std::max(0.0f, std::min(1.0f, site.rainfall));
    }

    // --- 2. Assign Moisture ---
    // Simple model: water bodies are fully moist. Land moisture based on rainfall.
    // More complex: proximity to water, rivers, elevation affecting evaporation.
    float moisture_noise_freq = 3.0f;
    float moisture_noise_strength = 0.2f;

    for (auto& site : meshData.sites) {
        if (site.isWater) {
            site.moisture = 1.0f;
        } else {
            // Base moisture from rainfall
            site.moisture = site.rainfall;
            
            // Add some noise for variation on land
            float norm_x = site.point.x / (meshData.bounds.GetWidth() * 0.1f);
            float norm_y = site.point.y / (meshData.bounds.GetHeight() * 0.1f);
            double noise_val = m_noise.Evaluate(norm_x * moisture_noise_freq + 5.0, norm_y * moisture_noise_freq + 5.0); // Offset noise
            float moisture_mod = static_cast<float>(noise_val * moisture_noise_strength);
            site.moisture += moisture_mod;

            // Clamp moisture to [0, 1]
            site.moisture = std::max(0.0f, std::min(1.0f, site.moisture));
        }
    }
    
    // TODO: A second pass for moisture spreading from water bodies to adjacent land sites
    // would be good here if neighbor data was available in MeshData.sites.
    // For now, this is skipped.

    // --- 3. Determine Biome Type ---
    for (auto& site : meshData.sites) {
        site.biome = DetermineBiome(site.elevation, site.moisture, site.isWater);
    }
}

} // namespace ProcGen
