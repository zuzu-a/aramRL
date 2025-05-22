#ifndef ITEM_DEFINITION_HPP
#define ITEM_DEFINITION_HPP

#include "ContentDefinition.hpp"
#include "CharacterDefinition.hpp" // For ComponentData, as per instruction
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

namespace Content {
    // Using ComponentData from CharacterDefinition.hpp as instructed

    struct ItemDefinition : public Definition {
        std::string friendlyName;
        std::string description;
        std::string textureAlias; // Alias for the texture in ResourceManager
        // Add other item-specific properties, e.g., stackable, value, effects
        std::vector<ComponentData> componentsToAdd;


        ItemDefinition() { type = Type::ITEM_DEFINITION; }
        static bool FromJson(const nlohmann::json& j, ItemDefinition& def);
    };
} // namespace Content

#endif // ITEM_DEFINITION_HPP
