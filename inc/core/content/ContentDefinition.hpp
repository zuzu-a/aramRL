#ifndef CONTENT_DEFINITION_HPP
#define CONTENT_DEFINITION_HPP

#include <string>
#include "ContentType.hpp" // Points to the new ContentType.hpp

namespace Content {
    struct Definition {
        std::string id; // Unique identifier for this content piece (e.g., "player_hero", "iron_sword")
        Type type = Type::UNKNOWN;
        std::string sourceFile; // The file this definition was loaded from (for debugging/editing)

        virtual ~Definition() = default; // Important for potential polymorphism if derived classes have members needing cleanup
    };
} // namespace Content

#endif // CONTENT_DEFINITION_HPP
