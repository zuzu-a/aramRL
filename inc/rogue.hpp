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

// Screen size
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 50

// Screen position
#define SAMPLE_SCREEN_X 20
#define SAMPLE_SCREEN_Y 10

static float delta_time = 0.0f;  // The time in seconds of the current frame.
#define DELTA_SAMPLES_LENGTH 64
static float delta_samples[DELTA_SAMPLES_LENGTH] = {0};
static int last_delta_sample = 0;

#endif