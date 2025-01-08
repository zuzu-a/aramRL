#ifndef ROGUE_H
#define ROGUE_H

#include <stdlib.h>

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

#endif