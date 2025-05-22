#include "inc/core/content/TilesetDefinition.hpp" // Adjust path
#include <iostream> // For std::cerr in case of error

namespace Content {
    bool TilesetDefinition::FromJson(const nlohmann::json& j, TilesetDefinition& def) {
        try {
            def.id = j.at("id").get<std::string>(); // e.g., "outdoor_map_tiles"
            def.jsonPath = j.at("jsonPath").get<std::string>();
            def.textureAlias = j.at("textureAlias").get<std::string>();
        } catch (const nlohmann::json::exception& e) {
            // std::cerr << "Error parsing TilesetDefinition JSON for id '" 
            //           << (j.contains("id") ? j.at("id").get<std::string>() : "UNKNOWN") 
            //           << "': " << e.what() << std::endl;
            return false;
        }
        return true;
    }
} // namespace Content
