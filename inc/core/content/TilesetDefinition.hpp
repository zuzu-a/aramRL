#ifndef TILESET_DEFINITION_HPP
#define TILESET_DEFINITION_HPP

#include "ContentDefinition.hpp"
#include <string>
#include <nlohmann/json.hpp>

namespace Content {
    struct TilesetDefinition : public Definition {
        std::string jsonPath;     // Path to the tileset JSON file (e.g., "assets/tilesets/outdoor.json")
        std::string textureAlias; // Alias for the texture in ResourceManager (e.g., "outdoor_tilesheet")
                                  // The texture itself should be pre-loaded into RM using this alias.

        TilesetDefinition() { type = Type::TILESET_DEFINITION; }
        static bool FromJson(const nlohmann::json& j, TilesetDefinition& def);
    };
} // namespace Content

#endif // TILESET_DEFINITION_HPP
