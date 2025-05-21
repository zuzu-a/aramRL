#ifndef INPUT_SYSTEM_HPP
#define INPUT_SYSTEM_HPP

#include <SDL_events.h> // For SDL_Event
#include <entt/entt.hpp> // For entt::registry

// Forward declare components if not included directly
// struct InputComponent; 
// struct PlayerComponent; // Assuming input is primarily for player entities

class InputSystem {
public:
    InputSystem();
    ~InputSystem();

    // Call this once per frame before processing continuous input like movement
    void PrepareForNewFrame(entt::registry& registry);

    // Processes SDL_Events for actions that happen once per key press (e.g., attack)
    void ProcessEvent(entt::registry& registry, const SDL_Event& event);

    // Processes continuous input (e.g., movement) by checking current keyboard state
    void UpdateContinuousInput(entt::registry& registry);

private:
    // Helper to update movement flags in InputComponent based on keyboard state
    void UpdateMovementFlags(InputComponent& inputComp);
};

#endif // INPUT_SYSTEM_HPP
