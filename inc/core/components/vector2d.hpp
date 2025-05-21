#ifndef VECTOR2D_HPP
#define VECTOR2D_HPP

#include <cmath> // For sqrt

struct Vector2D {
    float x = 0.0f;
    float y = 0.0f;

    Vector2D() = default;
    Vector2D(float x_val, float y_val) : x(x_val), y(y_val) {}

    // Basic operations
    Vector2D operator+(const Vector2D& other) const { return Vector2D(x + other.x, y + other.y); }
    Vector2D operator-(const Vector2D& other) const { return Vector2D(x - other.x, y - other.y); }
    Vector2D operator*(float scalar) const { return Vector2D(x * scalar, y * scalar); }
    // Add more if needed: dot product, length, normalize etc.
    float Length() const { return std::sqrt(x * x + y * y); }
    Vector2D Normalized() const {
        float l = Length();
        if (l > 0) { return Vector2D(x / l, y / l); }
        return Vector2D(); // Or throw error for zero vector
    }
};

#endif // VECTOR2D_HPP
