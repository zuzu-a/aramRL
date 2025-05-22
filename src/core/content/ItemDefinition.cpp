#include "inc/core/content/ItemDefinition.hpp" // Adjust path
#include "inc/core/content/CharacterDefinition.hpp" // For ComponentData
#include <iostream> // For std::cerr in case of error

namespace Content {
    bool ItemDefinition::FromJson(const nlohmann::json& j, ItemDefinition& def) {
        try {
            def.id = j.at("id").get<std::string>();
            def.friendlyName = j.value("friendlyName", def.id); // Default to id
            def.description = j.value("description", "");
            def.textureAlias = j.at("textureAlias").get<std::string>();

            if (j.contains("componentsToAdd") && j.at("componentsToAdd").is_array()) {
                for (const auto& compEntry : j.at("componentsToAdd")) {
                     if (!compEntry.is_object() || !compEntry.contains("name") || !compEntry.contains("properties")) {
                        // std::cerr << "Warning: Skipping malformed componentToAdd entry in ItemDefinition: " << def.id << std::endl;
                        continue;
                    }
                    ComponentData cd; // Assuming ComponentData is accessible via CharacterDefinition.hpp
                    cd.name = compEntry.at("name").get<std::string>();
                    cd.properties = compEntry.at("properties");
                    def.componentsToAdd.push_back(cd);
                }
            }
        } catch (const nlohmann::json::exception& e) {
            // std::cerr << "Error parsing ItemDefinition JSON for id '" << def.id << "': " << e.what() << std::endl;
            return false;
        }
        return true;
    }
} // namespace Content
