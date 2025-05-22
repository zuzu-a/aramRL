#ifndef ENTITY_FACTORY_HPP
#define ENTITY_FACTORY_HPP

#include <entt/entt.hpp>
#include <string>
#include "ResourceManager.hpp" // To get definitions and possibly textures
#include "inc/core/components/vector2d.hpp" // For Vector2D in CreateCharacter/Item

// Forward declare components that might be commonly added
// struct TransformComponent;
// struct SpriteComponent; // Will be added in Phase 2
// struct PlayerComponent;
// struct InputComponent;

namespace Content { 
    struct CharacterDefinition; // Forward declaration
    struct ItemDefinition;      // Forward declaration
    struct ComponentData;       // Forward declaration for ApplyComponentData
} 


class EntityFactory {
public:
    EntityFactory(entt::registry& registry, ResourceManager& resourceManager);

    entt::entity CreateCharacter(const std::string& characterAlias, const Vector2D& position = {0.0f, 0.0f});
    entt::entity CreateItem(const std::string& itemAlias, const Vector2D& position = {0.0f, 0.0f});
    // Add CreateMonster, CreateSetpiece etc. later

private:
    entt::registry& m_registry;
    ResourceManager& m_resourceManager;

    // Helper to apply components based on ComponentData from definitions
    // Using CharacterDefinition's ComponentData for now, as ItemDefinition refers to it.
    void ApplyComponentData(entt::entity entity, const std::vector<Content::ComponentData>& components);
};

#endif // ENTITY_FACTORY_HPP
