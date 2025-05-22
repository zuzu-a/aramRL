#include "inc/core/procgen/ElevationGenerator.hpp"
#include "inc/core/procgen/MeshPrimitives.hpp" // For Site, MeshData
#include <cmath>     // For std::sqrt, std::pow, std::max, std::min
#include <iostream>  // For debug (optional)

namespace ProcGen {

// Constructor: Initialize noise generator with the seed.
ElevationGenerator::ElevationGenerator(int64_t seed) : m_noise(seed) {}

// Helper function for island shape (e.g., radial gradient)
// nx_centered, ny_centered should be normalized to [-0.5, 0.5] where (0,0) is center
float ElevationGenerator::GetIslandShapeModifier(float nx_centered, float ny_centered, float islandShapeFactor) {
    // This version expects nx_centered, ny_centered to be in range [-0.5, 0.5]
    // The islandShapeFactor controls how strongly the island shape pulls down edges.
    // A higher factor means a more pronounced island with steeper drop-offs.
    
    float dist_sq = nx_centered * nx_centered + ny_centered * ny_centered;
    // Max squared distance from center for a point in [-0.5, 0.5] box is 0.5*0.5 + 0.5*0.5 = 0.5
    // We want the gradient to fall off towards the "corners" of this normalized square.
    // Let's define an effective radius for the island, e.g., 0.5.
    float effective_radius_sq = 0.5f * 0.5f; // Points outside this start to fall off sharply

    // A simple squared falloff from the center
    // float falloff = 1.0f - (dist_sq / effective_radius_sq); 
    // return std::max(0.0f, falloff) * islandShapeFactor;

    // Alternative: radial gradient based on distance, shaped by islandShapeFactor
    // islandShapeFactor = 1.0 (strong island) to e.g. 10.0 (very gentle island, almost no effect)
    // The prompt example: float gradient = 1.0f - (dist / maxDist);
    // Here, dist is sqrt(dist_sq). maxDist is effectively 0.5 for normalized coords.
    float dist = std::sqrt(dist_sq);
    float max_dist_from_center = 0.5f; // Given nx, ny are in [-0.5, 0.5]

    // The gradient calculation from the prompt example might be too simple.
    // Let's use a common technique: d = distance from center (0 to 1, where 1 is edge of map)
    // elevation = (1 - d^2) * (1 - d^2) ; a bit like a squared exponential falloff
    // To make it fit [-0.5, 0.5] range, d = dist / max_dist_from_center;
    float d_norm = dist / max_dist_from_center; // Normalized distance, 0 at center, 1 at edge of square [-0.5,0.5] box
    
    // A common shaping function: x^a / (x^a + (1-x)^a)
    // Or simpler: (1 - d_norm) is a linear gradient.
    // Let's use a power curve to shape the island, influenced by islandShapeFactor
    // islandShapeFactor: small values (e.g., 0.1-0.5) = gentle slope, large values (e.g. 1-4) = steep dropoff
    // The prompt's example: return std::max(0.0f, std::min(1.0f, gradient)) * islandShapeFactor;
    // This implies islandShapeFactor itself is the resulting elevation modifier.
    // Let's make islandShapeFactor more like a "strength" of the island shape.
    // A value of 1.0 means the radial gradient fully influences the shape.
    // The prompt's islandShapeFactor = 0.5f seems to be a weight.

    // A common method: create a value that is 1 at the center and 0 at distance `radius`.
    // float value = 1.0f - std::pow(dist / max_dist_from_center, islandShapeFactor);
    // The prompt's example: float gradient = 1.0f - (dist / maxDist);
    // return std::max(0.0f, std::min(1.0f, gradient)) * islandShapeFactor; (this is a bit odd if islandShapeFactor is a weight)

    // Let's use the formula from a known good source for island generation:
    // nx, ny here are assumed to be in range [-1, 1] for this formula usually.
    // Since ours are [-0.5, 0.5], we can scale them: nx_scaled = nx_centered * 2.0;
    float nx_scaled = nx_centered * 2.0f; // Now in [-1, 1]
    float ny_scaled = ny_centered * 2.0f; // Now in [-1, 1]
    
    // Polynomial shaping function. islandShapeFactor here could be an exponent.
    // Example: 1.0 - ( ( (x*x + y*y) / radius^2 ) ^ exponent )
    // For simplicity as per prompt example, let's adapt the linear falloff
    // but ensure islandShapeFactor is a multiplier for the gradient value.
    // The prompt's example: float gradient = 1.0f - (dist / maxDist);
    // return std::max(0.0f, std::min(1.0f, gradient)) * islandShapeFactor;
    // This makes islandShapeFactor act as a direct scaling of the gradient.
    // If islandShapeFactor is 0.5, max modifier is 0.5.

    // Let's try to make the modifier range from 0 at edge to 1 at center, then scale by islandShapeFactor.
    float gradient = 1.0f - (dist / max_dist_from_center); // dist/max_dist_from_center is 0 at center, 1 at edge
                                                            // so gradient is 1 at center, 0 at edge
    float shaped_gradient = std::pow(std::max(0.0f, gradient), islandShapeFactor); // islandShapeFactor acts as exponent here
                                                                                // if islandShapeFactor=1, linear.
                                                                                // if islandShapeFactor=2, quadratic (rounder island)
                                                                                // if islandShapeFactor=0.5, more plateau-like
    
    // The prompt's example: `finalElevation = noise_value * 0.5f + GetIslandShapeModifier(...) * 0.7f;`
    // suggests GetIslandShapeModifier should return a value that's scaled by 0.7.
    // Let's make it so that islandShapeFactor directly controls the "height" of the island center.
    // And the shape is somewhat fixed (e.g. quadratic falloff)
    // d = distance from center, normalized to map radius (0 to 1)
    // island_mask = (1 - d*d); // quadratic falloff
    // return island_mask * islandShapeFactor; // islandShapeFactor is max height of this mask

    // Let's stick to the prompt's example structure:
    // float gradient = 1.0f - (dist / maxDist); // maxDist is 0.5 for nx_centered in [-0.5, 0.5]
    // return std::max(0.0f, std::min(1.0f, gradient)) * islandShapeFactor;
    // This means islandShapeFactor is a scaling factor for the 0-1 gradient.
    // For islandShapeFactor = 0.7, the max modifier is 0.7.
    
    // Let's try a slightly more common approach for island shaping:
    // Create a value that is 1.0 at the center and falls off to 0.0 at the "edge"
    // An "edge" can be defined as dist_from_center_normalized = 1.0
    float d = dist / max_dist_from_center; // d is 0 at center, 1 at edge (max_dist_from_center)
    
    // Using a power curve to control the slope. islandShapeFactor could be the exponent.
    // Let's assume islandShapeFactor from the prompt (0.5f) is a weight, not an exponent.
    // For a simple radial gradient that is 1 at center and 0 at edge:
    float radial_gradient = 1.0f - d;
    radial_gradient = std::max(0.0f, std::min(1.0f, radial_gradient)); // Clamp to [0,1]
    
    // A common island shaping function: (1 - d^2)^power, let power be islandShapeFactor
    // Or simply: (1.0 - d) scaled by islandShapeFactor.
    // The prompt's calculation `finalElevation = noise_value * 0.5f + GetIslandShapeModifier(...) * 0.7f;`
    // suggests GetIslandShapeModifier result is weighted by 0.7.
    // If islandShapeFactor is 0.5 as in prompt, then 0.5 * 0.7 = 0.35 max contribution.
    // Let's make this function return a value between 0 and 1, representing the island mask.
    // The islandShapeFactor from the prompt (0.7f) is the weight in the final sum.
    // Let's rename the parameter in this function to 'exponent' or similar for clarity.
    // For now, let's use a simple quadratic falloff for the mask.
    // islandShapeFactor in AssignElevation will be the weight of this mask.
    float falloff_exponent = 2.0f; // Controls how steep the island drop-off is
    float island_mask = 1.0f - std::pow(d, falloff_exponent);
    return std::max(0.0f, island_mask);
}


// Assigns elevation to each site in MeshData
void ElevationGenerator::AssignElevation(MeshData& meshData, float islandShapeFactor) {
    if (meshData.sites.empty()) return;

    float waterLevel = 0.0f; // Example water level if elevation is centered around 0

    for (auto& site : meshData.sites) {
        // Normalize site coordinates to [0, 1]
        float nx = (site.point.x - meshData.bounds.min.x) / meshData.bounds.GetWidth();
        float ny = (site.point.y - meshData.bounds.min.y) / meshData.bounds.GetHeight();

        // Calculate base elevation using multiple octaves of noise (fBm)
        float baseElevation = 0.0f;
        float amplitude = 1.0f;
        float frequency = 1.0f; // Base frequency for noise
        int octaves = 4;        // Number of noise layers
        float persistence = 0.5f; // How much detail each octave adds
        float lacunarity = 2.0f;  // How much frequency increases per octave

        for (int i = 0; i < octaves; ++i) {
            // Scale coordinates for noise input - can be adjusted for different "zoom" levels of noise
            double noise_x = static_cast<double>(nx * frequency);
            double noise_y = static_cast<double>(ny * frequency);
            baseElevation += static_cast<float>(amplitude * m_noise.Evaluate(noise_x, noise_y));
            amplitude *= persistence;
            frequency *= lacunarity;
        }
        // Normalize baseElevation: SimplexNoise output is roughly [-1, 1].
        // Sum of amplitudes: 1 + 0.5 + 0.25 + 0.125 = 1.875. So divide by this.
        // This makes baseElevation roughly in [-1, 1] range.
        float sum_amplitudes = 0.0f;
        float temp_amp = 1.0f;
        for(int i=0; i<octaves; ++i) { sum_amplitudes += temp_amp; temp_amp *= persistence; }
        if (sum_amplitudes > 0.0001f) { // Avoid division by zero
            baseElevation /= sum_amplitudes;
        }


        // Apply the island shape modifier
        // Convert nx, ny to be centered at (0,0) for GetIslandShapeModifier: range [-0.5, 0.5]
        float nx_centered = nx - 0.5f;
        float ny_centered = ny - 0.5f;
        float islandModifier = GetIslandShapeModifier(nx_centered, ny_centered, 2.0f); // 2.0f for quadratic falloff strength

        // Combine noise and island shape.
        // The prompt example: finalElevation = noise_value * 0.5f + GetIslandShapeModifier(...) * 0.7f;
        // Let's try: elevation = (noise + island_mask) / 2.0; but ensure island_mask is scaled.
        // A common way: e = (e + 1.0) * 0.5; // map noise from [-1,1] to [0,1]
        // Then multiply by island_mask.
        // Or, more simply, make the island shape primary and noise adds detail.
        // elevation = island_mask * ( (noise_value_0_to_1 * noise_influence) + (1.0 - noise_influence) )
        // For now, let's use a weighted sum and then scale.
        // If islandShapeFactor is the weight of the island mask:
        
        // Let's adjust baseElevation to be mostly positive (e.g., 0 to 1)
        float noise_0_1 = (baseElevation + 1.0f) * 0.5f; // map [-1,1] to [0,1]
        
        // finalElevation combines the island shape (strongest effect) with noise (detail)
        // islandModifier is [0,1] (0 at edge, 1 at center)
        // noise_0_1 is [0,1]
        // We want land in the center, water at edges.
        // finalElevation = islandModifier; // Simplest island
        // Add noise to this:
        // finalElevation = islandModifier * (0.5f + noise_0_1 * 0.5f); // Noise modulates island height
        // Or, make the island shape more like a "mask" or "bias"
        float finalElevation = noise_0_1 * (1.0f - islandShapeFactor) + islandModifier * islandShapeFactor;
        // This makes islandShapeFactor control the blend. If 1.0, pure island. If 0.0, pure noise.
        // The prompt has islandShapeFactor = 0.7f for the example in Engine.cpp.
        // Let's use the prompt's logic:
        // finalElevation = noise_value * 0.5f + GetIslandShapeModifier(...) * 0.7f;
        // Here, GetIslandShapeModifier needs to return the *value* of the modifier, not a mask.
        // And noise_value is baseElevation which is [-1, 1].
        // So, if islandShapeFactor from prompt is 0.7 (weight of island shape):
        // finalElevation = baseElevation * (1.0f - 0.7f) + islandModifier * 0.7f;
        // where islandModifier is the [0,1] mask.
        // This makes sense.
        
        // Re-evaluating the prompt's islandShapeFactor usage:
        // `void AssignElevation(MeshData& meshData, float islandShapeFactor = 0.5f);`
        // `finalElevation = noise_value * 0.5f + GetIslandShapeModifier(...) * 0.7f;`
        // The 0.5f and 0.7f are weights. The islandShapeFactor passed to AssignElevation
        // is used by GetIslandShapeModifier. Let's rename it there.
        // In GetIslandShapeModifier, the parameter `islandShapeFactor` from the HPP is the exponent for pow.
        // Let's rename the parameter in `AssignElevation` to `islandWeight` to avoid confusion.
        // And make the `GetIslandShapeModifier` take an exponent.
        // For now, let's use fixed weights as in the prompt example.
        // The `islandShapeFactor` parameter to `AssignElevation` will be used as the exponent for the island shape.
        // And fixed weights for combining noise and island shape.
        
        // Recalculate islandModifier with the passed islandShapeFactor as exponent
        islandModifier = GetIslandShapeModifier(nx_centered, ny_centered, islandShapeFactor);


        // finalElevation = baseElevation * 0.3f + islandModifier * 0.7f; // Weighted sum. baseElevation is [-1,1], islandModifier is [0,1]
                                                                    // This could result in range like [-0.3, 0.3] + [0, 0.7] = [-0.3, 1.0]
        // Let's try to make elevation mostly positive for land, negative for water.
        // Ensure noise is [-1, 1]. Ensure islandModifier is [0, 1] (max at center).
        // final_elevation = noise_value * (1 - island_mask_strength) + (island_mask * island_mask_strength)
        // To make it simpler and more like common examples:
        // elevation = (noise + 1.0) * 0.5; // map noise to 0..1
        // elevation = elevation * island_mask; // apply island mask
        // elevation = elevation * 2.0 - 1.0; // map back to -1..1 if desired, for water level at 0
        
        float e = (baseElevation + 1.0f) * 0.5f; // Normalize noise to [0, 1]
        finalElevation = e * islandModifier;     // Modulate by island shape
        
        // Redistribute elevation to create more defined land/water (optional)
        // For example, power curve: finalElevation = std::pow(finalElevation, 1.5f);

        // Scale to desired range, e.g., -1 to 1, where 0 is water level
        finalElevation = finalElevation * 2.0f - 1.0f;


        site.elevation = finalElevation;
        site.isWater = (finalElevation < waterLevel); 
    }
}


} // namespace ProcGen
