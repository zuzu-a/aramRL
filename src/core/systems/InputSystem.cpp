#include "inc/core/systems/InputSystem.hpp" // Adjust path
#include "inc/core/components/InputComponent.hpp" // Adjust path
#include "inc/core/components/PlayerComponent.hpp" // Adjust path
#include <SDL_keyboard.h> // For SDL_GetKeyboardState
#include <iostream> // For debug prints

InputSystem::InputSystem() {}
InputSystem::~InputSystem() {}

void InputSystem::PrepareForNewFrame(entt::registry& registry) {
    auto view = registry.view<InputComponent>();
    for (auto entity : view) {
        auto& inputComp = view.get<InputComponent>(entity);
        inputComp.action_triggered = false; // Reset action trigger
        // movement_direction will be calculated from flags later in UpdateContinuousInput
    }
}

void InputSystem::ProcessEvent(entt::registry& registry, const SDL_Event& event) {
    auto view = registry.view<InputComponent, PlayerComponent>(); // Process for player entities
    for (auto entity : view) {
        auto& inputComp = view.get<InputComponent>(entity);
        if (event.type == SDL_KEYDOWN && !event.key.repeat) {
            if (event.key.keysym.sym == SDLK_SPACE) { // Example: Space for attack
                inputComp.current_action = InputAction::ATTACK;
                inputComp.action_triggered = true;
                // std::cout << "Attack action triggered for entity " << static_cast<uint32_t>(entity) << std::endl; // Debug
            }
        }
        // Can add more event handling here (e.g., mouse clicks)
    }
}
 
void InputSystem::UpdateMovementFlags(InputComponent& inputComp) {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    inputComp.move_up = keystate[SDL_SCANCODE_W];
    inputComp.move_down = keystate[SDL_SCANCODE_S];
    inputComp.move_left = keystate[SDL_SCANCODE_A];
    inputComp.move_right = keystate[SDL_SCANCODE_D];
}

void InputSystem::UpdateContinuousInput(entt::registry& registry) {
    auto view = registry.view<InputComponent, PlayerComponent>(); // Process for player entities
    for (auto entity : view) {
        auto& inputComp = view.get<InputComponent>(entity);
         
        UpdateMovementFlags(inputComp); // Update movement flags first

        inputComp.movement_direction = {0.0f, 0.0f}; // Reset before applying new directions

        if (inputComp.move_up)    inputComp.movement_direction.y -= 1.0f;
        if (inputComp.move_down)  inputComp.movement_direction.y += 1.0f;
        if (inputComp.move_left)  inputComp.movement_direction.x -= 1.0f;
        if (inputComp.move_right) inputComp.movement_direction.x += 1.0f;

        // Normalize if diagonal movement should not be faster
        if (inputComp.movement_direction.Length() > 0.1f) { // Check against small epsilon
             inputComp.movement_direction = inputComp.movement_direction.Normalized();
        }
        
        // Debug print for movement direction
        // if (inputComp.movement_direction.x != 0 || inputComp.movement_direction.y != 0) {
        //     std::cout << "Entity " << static_cast<uint32_t>(entity) << " movement_direction: ("
        //               << inputComp.movement_direction.x << ", "
        //               << inputComp.movement_direction.y << ")" << std::endl;
        // }
    }
}
