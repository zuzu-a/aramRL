/*
 * OpenSimplex Noise in C++
 * Ported by K. S. Kynd
 * Based on Java code by Kurt Spencer
 *
 * This is a public domain implementation of OpenSimplex noise.
 */

#pragma once

#include <vector>
#include <cstdint> // For int32_t, int64_t
#include <algorithm> // For std::sort (not used in this basic version but often in more complex noise)
#include <cmath>     // For floor, etc.

class SimplexNoise {
private:
    static const double STRETCH_CONSTANT_2D;
    static const double SQUISH_CONSTANT_2D;
    static const double NORM_CONSTANT_2D;

    std::vector<int16_t> perm;
    std::vector<int16_t> permGradIndex3D; // Not used for 2D, but part of original structure

    // Gradients for 2D. These are the vectors from the edges of the simplex to the corners.
    static const int8_t gradients2D[];

    double extrapolate(int xsb, int ysb, double dx, double dy);

public:
    // Initializes permutation array using a seed.
    // This method is not thread safe due to the static PRNG.
    SimplexNoise(int64_t seed = 0);

    // 2D OpenSimplex Noise.
    double Evaluate(double x, double y);
};

// Constants for 2D Simplex Noise
const double SimplexNoise::STRETCH_CONSTANT_2D = -0.211324865405187;    //(1/Math.sqrt(2+1)-1)/2;
const double SimplexNoise::SQUISH_CONSTANT_2D = 0.366025403784439;      //(Math.sqrt(2+1)-1)/2;
const double SimplexNoise::NORM_CONSTANT_2D = 47.0;

const int8_t SimplexNoise::gradients2D[] = {
     5,  2,    2,  5,
    -5,  2,   -2,  5,
     5, -2,    2, -5,
    -5, -2,   -2, -5,
};

// Private method
double SimplexNoise::extrapolate(int xsb, int ysb, double dx, double dy) {
    int index = perm[(perm[xsb & 0xFF] + ysb) & 0xFF] & 0x0E;
    return gradients2D[index] * dx + gradients2D[index + 1] * dy;
}

// Public methods
SimplexNoise::SimplexNoise(int64_t seed) {
    perm.resize(256);
    // Not used for 2D noise, but kept for compatibility with original OpenSimplex structure
    permGradIndex3D.resize(256); 

    std::vector<int16_t> source(256);
    for (int16_t i = 0; i < 256; i++)
        source[i] = i;
    
    // Simple PRNG for shuffling (LCG)
    // For more robust seeding, use std::mt19937 or similar
    if (seed == 0) { // Use a default seed if 0
        seed = 123456789; 
    }
    for (int i = 255; i >= 0; i--) {
        seed = (seed * 6364136223846793005LL + 1442695040888963407LL);
        int r = static_cast<int>((seed + 31) % (i + 1));
        if (r < 0)
            r += (i + 1);
        perm[i] = source[r];
        //permGradIndex3D[i] = static_cast<int16_t>((perm[i] % (gradients3D.size() / 3)) * 3); // Not for 2D
        source[r] = source[i];
    }
}

