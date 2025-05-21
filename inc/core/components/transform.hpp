#ifndef TRANSFORM_COMPONENT_HPP
#define TRANSFORM_COMPONENT_HPP

#include "vector2d.hpp" // Assuming it's in the same directory

struct TransformComponent {
    Vector2D position = {0.0f, 0.0f};
    float rotation = 0.0f;    // In degrees for simplicity, or radians if preferred
    Vector2D scale = {1.0f, 1.0f};

    TransformComponent() = default;
    TransformComponent(const Vector2D& pos, float rot = 0.0f, const Vector2D& scl = {1.0f, 1.0f})
        : position(pos), rotation(rot), scale(scl) {}
};

#endif // TRANSFORM_COMPONENT_HPP
