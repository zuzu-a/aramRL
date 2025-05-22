//
// Based on "Fast Poisson Disk Sampling in Arbitrary Dimensions" by Robert Bridson
//
// Public domain.
//
// Notes:
// - Coordinates are normalized to 0..1 for the algorithm. Scale them to your world dimensions.
// - The 'k' parameter (default 30) is the number of samples to choose before rejection.
//   Higher k = slower but better distribution.
// - The R parameter (radius) is the minimum distance between samples.
//
// Example Usage:
//   std::vector<PoissonDiskSampling::Point> points;
//   PoissonDiskSampling::sample(width, height, radius, k, points, seed);
//   // points now contains 2D points with x,y in [0, width] and [0, height] respectively.
//

#pragma once

#include <vector>
#include <random>
#include <cmath>
#include <array>
#include <algorithm> // For std::max

namespace PoissonDiskSampling {

struct Point {
    float x, y;
};

namespace Internal {

    template <int DIMENSIONS>
    struct Sample {
        std::array<float, DIMENSIONS> pos;
        bool valid = false;
        int gridPos[DIMENSIONS];

        Sample() = default;
        Sample(std::array<float, DIMENSIONS> p) : pos(p), valid(true) {}
    };

    template <int DIMENSIONS>
    float distSq(const Sample<DIMENSIONS>& s1, const Sample<DIMENSIONS>& s2) {
        float d = 0;
        for (int i = 0; i < DIMENSIONS; ++i) {
            float diff = s1.pos[i] - s2.pos[i];
            d += diff * diff;
        }
        return d;
    }

    template <int DIMENSIONS>
    void toGridPos(const Sample<DIMENSIONS>& s, float cellSize, int gridPos[DIMENSIONS]) {
        for (int i = 0; i < DIMENSIONS; ++i) {
            gridPos[i] = static_cast<int>(s.pos[i] / cellSize);
        }
    }