double SimplexNoise::Evaluate(double x, double y) {
    // Place input coordinates onto grid.
    double stretchOffset = (x + y) * STRETCH_CONSTANT_2D;
    double xs = x + stretchOffset;
    double ys = y + stretchOffset;

    // Floor to get grid coordinates of rhombus (stretched square) cell origin.
    int xsb = static_cast<int>(std::floor(xs));
    int ysb = static_cast<int>(std::floor(ys));

    // Skew out to get actual coordinates of rhombus origin.
    double squishOffset = (xsb + ysb) * SQUISH_CONSTANT_2D;
    double xb = xsb + squishOffset;
    double yb = ysb + squishOffset;

    // Compute grid coordinates relative to rhombus origin.
    double xins = xs - xsb;
    double yins = ys - ysb;

    // Sum those together to get a value that determines which region we're in.
    double inSum = xins + yins;

    // Positions relative to origin point.
    double dx0 = x - xb;
    double dy0 = y - yb;

    // Contribution (1,0)
    double dx1 = dx0 - 1 - SQUISH_CONSTANT_2D;
    double dy1 = dy0 - 0 - SQUISH_CONSTANT_2D;
    double attn1 = 2 - dx1 * dx1 - dy1 * dy1;
    double value = 0; // Initialize value

    if (attn1 > 0) {
        attn1 *= attn1;
        value += attn1 * attn1 * extrapolate(xsb + 1, ysb + 0, dx1, dy1);
    }

    // Contribution (0,1)
    double dx2 = dx0 - 0 - SQUISH_CONSTANT_2D;
    double dy2 = dy0 - 1 - SQUISH_CONSTANT_2D;
    double attn2 = 2 - dx2 * dx2 - dy2 * dy2;
    if (attn2 > 0) {
        attn2 *= attn2;
        value += attn2 * attn2 * extrapolate(xsb + 0, ysb + 1, dx2, dy2);
    }

    int xsv_ext, ysv_ext;
    double dx_ext, dy_ext;

    if (inSum <= 1) { // We're inside the triangle (2-Simplex) at (0,0)
        double zins = 1 - inSum;
        if (zins > xins || zins > yins) { // (0,0) is one of the closest two triangular vertices
            if (xins > yins) {
                xsv_ext = xsb + 1;
                ysv_ext = ysb - 1;
                dx_ext = dx0 - 1;
                dy_ext = dy0 + 1;
            } else {
                xsv_ext = xsb - 1;
                ysv_ext = ysb + 1;
                dx_ext = dx0 + 1;
                dy_ext = dy0 - 1;
            }
        } else { // (1,0) and (0,1) are the closest two vertices.
            xsv_ext = xsb + 1;
            ysv_ext = ysb + 1;
            dx_ext = dx0 - 1 - 2 * SQUISH_CONSTANT_2D;
            dy_ext = dy0 - 1 - 2 * SQUISH_CONSTANT_2D;
        }
    } else { // We're inside the triangle (2-Simplex) at (1,1)
        double zins = 2 - inSum;
        if (zins < xins || zins < yins) { // (0,0) is one of the closest two triangular vertices
            if (xins > yins) {
                xsv_ext = xsb + 2;
                ysv_ext = ysb + 0;
                dx_ext = dx0 - 2 - 2 * SQUISH_CONSTANT_2D;
                dy_ext = dy0 - 0 - 2 * SQUISH_CONSTANT_2D;
            } else {
                xsv_ext = xsb + 0;
                ysv_ext = ysb + 2;
                dx_ext = dx0 - 0 - 2 * SQUISH_CONSTANT_2D;
                dy_ext = dy0 - 2 - 2 * SQUISH_CONSTANT_2D;
            }
        } else { // (1,0) and (0,1) are the closest two vertices.
            xsv_ext = xsb;
            ysv_ext = ysb;
            dx_ext = dx0;
            dy_ext = dy0;
        }
        xsb += 1;
        ysb += 1;
        dx0 = dx0 - 1 - 2 * SQUISH_CONSTANT_2D;
        dy0 = dy0 - 1 - 2 * SQUISH_CONSTANT_2D;
    }

    // Contribution from (0,0) or (1,1)
    double attn0 = 2 - dx0 * dx0 - dy0 * dy0;
    if (attn0 > 0) {
        attn0 *= attn0;
        value += attn0 * attn0 * extrapolate(xsb, ysb, dx0, dy0);
    }
    
    // Extra Vertex
    double attn_ext = 2 - dx_ext * dx_ext - dy_ext * dy_ext;
    if (attn_ext > 0) {
        attn_ext *= attn_ext;
        value += attn_ext * attn_ext * extrapolate(xsv_ext, ysv_ext, dx_ext, dy_ext);
    }

    return value / NORM_CONSTANT_2D;
}
