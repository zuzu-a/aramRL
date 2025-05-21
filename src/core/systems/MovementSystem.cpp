#include "inc/core/systems/MovementSystem.hpp" // Adjust path
#include "inc/core/components/TransformComponent.hpp" // Adjust path
#include "inc/core/components/InputComponent.hpp"   // Adjust path
// #include "inc/core/components/PlayerStatsComponent.hpp" // If you create this
#include "inc/core/components/PlayerComponent.hpp" // Assuming player has some base speed or identifier
#include <iostream> // For potential debug prints

MovementSystem::MovementSystem() {}
MovementSystem::~MovementSystem() {}

void MovementSystem::Update(entt::registry& registry, float deltaTime) {
    // Example: Define a base speed here. Ideally, this comes from a component.
    const float baseSpeed = 200.0f; // pixels per second

    auto view = registry.view<TransformComponent, const InputComponent, const PlayerComponent>();

    for (auto entity : view) {
        auto& transform = view.get<TransformComponent>(entity);
        const auto& input = view.get<const InputComponent>(entity);
        // const auto& player = view.get<const PlayerComponent>(entity); // If speed is per-player

        // Apply movement based on input direction
        if (input.movement_direction.Length() > 0.001f) { // Check if there's any input (use small epsilon)
            // No need to normalize input.movement_direction again if InputSystem already does it.
            transform.position.x += input.movement_direction.x * baseSpeed * deltaTime;
            transform.position.y += input.movement_direction.y * baseSpeed * deltaTime;
            
            // Debugging: Print new position
            // std::cout << "Entity " << static_cast<uint32_t>(entity) 
            //           << " new position: (" << transform.position.x 
            //           << ", " << transform.position.y << ")" << std::endl;
        }
    }
}
