#ifndef ROGUE_H
#define ROGUE_H



/* - - - - - - - - - - - - - - - - - - */
// EXTERNAL LIBRARIES
/* - - - - - - - - - - - - - - - - - - */

#include <SDL.h>
#include <SDL_image.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <entt/entt.hpp>
#include <lodepng.h>
#include <nlohmann/json.hpp>

/* - - - - - - - - - - - - - - - - - - */
// INLINE
/* - - - - - - - - - - - - - - - - - - */

// TODO: entt::registry will be managed by a central game state manager or an Engine/Application class.
// TODO: Caches (textureCache, fontCache, musicCache) will be part of a ResourceManager class.
// TODO: TileSet (tileset, spriteSheet) will be managed by ResourceManager or specific Map/Sprite managers.

using json = nlohmann::json; // Keep this if it's just a type alias and not a global instance

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
#include <tools/errors.hpp>

/* - - - - - - - - - - - - - - - - - - */
// CLASSES
/* - - - - - - - - - - - - - - - - - - */


/* - - - - - - - - - - - - - - - - - - */
// COMPONENTS
/* - - - - - - - - - - - - - - - - - - */


/* - - - - - - - - - - - - - - - - - - */
// LOCAL CONSTANTS
/* - - - - - - - - - - - - - - - - - - */

// TODO: Delta time calculation (delta_time, delta_samples, etc.) will be handled by the main game loop or an Engine class.
// SCREEN_WIDTH and SCREEN_HEIGHT were related to libtcod and removed.
// These might be redefined later for SDL window size.

#endif