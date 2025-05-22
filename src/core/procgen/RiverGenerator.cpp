#include "inc/core/procgen/RiverGenerator.hpp"
#include "inc/core/procgen/MeshPrimitives.hpp" // For Site, MeshData
#include <algorithm> // For std::sort, std::min_element
#include <vector>    // For std::vector
#include <cmath>     // For std::sqrt, std::abs
#include <limits>    // For std::numeric_limits<float>::max()
#include <iostream>  // For debug (optional)

namespace ProcGen {

// Constructor: Store seed.
RiverGenerator::RiverGenerator(int64_t seed) : m_seed(seed) {
    // Seed is stored but not directly used in this simplified version
    // unless randomness is introduced in FindDownslopeNeighbor tie-breaking, etc.
}

// Helper to find the "lowest" neighbor for a given site (simplified)
int RiverGenerator::FindDownslopeNeighbor(const Site& currentSite, const MeshData& meshData) {
    int bestNeighborId = -1;
    float steepestDrop = 0.0f; // We want the largest positive elevation drop

    // Define a reasonable proximity for "neighbors"
    // This is a simplification. A real mesh would give direct neighbors.
    // Let's use an arbitrary fixed radius for now, or scale with map size.
    // Average spacing between sites could be estimated if points are from Poisson Disk.
    // For now, a heuristic:
    float max_neighbor_dist_sq = std::pow(std::min(meshData.bounds.GetWidth(), meshData.bounds.GetHeight()) * 0.15f, 2); 
                                // e.g., 15% of smallest map dimension

    for (const auto& otherSite : meshData.sites) {
        if (otherSite.id == currentSite.id) continue; // Skip self

        float distSq = currentSite.point.DistanceSq(otherSite.point); // Assuming Vector2D has DistanceSq

        if (distSq < max_neighbor_dist_sq) { // Check if within proximity
            if (otherSite.elevation < currentSite.elevation) {
                float drop = currentSite.elevation - otherSite.elevation;
                if (drop > steepestDrop) {
                    steepestDrop = drop;
                    bestNeighborId = otherSite.id;
                }
            }
        }
    }
    return bestNeighborId;
}


// Assigns riverFlow and downslopeSiteId to sites
void RiverGenerator::GenerateRivers(MeshData& meshData, float minFlowForRiver) {
    if (meshData.sites.empty()) return;

    // 1. Initialization
    for (auto& site : meshData.sites) {
        site.downslopeSiteId = -1; // Initialize
        if (!site.isWater && site.moisture > 0.3f && site.elevation > 0.0f) { // Only land sites above water level with some moisture
            // Initial flow can be proportional to moisture or a base value for "springs"
            site.riverFlow = site.moisture * 0.1f; // Example: 10% of moisture contributes as initial flow
                                                 // Or a small constant: site.riverFlow = 0.005f;
        } else {
            site.riverFlow = 0.0f;
        }
    }

    // 2. Sort Sites by Elevation (Descending)
    std::vector<int> siteIndices(meshData.sites.size());
    for (int i = 0; i < siteIndices.size(); ++i) {
        siteIndices[i] = i; // Store original indices
    }

    std::sort(siteIndices.begin(), siteIndices.end(), [&](int a_idx, int b_idx) {
        return meshData.sites[a_idx].elevation > meshData.sites[b_idx].elevation;
    });

    // 3. Flow Accumulation
    for (int siteIdx : siteIndices) {
        Site& currentSite = meshData.sites[siteIdx];

        // Skip if it's a water body (rivers flow into water, not out of it in this simple model)
        // or if it has no flow to contribute (unless it's a spring point that could start a river)
        if (currentSite.isWater || currentSite.riverFlow < 1e-5f) { // Use epsilon for float comparison
            continue; 
        }

        int downslopeId = FindDownslopeNeighbor(currentSite, meshData);
        currentSite.downslopeSiteId = downslopeId;

        if (downslopeId != -1) {
            Site& downslopeSite = meshData.sites[downslopeId]; // Assuming site.id is the index
            // Rivers should not flow "uphill" into a water body that's higher,
            // but FindDownslopeNeighbor should already handle this by picking a lower elevation.
            // If the downslope site is water, flow terminates there.
            // If it's land, accumulate flow.
            if (!downslopeSite.isWater) {
                 downslopeSite.riverFlow += currentSite.riverFlow;
            }
            // (Optional) If storing river segments and current site meets minFlowForRiver
            if (currentSite.riverFlow >= minFlowForRiver) {
                 meshData.riverSegments.push_back({currentSite.id, downslopeId});
            }
        }
    }
    
    // 4. (Optional) Second pass to update biome for river sites
    // This could change a GRASSLAND site with enough riverFlow to something like RIVER_BANK or similar.
    // For now, we'll just use the riverFlow value for rendering decisions later.
    // And potentially update moisture for sites along rivers.
    for (auto& site : meshData.sites) {
        if (!site.isWater && site.riverFlow >= minFlowForRiver) {
            // Example: increase moisture for river sites, which might affect biome in a second biome pass
            site.moisture = std::min(1.0f, site.moisture + site.riverFlow * 2.0f); // Arbitrary moisture increase
            // Re-determine biome? Or have specific RIVER biome?
            // For now, let's assume rendering will check riverFlow.
        }
    }
}

} // namespace ProcGen
