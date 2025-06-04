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

bool AABBvsAABB(Manifold* m) {
    Object* A = m->A;
    Object* B = m->B;
    Vec2 n = B->pos - A->pos;

    AABB abox = A->aabb;
    AABB bbox = B->aabb;

    float a_extent_x = (abox.max.x - abox.min.x) / 2;
    float b_extent_x = (bbox.max.x - bbox.min.x) / 2;
    float x_overlap = a_extent_x + b_extent_x - std::abs(n.x);

    if (x_overlap > 0) {
        float a_extent_y = (abox.max.y - abox.min.y) / 2;
        float b_extent_y = (bbox.max.y - bbox.min.y) / 2;
        float y_overlap = a_extent_y + b_extent_y - std::abs(n.y);

        if (y_overlap > 0) {
            if (x_overlap < y_overlap) {
                m->normal = (n.x < 0) ? Vec2(-1, 0) : Vec2(1, 0);
                m->penetration = x_overlap;
            } else {
                m->normal = (n.y < 0) ? Vec2(0, -1) : Vec2(0, 1);
                m->penetration = y_overlap;
            }
            return true;
        }
    }

    return false;
}

bool AABBvsCircle(Manifold* m) {
    Object* A = m->A; // AABB
    Object* B = m->B; // Circle

    Vec2 n = B->pos - A->pos;

    float x_extent = (A->aabb.max.x - A->aabb.min.x) / 2;
    float y_extent = (A->aabb.max.y - A->aabb.min.y) / 2;

    Vec2 closest = n;
    closest.x = Clamp(-x_extent, x_extent, closest.x);
    closest.y = Clamp(-y_extent, y_extent, closest.y);

    bool inside = false;
    if (n == closest) {
        inside = true;

        if (std::abs(n.x) > std::abs(n.y)) {
            closest.x = (closest.x > 0) ? x_extent : -x_extent;
        } else {
            closest.y = (closest.y > 0) ? y_extent : -y_extent;
        }
    }

    Vec2 normal = n - closest;
    float d2 = normal.LengthSquared();
    float r = B->radius;

    if (d2 > r * r && !inside)
        return false;

    float d = std::sqrt(d2);

    if (inside) {
        m->normal = -normal / d;
        m->penetration = r - d;
    } else {
        m->normal = normal / d;
        m->penetration = r - d;
    }

    return true;
}

int main() {
    Object circle1 = { Vec2(0, 0), 5.0f };
    Object circle2 = { Vec2(6, 0), 5.0f };

    Manifold m1 = { &circle1, &circle2 };
    if (CirclevsCircle(&m1)) {
        std::cout << "Circle vs Circle Collision!\n";
        std::cout << "Normal: (" << m1.normal.x << ", " << m1.normal.y << ")\n";
        std::cout << "Penetration: " << m1.penetration << "\n";
    }

    Object box1 = { Vec2(0, 0), 0.0f, { Vec2(-2, -2), Vec2(2, 2) } };
    Object box2 = { Vec2(3, 0), 0.0f, { Vec2(-2, -2), Vec2(2, 2) } };

    Manifold m2 = { &box1, &box2 };
    if (AABBvsAABB(&m2)) {
        std::cout << "AABB vs AABB Collision!\n";
        std::cout << "Normal: (" << m2.normal.x << ", " << m2.normal.y << ")\n";
        std::cout << "Penetration: " << m2.penetration << "\n";
    }

    Object box3 = { Vec2(0, 0), 0.0f, { Vec2(-3, -3), Vec2(3, 3) } };
    Object circle3 = { Vec2(5, 0), 2.0f };

    Manifold m3 = { &box3, &circle3 };
    if (AABBvsCircle(&m3)) {
        std::cout << "AABB vs Circle Collision!\n";
        std::cout << "Normal: (" << m3.normal.x << ", " << m3.normal.y << ")\n";
        std::cout << "Penetration: " << m3.penetration << "\n";
    }

    return 0;
}


