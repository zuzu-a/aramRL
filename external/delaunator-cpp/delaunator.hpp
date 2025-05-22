#ifndef DELAUNATOR_HPP
#define DELAUNATOR_HPP

#include <vector>
#include <cmath> // For std::sqrt, std::pow, std::abs, etc.
#include <limits> // For std::numeric_limits
#include <algorithm> // For std::sort, std::max, std::min
#include <stdexcept> // For std::runtime_error

namespace delaunator {

// Forward declaration
class Delaunator;

constexpr double EPSILON = std::numeric_limits<double>::epsilon();
constexpr std::size_t INVALID_INDEX = std::numeric_limits<std::size_t>::max();

inline bool nearly_equals(double a, double b) {
    return std::abs(a - b) < EPSILON;
}

struct Point {
    double x;
    double y;
};

class Delaunator {
public:
    std::vector<double> const& coords;
    std::vector<std::size_t> triangles;
    std::vector<std::size_t> halfedges;
    std::vector<std::size_t> hull_prev;
    std::vector<std::size_t> hull_next;
    std::vector<std::size_t> hull_tri;
    std::size_t hull_start;

    Delaunator(std::vector<double> const& in_coords) :
        coords(in_coords) {
        std::size_t n = coords.size() >> 1;

        if (n == 0) { // No points, do nothing.
            return;
        }
        if (n > 0 && coords.size() % 2 != 0) {
            throw std::runtime_error("Odd number of coordinates");
        }
        if (n < 3) {
             // Not enough points for triangulation
            // std::cerr << "Warning: Less than 3 points provided for Delaunay triangulation." << std::endl;
            return;
        }

        // Placeholder: Create a single dummy triangle if there are at least 3 points
        // This is NOT a real Delaunay triangulation.
        // A real implementation would involve complex geometric calculations.
        if (n >= 3) {
            triangles.push_back(0);
            triangles.push_back(1);
            triangles.push_back(2);

            // Fill halfedges and hull with placeholder data if needed by API,
            // or leave them empty if the user is expected to check triangles.size().
            // For this placeholder, we'll leave them mostly empty.
            halfedges.resize(triangles.size(), INVALID_INDEX); // Example for halfedges
            
            // Simple convex hull for the first 3 points
            hull_start = 0;
            hull_prev.resize(n, INVALID_INDEX);
            hull_next.resize(n, INVALID_INDEX);
            hull_tri.resize(n, INVALID_INDEX);

            if (n >= 3) { // Ensure we have at least 3 points for a minimal hull
                hull_next[0] = 1; hull_prev[0] = 2;
                hull_next[1] = 2; hull_prev[1] = 0;
                hull_next[2] = 0; hull_prev[2] = 1;
                
                hull_tri[0] = 0; // Triangle 0 uses point 0
                hull_tri[1] = 0; // Triangle 0 uses point 1
                hull_tri[2] = 0; // Triangle 0 uses point 2
            }
        }
    }
};

} // namespace delaunator

#endif // DELAUNATOR_HPP
