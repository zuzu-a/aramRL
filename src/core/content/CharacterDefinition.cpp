#include "inc/core/content/CharacterDefinition.hpp" // Adjust path
#include <iostream> // For std::cerr in case of error

namespace Content {
    bool CharacterDefinition::FromJson(const nlohmann::json& j, CharacterDefinition& def) {
        try {
            def.id = j.at("id").get<std::string>();
            def.friendlyName = j.value("friendlyName", def.id); // Default to id if not present
            def.textureAlias = j.at("textureAlias").get<std::string>();
            def.speed = j.value("speed", 100.0f);
            def.health = j.value("health", 100);
            
            if (j.contains("initialScale") && j.at("initialScale").is_object()) {
                def.initialScale.x = j.at("initialScale").value("x", 1.0f);
                def.initialScale.y = j.at("initialScale").value("y", 1.0f);
            } else {
                // Default if not present or not an object
                def.initialScale = {1.0f, 1.0f};
            }

            if (j.contains("componentsToAdd") && j.at("componentsToAdd").is_array()) {
                for (const auto& compEntry : j.at("componentsToAdd")) {
                    if (!compEntry.is_object() || !compEntry.contains("name") || !compEntry.contains("properties")) {
                        // std::cerr << "Warning: Skipping malformed componentToAdd entry in CharacterDefinition: " << def.id << std::endl;
                        continue;
                    }
                    ComponentData cd;
                    cd.name = compEntry.at("name").get<std::string>();
                    cd.properties = compEntry.at("properties"); // Keep as json object
                    def.componentsToAdd.push_back(cd);
                }
            }
        } catch (const nlohmann::json::exception& e) {
            // std::cerr << "Error parsing CharacterDefinition JSON for id '" << def.id << "': " << e.what() << std::endl;
            return false;
        }
        return true;
    }
} // namespace Content
