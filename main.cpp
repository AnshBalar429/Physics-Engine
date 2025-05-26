#include <iostream>
#include <cmath>
#include <algorithm>

struct Vec2 {
    float x, y;

    Vec2() : x(0), y(0) {}
    Vec2(float x_, float y_) : x(x_), y(y_) {}

    Vec2 operator - (const Vec2& other) const {
        return Vec2(x - other.x, y - other.y);
    }

    Vec2 operator + (const Vec2& other) const {
        return Vec2(x + other.x, y + other.y);
    }

    Vec2 operator * (float scalar) const {
        return Vec2(x * scalar, y * scalar);
    }

    Vec2 operator / (float scalar) const {
        return Vec2(x / scalar, y / scalar);
    }

    Vec2 operator - () const {
        return Vec2(-x, -y);
    }

    float Length() const {
        return std::sqrt(x * x + y * y);
    }

    float LengthSquared() const {
        return x * x + y * y;
    }

    bool operator == (const Vec2& other) const {
        return x == other.x && y == other.y;
    }
};

float Clamp(float min, float max, float value) {
    return std::max(min, std::min(max, value));
}

struct AABB {
    Vec2 min;
    Vec2 max;
};

struct Object {
    Vec2 pos;
    float radius;  // For circle
    AABB aabb;     // For box
};

struct Manifold {
    Object* A;
    Object* B;
    Vec2 normal;
    float penetration;
};





