#include <rogue.hpp>

/*  - - - - - - - - - - - - - - */
//  MOVEMENT SYSTEM             //
/*  - - - - - - - - - - - - - - */


class MovementSystem {
public:
    void update(Position& position, const Input& input) {
        if (input.moveUp) position.y--;
        if (input.moveDown) position.y++;
        if (input.moveLeft) position.x--;
        if (input.moveRight) position.x++;
    }
};