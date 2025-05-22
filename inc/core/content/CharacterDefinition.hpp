#ifndef CHARACTER_DEFINITION_HPP
#define CHARACTER_DEFINITION_HPP

#include "ContentDefinition.hpp"
#include "inc/core/components/vector2d.hpp" // For default scale if needed
#include <string>
#include <vector>
#include <nlohmann/json.hpp> // For parsing from JSON

namespace Content {
    struct ComponentData { // A generic way to specify component data from JSON
        std::string name;       // e.g., "TransformComponent"
        nlohmann::json properties; // JSON object with properties for this component
    };

    struct CharacterDefinition : public Definition {
        std::string friendlyName;
        std::string textureAlias; // Alias for the texture in ResourceManager
        // Example: Define initial stats or components directly
        float speed = 100.0f; 
        int health = 100;
        Vector2D initialScale = {1.0f, 1.0f};

        std::vector<ComponentData> componentsToAdd; // More flexible way

        CharacterDefinition() { type = Type::CHARACTER_DEFINITION; }

        // Helper to parse from JSON - static so it can be called before object creation
        static bool FromJson(const nlohmann::json& j, CharacterDefinition& def);
    };
} // namespace Content

#endif // CHARACTER_DEFINITION_HPP
