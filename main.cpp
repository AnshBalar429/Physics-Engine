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

bool CirclevsCircle(Manifold* m) {
    Object* A = m->A;
    Object* B = m->B;

    Vec2 n = B->pos - A->pos;
    float r = A->radius + B->radius;
    float r2 = r * r;

    if (n.LengthSquared() > r2)
        return false;

    float d = n.Length();

    if (d != 0) {
        m->penetration = r - d;
        m->normal = n / d;
    } else {
        m->penetration = A->radius;
        m->normal = Vec2(1, 0);  // arbitrary
    }

    return true;
}




