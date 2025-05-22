#ifndef RETRO_DITHER_HPP
#define RETRO_DITHER_HPP

#include <SDL.h> // For SDL_Surface
#include <vector>

namespace RetroDither {
    // Bayer matrices are defined with integer values.
    // The thresholding logic will typically involve normalizing these against their range.
    // For example, a 2x2 matrix has values 0,1,2,3. A pixel (0-255) can be scaled to 0-3.99,
    // then compared. Or the matrix values are scaled to match the 0-255 pixel range.
    // The provided example scales the matrix values (e.g. BAYER_SCALE_2X2 = 1.0f / 4.0f)
    // and then compares a normalized pixel value (0-1) against this scaled threshold.

    // 2x2 Bayer matrix
    const int BAYER_MATRIX_2X2[2][2] = {
        {0, 2},
        {3, 1}
    };
    // To use this: scale pixel to 0-3 range. threshold = BAYER_MATRIX_2X2[x%2][y%2]. output = pixel_scaled > threshold ? max_val : 0;
    // OR: scale matrix values: e.g. (BAYER_MATRIX_2X2[x%2][y%2] / 4.0f) * 255 as threshold for 0-255 pixel.
    // The example in the prompt uses: (gray / 255.0f > (matrix_val * scale_factor))
    const float BAYER_SCALE_2X2 = 1.0f / 4.0f; 

    // 4x4 Bayer matrix
    const int BAYER_MATRIX_4X4[4][4] = {
        { 0,  8,  2, 10},
        {12,  4, 14,  6},
        { 3, 11,  1,  9},
        {15,  7, 13,  5}
    };
    const float BAYER_SCALE_4X4 = 1.0f / 16.0f;

    // 8x8 Bayer Matrix (Optional, more levels)
    const int BAYER_MATRIX_8X8[8][8] = {
        { 0, 32,  8, 40,  2, 34, 10, 42},
        {48, 16, 56, 24, 50, 18, 58, 26},
        {12, 44,  4, 36, 14, 46,  6, 38},
        {60, 28, 52, 20, 62, 30, 54, 22},
        { 3, 35, 11, 43,  1, 33,  9, 41},
        {51, 19, 59, 27, 49, 17, 57, 25},
        {15, 47,  7, 39, 13, 45,  5, 37},
        {63, 31, 55, 23, 61, 29, 53, 21}
    };
    const float BAYER_SCALE_8X8 = 1.0f / 64.0f;


    // Function to apply dithering to a surface (CPU-based)
    // Takes a source surface, modifies/creates a destination surface.
    // Destination surface should be pre-allocated with the same dimensions and format.
    void ApplyBayerDithering(SDL_Surface* srcSurface, SDL_Surface* dstSurface, const int matrixSize = 4);
    
} // namespace RetroDither

#endif // RETRO_DITHER_HPP
