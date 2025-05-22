#include "inc/core/RetroDither.hpp" // Adjust path as needed
#include <iostream> // For std::cerr

namespace RetroDither {

void ApplyBayerDithering(SDL_Surface* srcSurface, SDL_Surface* dstSurface, const int matrixSize) {
    if (!srcSurface || !dstSurface) {
        std::cerr << "ApplyBayerDithering: Source or destination surface is null." << std::endl;
        return;
    }
    if (srcSurface->w != dstSurface->w || srcSurface->h != dstSurface->h) {
        std::cerr << "ApplyBayerDithering: Source and destination surface dimensions do not match." << std::endl;
        return;
    }
    // Ensure dstSurface is of a suitable format, e.g., same as src or a known format like ARGB8888
    // For this example, we assume they are compatible or dstSurface is set up correctly.

    if (SDL_LockSurface(srcSurface) != 0) {
        std::cerr << "ApplyBayerDithering: Failed to lock source surface: " << SDL_GetError() << std::endl;
        return;
    }
    if (SDL_LockSurface(dstSurface) != 0) {
        std::cerr << "ApplyBayerDithering: Failed to lock destination surface: " << SDL_GetError() << std::endl;
        SDL_UnlockSurface(srcSurface);
        return;
    }

    Uint8 r, g, b, a; // To hold pixel components
    Uint32* srcPixels = static_cast<Uint32*>(srcSurface->pixels);
    Uint32* dstPixels = static_cast<Uint32*>(dstSurface->pixels);

    const int (*bayerMatrixPtr)[4] = nullptr; // Using 4x4 as a common case
    float bayerScale = 1.0f;
    int currentMatrixSize = 4; // Default to 4x4

    if (matrixSize == 2) {
        // Not directly assigning BAYER_MATRIX_2X2 to bayerMatrixPtr due to type mismatch [2][2] vs [4][4]
        // We'll handle this case by indexing directly inside the loop.
        currentMatrixSize = 2;
        bayerScale = BAYER_SCALE_2X2;
    } else if (matrixSize == 4) {
        bayerMatrixPtr = BAYER_MATRIX_4X4; // Assign if matrixSize is 4
        currentMatrixSize = 4;
        bayerScale = BAYER_SCALE_4X4;
    } else if (matrixSize == 8) {
        // Similar to 2x2, handle 8x8 by direct indexing or use a common pointer type if possible
        currentMatrixSize = 8;
        bayerScale = BAYER_SCALE_8X8;
    } else {
        std::cerr << "ApplyBayerDithering: Unsupported matrix size " << matrixSize << ". Defaulting to 4x4." << std::endl;
        bayerMatrixPtr = BAYER_MATRIX_4X4;
        currentMatrixSize = 4;
        bayerScale = BAYER_SCALE_4X4;
    }


    for (int y = 0; y < srcSurface->h; ++y) {
        for (int x = 0; x < srcSurface->w; ++x) {
            Uint32 pixel = srcPixels[y * (srcSurface->pitch / sizeof(Uint32)) + x];
            SDL_GetRGBA(pixel, srcSurface->format, &r, &g, &b, &a); // Get RGBA, alpha is preserved

            // Convert to grayscale
            float gray = 0.299f * r + 0.587f * g + 0.114f * b;

            float threshold_normalized = 0.0f;
            if (currentMatrixSize == 2) {
                threshold_normalized = BAYER_MATRIX_2X2[y % 2][x % 2] * bayerScale;
            } else if (currentMatrixSize == 4) {
                threshold_normalized = BAYER_MATRIX_4X4[y % 4][x % 4] * bayerScale;
            } else if (currentMatrixSize == 8) {
                 threshold_normalized = BAYER_MATRIX_8X8[y % 8][x % 8] * bayerScale;
            }
            
            Uint8 output_gray_level = (gray / 255.0f > threshold_normalized) ? 255 : 0;
            
            dstPixels[y * (dstSurface->pitch / sizeof(Uint32)) + x] = SDL_MapRGBA(dstSurface->format, output_gray_level, output_gray_level, output_gray_level, a);
        }
    }

    SDL_UnlockSurface(dstSurface);
    SDL_UnlockSurface(srcSurface);
}

} // namespace RetroDither
