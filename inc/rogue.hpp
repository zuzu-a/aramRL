#ifndef ROGUE_H
#define ROGUE_H

#include <SDL.h>
#include <libtcod.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int y;
    int x;
} Position;

typedef struct {
    int strength;
    int dexterity;
    int intelligence;
    int luck;
} Attributes;

typedef struct {
    Position pos;
    Attributes attr;
    char avatar;
} Entity;


// player.c functions
Entity* createPlayer(Position start_pos);
void handleInput(int input);

// externs
extern Entity *player;

/* sample screen size */
#define SAMPLE_SCREEN_WIDTH 46
#define SAMPLE_SCREEN_HEIGHT 20
/* sample screen position */
#define SAMPLE_SCREEN_X 20
#define SAMPLE_SCREEN_Y 10

// A custom SDL event for when a sample is switched to.
#define ON_ENTER_USEREVENT (SDL_USEREVENT + 0)
// A custom SDL event to tell a sample to draw.
#define ON_DRAW_USEREVENT (SDL_USEREVENT + 1)

static float delta_time = 0.0f;  // The time in seconds of the current frame.
#define DELTA_SAMPLES_LENGTH 64
static float delta_samples[DELTA_SAMPLES_LENGTH] = {0};
static int last_delta_sample = 0;

#endif