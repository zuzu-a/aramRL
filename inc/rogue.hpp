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
#include <entt/entt.hpp>

/* - - - - - - - - - - - - - - - - - - */
// INLINE
/* - - - - - - - - - - - - - - - - - - */

inline entt::registry registry{};
inline sdl::TextureCache textureCache;
inline sdl::FontCache fontCache;
inline sdl::MusicCache musicCache;
inline TileSet tileset;
inline TileSet spriteSheet;

/* - - - - - - - - - - - - - - - - - - */
// LOCAL INCLUDES FROM SOURCE CODE
/* - - - - - - - - - - - - - - - - - - */

#include <sdl/EventHandler.hpp>
#include <render.hpp>
#include <tileset.hpp>
#include <systems/InputSystem.hpp>
#include <components/player.hpp>
#include <components/tileset.hpp>
#include <components/transform.hpp>
#include <components/vector2d.hpp>


/* - - - - - - - - - - - - - - - - - - */
// CLASSES
/* - - - - - - - - - - - - - - - - - - */


/* - - - - - - - - - - - - - - - - - - */
// COMPONENTS
/* - - - - - - - - - - - - - - - - - - */


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

#endif