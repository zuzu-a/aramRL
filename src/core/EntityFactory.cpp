#include "inc/core/EntityFactory.hpp" // Adjust path
#include "inc/core/content/CharacterDefinition.hpp" // For CharacterDefinition and ComponentData
#include "inc/core/content/ItemDefinition.hpp"
#include "inc/core/components/TransformComponent.hpp"
#include "inc/core/components/PlayerComponent.hpp"    // Example components
#include "inc/core/components/InputComponent.hpp"     // Example components
// #include "inc/core/components/SpriteComponent.hpp" // Will be used in Phase 2
#include <iostream> // For debug messages

EntityFactory::EntityFactory(entt::registry& registry, ResourceManager& resourceManager)
    : m_registry(registry), m_resourceManager(resourceManager) {}

// Helper to apply components based on ComponentData from definitions
// Using CharacterDefinition's ComponentData for now, as ItemDefinition refers to it.
// This can be templated or overloaded if other definition types have different ComponentData structures
// or need different handling.
void EntityFactory::ApplyComponentData(entt::entity entity, const std::vector<Content::ComponentData>& components) {
    for (const auto& compData : components) {
        if (compData.name == "PlayerComponent") {
            auto& pc = m_registry.emplace_or_replace<PlayerComponent>(entity);
            pc.player_id = compData.properties.value("player_id", 0); // Default to 0 if not specified
            pc.is_active = compData.properties.value("is_active", true); // Default to true
        } else if (compData.name == "InputComponent") {
            m_registry.emplace_or_replace<InputComponent>(entity);
            // Customize InputComponent from compData.properties if needed in the future
            // For example:
            // auto& ic = m_registry.get<InputComponent>(entity);
            // if (compData.properties.contains("some_input_flag")) {
            //    ic.some_flag = compData.properties.at("some_input_flag").get<bool>();
            // }
        } else if (compData.name == "TransformComponent") {
            // Usually TransformComponent is added separately with position,
            // but this allows overriding/setting defaults from JSON.
            auto& tc = m_registry.get_or_emplace<TransformComponent>(entity); // Use get_or_emplace if position is set before this
            if (compData.properties.contains("position") && compData.properties.at("position").is_object()) {
                tc.position.x = compData.properties.at("position").value("x", tc.position.x);
                tc.position.y = compData.properties.at("position").value("y", tc.position.y);
            }
            tc.rotation = compData.properties.value("rotation", tc.rotation);
            if (compData.properties.contains("scale") && compData.properties.at("scale").is_object()) {
                tc.scale.x = compData.properties.at("scale").value("x", tc.scale.x);
                tc.scale.y = compData.properties.at("scale").value("y", tc.scale.y);
            }
        }
        // Add more component types here
        else if (compData.name == "SpriteComponent") {
            // SpriteComponent is typically added based on textureAlias directly,
            // but this allows overriding properties like z_order or initial sourceRect from JSON.
            auto& sprite = m_registry.get_or_emplace<SpriteComponent>(entity);
            if (compData.properties.contains("sourceRect") && compData.properties.at("sourceRect").is_object()) {
                sprite.sourceRect.x = compData.properties.at("sourceRect").value("x", sprite.sourceRect.x);
                sprite.sourceRect.y = compData.properties.at("sourceRect").value("y", sprite.sourceRect.y);
                sprite.sourceRect.w = compData.properties.at("sourceRect").value("w", sprite.sourceRect.w);
                sprite.sourceRect.h = compData.properties.at("sourceRect").value("h", sprite.sourceRect.h);
            }
            sprite.z_order = compData.properties.value("z_order", sprite.z_order);
            sprite.visible = compData.properties.value("visible", sprite.visible);
            // Pivot and flip could also be configured here if needed.
        } else {
            std::cerr << "EntityFactory: Unknown component type '" << compData.name << "' in definition for entity " << static_cast<uint32_t>(entity) << std::endl;
        }
    }
}


entt::entity EntityFactory::CreateCharacter(const std::string& characterAlias, const Vector2D& position) {
    Content::CharacterDefinition* def = m_resourceManager.GetCharacterDefinition(characterAlias);
    if (!def) {
        std::cerr << "EntityFactory: CharacterDefinition not found for alias: " << characterAlias << std::endl;
        return entt::null;
    }

    entt::entity entity = m_registry.create();
    
    // Always add TransformComponent first and set its initial position and scale from definition
    auto& transform = m_registry.emplace<TransformComponent>(entity);
    transform.position = position;
    transform.scale = def->initialScale; 

    // Apply components from definition's list
    ApplyComponentData(entity, def->componentsToAdd);
    
    // Add SpriteComponent using def->textureAlias
    if (!def->textureAlias.empty()) {
        SDL_Texture* texture = m_resourceManager.GetTexture(def->textureAlias);
        if (texture) {
            auto& sprite = m_registry.get_or_emplace<SpriteComponent>(entity); // Use get_or_emplace in case ComponentData added it
            sprite.texture = texture;
            // Set sourceRect to full texture only if it wasn't set by ComponentData
            if (sprite.sourceRect.w == 0 && sprite.sourceRect.h == 0) { 
                int w, h;
                SDL_QueryTexture(texture, nullptr, nullptr, &w, &h); // Use nullptr instead of NULL
                sprite.sourceRect = {0, 0, w, h};
            }
        } else {
            std::cerr << "EntityFactory: Texture not found via ResourceManager for alias: " << def->textureAlias << " for character " << characterAlias << std::endl;
        }
    }

    std::cout << "EntityFactory: Created character '" << def->friendlyName << "' (Alias: " << characterAlias << ", EntityID: " << static_cast<uint32_t>(entity) << ")" << std::endl;
    return entity;
}

entt::entity EntityFactory::CreateItem(const std::string& itemAlias, const Vector2D& position) {
    Content::ItemDefinition* def = m_resourceManager.GetItemDefinition(itemAlias);
    if (!def) {
        std::cerr << "EntityFactory: ItemDefinition not found for alias: " << itemAlias << std::endl;
        return entt::null;
    }

    entt::entity entity = m_registry.create();
    m_registry.emplace<TransformComponent>(entity, position); // Default rotation and scale

    // Apply components from definition's list
    ApplyComponentData(entity, def->componentsToAdd); // Using the same ApplyComponentData for now

    // Add SpriteComponent using def->textureAlias
    if (!def->textureAlias.empty()) {
        SDL_Texture* texture = m_resourceManager.GetTexture(def->textureAlias);
        if (texture) {
            auto& sprite = m_registry.get_or_emplace<SpriteComponent>(entity); // Use get_or_emplace
            sprite.texture = texture;
            // Set sourceRect to full texture only if it wasn't set by ComponentData
            if (sprite.sourceRect.w == 0 && sprite.sourceRect.h == 0) {
                int w, h;
                SDL_QueryTexture(texture, nullptr, nullptr, &w, &h); // Use nullptr
                sprite.sourceRect = {0, 0, w, h};
            }
        } else {
            std::cerr << "EntityFactory: Texture not found via ResourceManager for alias: " << def->textureAlias << " for item " << itemAlias << std::endl;
        }
    }

    std::cout << "EntityFactory: Created item '" << def->friendlyName << "' (Alias: " << itemAlias << ", EntityID: " << static_cast<uint32_t>(entity) << ")" << std::endl;
    return entity;
}