    template <int DIMENSIONS>
    std::array<float, DIMENSIONS> randomPointAround(const Sample<DIMENSIONS>& s, float r, std::mt19937& gen) {
        std::uniform_real_distribution<float> dist(0, 1);
        std::array<float, DIMENSIONS> p;
        
        // Generate random point in a DIMENSIONS-sphere (annulus for 2D/3D)
        // Using uniform distribution in spherical coordinates
        float radius_sq;
        do {
            for (int i = 0; i < DIMENSIONS; ++i) {
                p[i] = dist(gen) * 2.0f - 1.0f; // Range -1 to 1
            }
            radius_sq = 0;
            for (int i = 0; i < DIMENSIONS; ++i) {
                radius_sq += p[i] * p[i];
            }
        } while (radius_sq > 1.0f || radius_sq == 0.0f); // Ensure it's inside unit sphere and not zero

        float length = std::sqrt(radius_sq);
        float R = r * (dist(gen) + 1.0f); // Random radius between r and 2r

        for(int i = 0; i < DIMENSIONS; ++i) {
            p[i] = s.pos[i] + p[i]/length * R;
        }
        return p;
    }

} // namespace Internal

// Generates points in [0, worldSize[0]] x [0, worldSize[1]] ...
template <int DIMENSIONS>
void sample(const std::array<float, DIMENSIONS>& worldSize,
            float r,
            int k, // Number of samples to try before rejection
            std::vector<Internal::Sample<DIMENSIONS>>& samples,
            unsigned int seed = std::random_device{}()) {

    std::mt19937 gen(seed);
    std::uniform_real_distribution<float> dist(0, 1);

    float rSq = r * r;
    float cellSize = r / std::sqrt(static_cast<float>(DIMENSIONS));

    std::array<int, DIMENSIONS> gridSize;
    for (int i = 0; i < DIMENSIONS; ++i) {
        gridSize[i] = static_cast<int>(std::ceil(worldSize[i] / cellSize));
    }

    // Using a map for sparse grid, could use std::vector<std::vector<...>> for dense
    // For simplicity here, assume grid is dense enough for vector of vectors.
    // A more robust approach would be a hash map for sparse grids or higher dimensions.
    // For 2D:
    std::vector<std::vector<Internal::Sample<DIMENSIONS>*>> grid(gridSize[0], std::vector<Internal::Sample<DIMENSIONS>*>(gridSize[1], nullptr));
    
    if (DIMENSIONS != 2) {
        // This simple grid is only for 2D for now.
        // Higher dimensions need a more complex grid structure (e.g., hash grid or flat vector with N-dim indexing).
        // For this example, let's return if not 2D.
        // Or throw an exception.
        return; 
    }


    std::vector<Internal::Sample<DIMENSIONS>> activeList;
    samples.clear();

    // Initial sample
    std::array<float, DIMENSIONS> initialPos;
    for (int i = 0; i < DIMENSIONS; ++i) {
        initialPos[i] = dist(gen) * worldSize[i];
    }
    Internal::Sample<DIMENSIONS> initialSample(initialPos);
    Internal::toGridPos(initialSample, cellSize, initialSample.gridPos);

    activeList.push_back(initialSample);
    samples.push_back(initialSample);
    grid[initialSample.gridPos[0]][initialSample.gridPos[1]] = &samples.back();


    while (!activeList.empty()) {
        std::uniform_int_distribution<int> activeDist(0, static_cast<int>(activeList.size()) - 1);
        int activeIdx = activeDist(gen);
        Internal::Sample<DIMENSIONS> currentSample = activeList[activeIdx];

        bool foundSample = false;
        for (int i = 0; i < k; ++i) {
            std::array<float, DIMENSIONS> newPos = Internal::randomPointAround(currentSample, r, gen);
            Internal::Sample<DIMENSIONS> newSample(newPos);
            Internal::toGridPos(newSample, cellSize, newSample.gridPos);

            bool inBounds = true;
            for (int dim = 0; dim < DIMENSIONS; ++dim) {
                if (newSample.pos[dim] < 0 || newSample.pos[dim] >= worldSize[dim]) {
                    inBounds = false;
                    break;
                }
            }
            if (!inBounds) continue;

            bool tooClose = false;
            // Check neighbors in grid
            // For 2D: 5x5 neighborhood (including diagonals)
            for (int R = -2; R <= 2; ++R) {
                for (int C = -2; C <= 2; ++C) {
                    int ngx = newSample.gridPos[0] + C;
                    int ngy = newSample.gridPos[1] + R;

                    if (ngx >= 0 && ngx < gridSize[0] && ngy >= 0 && ngy < gridSize[1]) {
                        Internal::Sample<DIMENSIONS>* neighbor = grid[ngx][ngy];
                        if (neighbor && Internal::distSq(*neighbor, newSample) < rSq) {
                            tooClose = true;
                            break;
                        }
                    }
                }
                if (tooClose) break;
            }

            if (!tooClose) {
                samples.push_back(newSample);
                activeList.push_back(newSample);
                grid[newSample.gridPos[0]][newSample.gridPos[1]] = &samples.back();
                foundSample = true;
                // break; // As per Bridson's algorithm, can break after finding one valid sample
            }
        }

        if (!foundSample) {
            activeList.erase(activeList.begin() + activeIdx);
        }
    }
}

// Convenience wrapper for 2D points (outputting Point struct)
void sample(float worldWidth, float worldHeight, float r, int k, std::vector<Point>& points, unsigned int seed = std::random_device{}()) {
    std::vector<Internal::Sample<2>> internal_samples;
    sample<2>({worldWidth, worldHeight}, r, k, internal_samples, seed);
    points.clear();
    points.reserve(internal_samples.size());
    for (const auto& s : internal_samples) {
        points.push_back({s.pos[0], s.pos[1]});
    }
}


} // namespace PoissonDiskSampling
