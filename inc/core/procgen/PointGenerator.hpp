#ifndef POINT_GENERATOR_HPP
#define POINT_GENERATOR_HPP

#include "inc/core/components/vector2d.hpp" // For Vector2D
#include <vector>
#include <string> // For seed string, or use unsigned int

// Define AABB struct here or include if defined elsewhere
struct AABB {
    Vector2D min = {0.0f, 0.0f};
    Vector2D max = {0.0f, 0.0f};

    AABB() = default;
    AABB(const Vector2D& min_p, const Vector2D& max_p) : min(min_p), max(max_p) {}
    AABB(float min_x, float min_y, float max_x, float max_y) : min(min_x, min_y), max(max_x, max_y) {}

    float GetWidth() const { return max.x - min.x; }
    float GetHeight() const { return max.y - min.y; }
};


class PointGenerator {
public:
    PointGenerator();

    // Generates points using Poisson Disk Sampling
    std::vector<Vector2D> GeneratePoissonPoints(
        const AABB& bounds, 
        float minDistance, 
        int k = 30, // Max attempts before rejection (from Bridson's algorithm)
        unsigned int seed = 0 // 0 for random_device seed
    );

    // Generates points along the boundary of the AABB
    std::vector<Vector2D> GenerateBoundaryPoints(
        const AABB& bounds, 
        float spacing
    );

    // Combined method (conceptual, for now just calls GeneratePoissonPoints)
    std::vector<Vector2D> GeneratePoints(
        const AABB& bounds, 
        float spacing, 
        float mountainSpacing, // Currently unused, for future differentiation
        unsigned int seed = 0
    );
};

#endif // POINT_GENERATOR_HPP
