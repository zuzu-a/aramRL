#ifndef INPUT_COMPONENT_HPP
#define INPUT_COMPONENT_HPP

#include "vector2d.hpp" // For movement_direction

enum class InputAction {
    NONE,
    ATTACK,
    // Add other actions as needed
};

struct InputComponent {
    Vector2D movement_direction = {0.0f, 0.0f};
    InputAction current_action = InputAction::NONE;
    bool action_triggered = false; // To signal a new action this frame

    // Flags for continuous movement, can be updated each frame by InputSystem
    bool move_up = false;
    bool move_down = false;
    bool move_left = false;
    bool move_right = false;
};

#endif // INPUT_COMPONENT_HPP
