#include <rogue.h>

Entity *createPlayer(Position start_pos, Attributes start_attr) {
    Entity *newPlayer = calloc(1, sizeof(Entity));

    newPlayer -> pos.x = start_pos.x;
    newPlayer -> pos.y = start_pos.y;

    newPlayer -> attr.strength = start_attr.strength;
    newPlayer -> attr.dexterity = start_attr.dexterity;
    newPlayer -> attr.intelligence = start_attr.intelligence;
    newPlayer -> attr.luck = start_attr.luck;
    
    newPlayer -> avatar = '@';

    return newPlayer;
};

void inputHandler(int input) {
    switch(input) {
        case 'w':
            player -> pos.y++;
        case 'a':
            player -> pos.x--;
        case 's':
            player -> pos.y--;
        case 'd':
            player -> pos.x++;
        default:
            break;
    };
};


int main(void) {
    initscr();
    endwin();
};