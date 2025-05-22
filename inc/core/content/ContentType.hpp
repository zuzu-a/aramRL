#ifndef CONTENT_TYPE_HPP
#define CONTENT_TYPE_HPP

namespace Content {
    enum class Type {
        UNKNOWN,
        SETPIECE, // Example for future use
        MAP_DEFINITION, // Example for future use
        ITEM_DEFINITION,
        MONSTER_DEFINITION, // Using this for "enemies"
        CHARACTER_DEFINITION,
        CLASS_DEFINITION, // Example for future use
        TEXTURE,    // Managed by ResourceManager, but definitions might refer to them
        SOUND,      // Future
        FONT,       // Future
        TILESET_DEFINITION, // Data that points to a tileset JSON and its texture
        // Add more as needed
    };
} // namespace Content

#endif // CONTENT_TYPE_HPP
