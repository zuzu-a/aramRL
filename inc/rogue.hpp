#ifndef ROGUE_H
#define ROGUE_H



/* - - - - - - - - - - - - - - - - - - */
// EXTERNAL LIBRARIES
/* - - - - - - - - - - - - - - - - - - */

#include <SDL.h>
#include <SDL_image.h>
#include <libtcod.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* - - - - - - - - - - - - - - - - - - */
// LOCAL INCLUDES FROM SOURCE CODE
/* - - - - - - - - - - - - - - - - - - */

#include <render.hpp>
#include <tileset.hpp>
#include <systems.hpp>

/* - - - - - - - - - - - - - - - - - - */
// CLASSES
/* - - - - - - - - - - - - - - - - - - */


/* - - - - - - - - - - - - - - - - - - */
// COMPONENTS
/* - - - - - - - - - - - - - - - - - - */

using Entity = uint32_t;

typedef struct Position {
    int x;
    int y;
} Position;

typedef struct Velocity {
    int dx;
    int dy;
} Velocity;

typedef struct LightSource {
    int brightness;
    SDL_Color color;
} LightSource;

typedef struct Inventory {
    std::vector<int> items; // Item IDs
} Inventory;

typedef struct Item {
    std::string name;
    int weight;
} Item;

typedef struct Tile {
    int tile_id;
    bool isWalkable;
    bool isTransparent;
} Tile;

/* - - - - - - - - - - - - - - - - - - */
// LOCAL CONSTANTS
/* - - - - - - - - - - - - - - - - - - */


// Screen size
#define SCREEN_WIDTH 24
#define SCREEN_HEIGHT 12

// Delta time
#define DELTA_SAMPLES_LENGTH 64
static float delta_time = 0.0f;  // The time in seconds of the current frame.
static float delta_samples[DELTA_SAMPLES_LENGTH] = {0};
static int last_delta_sample = 0;

// Custom charmap for libtcod (Deprecated. Using SDL2 instead.)
static constexpr std::array<int, 48> CHARMAP_TILESET = {0x0000, 0x263A, 0x263B, 0x2665, 0x2666, 0x2663, 0x2660, 0x2022, 0x25D8, 0x25CB, 0x25D9, 0x2642, 0x2640, 0x266A, 0x266B, 0x263C, 0x25BA, 0x25C4, 0x2195, 0x203C, 0x00B6, 0x00A7, 0x25AC, 0x21A8, 0x2191, 0x2193, 0x2192, 0x2190, 0x221F, 0x2194, 0x25B2, 0x25BC, 0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,};

#endif