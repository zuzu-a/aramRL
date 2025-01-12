#include <rogue.hpp>

/*  - - - - - - - - - - - - - - */
//  MOVEMENT SYSTEM             //
/*  - - - - - - - - - - - - - - */


class InputSystem {
public:
    void InputMovement(float dt) {
        auto state  = Events::KeyboardState();
        auto view   = registry.view<Player, Input>();

        if (view.begin() != view.end()) {
            auto &&[input] = view.get<Input>(*view.begin());
            if      (state[SDL_SCANCODE_W] && state[SDL_SCANCODE_D])    { /* player.input = NorthEast */ }
            else if (state[SDL_SCANCODE_W] && state[SDL_SCANCODE_A])    { /* player.input = NorthWest */ }
            else if (state[SDL_SCANCODE_S] && state[SDL_SCANCODE_D])    { /* player.input = SouthEast */ }
            else if (state[SDL_SCANCODE_S] && state[SDL_SCANCODE_A])    { /* player.input = SouthWest */ }
            else if (state[SDL_SCANCODE_W])                             { /* player.input = North     */ }
            else if (state[SDL_SCANCODE_A])                             { /* player.input = West      */ }
            else if (state[SDL_SCANCODE_S])                             { /* player.input = South     */ }
            else if (state[SDL_SCANCODE_D])                             { /* player.input = East      */ }
            else                                                        { /* destroy player.input     */ }
        }


    };

    void InputAttack(float dt) {
        auto state  = sdl2::Events::KeyboardState();
        auto view   = registry.view<Player, Input>();

        if (view.begin() != view.end()) {
            
        }
    };


private:

};