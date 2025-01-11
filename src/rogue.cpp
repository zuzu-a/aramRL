#include <rogue.hpp>
#include <render.hpp>
#include <systems.hpp>
#include <tileset.hpp>


int main(int argc, char* argv[]) {

/* - - - - - - - - - - - - - - - - - - */
// TESTING VARIABLES
/* - - - - - - - - - - - - - - - - - - */

    // Initialize SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error("Failed to initialize SDL: " + std::string(SDL_GetError()));
    }

    // Create a window.
    SDL_Window *window = SDL_CreateWindow("Aram", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, 0);

    if (!window) {
        throw std::runtime_error("Failed to create window: " + std::string(SDL_GetError()));
    }

    // Create a renderer.
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!renderer) {
        throw std::runtime_error("Failed to create renderer: " + std::string(SDL_GetError()));
    }



    // Test map for debugging.
    int map[10][10] = {
        { 16, 24, 24, 24, 24, 24, 24, 24, 24, 16 },
        { 16, 8, 8, 8, 8, 8, 8, 8, 8, 16 },
        { 16, 8, 12, 12, 12, 12, 12, 12, 8, 16 },
        { 16, 8, 12, 12, 12, 12, 12, 12, 8, 16 },
        { 16, 8, 12, 12, 12, 12, 12, 12, 8, 16 },
        { 16, 8, 12, 12, 12, 12, 12, 12, 8, 16 },
        { 16, 8, 12, 12, 12, 12, 12, 12, 8, 16 },
        { 16, 8, 8, 8, 8, 8, 8, 8, 8, 16 },
        { 16, 8, 8, 8, 8, 8, 8, 8, 8, 16 },
        { 24, 24, 24, 24, 24, 24, 24, 24, 24, 24 },
    };

    Tileset tileset(renderer, "assets/gfx/TerrainTest.png", 16, 24);

    // Main loop.

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Clear the screen.
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render the map.
        for (int y = 0; y < 10; y++) {
            for (int x = 0; x < 10; x++) {
                tileset.renderTile(renderer, map[y][x], x * tileset.getTileWidth(), y * tileset.getTileHeight());
            }
        }

        // Present the screen.
        SDL_RenderPresent(renderer);
    }

    return 0;
}