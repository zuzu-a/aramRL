#ifndef MOVEMENT_SYSTEM_HPP
#define MOVEMENT_SYSTEM_HPP

#include <entt/entt.hpp>

// Forward declarations of components it will use
// struct TransformComponent;
// struct InputComponent;
// struct PlayerStatsComponent; // Or use PlayerComponent if speed is there

class MovementSystem {
public:
    MovementSystem();
    ~MovementSystem();

    // Speed is a temporary parameter; ideally, it comes from a component
    void Update(entt::registry& registry, float deltaTime); 
};

#endif // MOVEMENT_SYSTEM_HPP
