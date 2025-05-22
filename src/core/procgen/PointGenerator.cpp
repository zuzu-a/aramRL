#include "inc/core/procgen/PointGenerator.hpp"
#include "external/poisson_disk_sampling/PoissonDiskSampling.h" // Adjust path as needed
#include <algorithm> // For std::min/max if used for boundary points
#include <iostream>  // For debug

PointGenerator::PointGenerator() {}

std::vector<Vector2D> PointGenerator::GeneratePoissonPoints(
    const AABB& bounds, 
    float minDistance, 
    int k, 
    unsigned int seed
) {
    std::vector<PoissonDiskSampling::Point> generated_points_pds;
    float worldWidth = bounds.GetWidth();
    float worldHeight = bounds.GetHeight();

    if (worldWidth <= 0 || worldHeight <= 0) {
        // std::cerr << "PointGenerator: Invalid bounds for Poisson Disk Sampling." << std::endl;
        return {};
    }
    
    // The library's sample function generates points in [0, worldSize] relative to its input.
    // We need to translate these points to be within the AABB's min/max.
    PoissonDiskSampling::sample(worldWidth, worldHeight, minDistance, k, generated_points_pds, seed);

    std::vector<Vector2D> final_points;
    final_points.reserve(generated_points_pds.size());
    for (const auto& pds_point : generated_points_pds) {
        final_points.push_back({bounds.min.x + pds_point.x, bounds.min.y + pds_point.y});
    }
    return final_points;
}

std::vector<Vector2D> PointGenerator::GenerateBoundaryPoints(
    const AABB& bounds, 
    float spacing
) {
    std::vector<Vector2D> boundary_points;
    if (spacing <= 0) return boundary_points;

    // Top edge (excluding last point to avoid duplicate with right edge start)
    for (float x = bounds.min.x; x < bounds.max.x; x += spacing) {
        boundary_points.push_back({x, bounds.min.y});
    }
    // Right edge (excluding last point to avoid duplicate with bottom edge start)
    for (float y = bounds.min.y; y < bounds.max.y; y += spacing) {
        boundary_points.push_back({bounds.max.x, y});
    }
    // Bottom edge (excluding last point to avoid duplicate with left edge start)
    for (float x = bounds.max.x; x > bounds.min.x; x -= spacing) {
        boundary_points.push_back({x, bounds.max.y});
    }
    // Left edge (excluding last point to avoid duplicate with top edge start)
    for (float y = bounds.max.y; y > bounds.min.y; y -= spacing) {
        boundary_points.push_back({bounds.min.x, y});
    }
    return boundary_points;
}

std::vector<Vector2D> PointGenerator::GeneratePoints(
    const AABB& bounds, 
    float spacing, 
    float mountainSpacing, // Currently unused, for future differentiation
    unsigned int seed
) {
    // For this initial version, we'll just generate one set of Poisson points.
    // Future: Could generate boundary, then mountain seeds, then general points,
    // and ensure no overlaps or combine them.
    // std::cout << "GeneratePoints: Using general spacing: " << spacing << std::endl;
    return GeneratePoissonPoints(bounds, spacing, 30, seed);
}
