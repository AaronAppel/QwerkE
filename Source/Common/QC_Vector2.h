#pragma once

#include <assert.h>

#include "QC_Math.h"

// #TODO Resolve circular dependency. May need to use .cpp files
// #include "Vector3.h"
// #include "Vector4.h"

namespace QwerkE {

    // #TODO Template or support other types
    struct Vector2u16 final
    {
        Vector2u16(uint16_t _x, uint16_t _y) : x(_x), y(_y){ }

        // Member fields and aliases
        union {
            struct { // 2D Position
                uint16_t x, y; // #NOTE Default member initialization not allowed in anonymous structs inside of unions
            };
            struct { // Texture coordinates
                uint16_t u, v;
            };
            struct {
                // #TODO Rotating alias? float roll;
            };
        };
    };

    struct Vector2f final // #TODO Vec3 capital or not? Should types always be capitalized? Does anyone want the vec3 variable name available?
    {
        Vector2f() : x(0.f), y(0.f) {}
        Vector2f(const int otherX, const int otherY) : x(otherX), y(otherY) {}
        Vector2f(const float otherX, const float otherY) : x(otherX), y(otherY) {}
        Vector2f(const double otherX, const double otherY) : x(otherX), y(otherY) {}
        Vector2f(const float other[]) : x(other[0]), y(other[1]) {}
        // Vector2f(const Vector3f& other) : x(other.x), y(other.y) { }
        // Vector2f(const Vector4f& other) : x(other.x), y(other.y) {}

        inline float Magnitude() const { return std::sqrtf(x * x + y * y); }

        inline Vector2f Normalized() const { return *this / Magnitude(); }

        inline float Dot(const Vector2f& other) const { return x * other.x + y * other.y; }
        inline static float Dot(const Vector2f& lhs, const Vector2f& rhs) { return lhs.Dot(rhs); }

        inline Vector2f Lerp(const Vector2f& other, float normal) const { return *this + (other - *this) * normal; }

        // Producing, non-altering operators
        inline Vector2f operator +(const float other) const { return Vector2f(x + other, y + other); }
        inline Vector2f operator +(const Vector2f& other) const { return Vector2f(x + other.x, y + other.y); }

        inline Vector2f operator -(const float other) const { return Vector2f(x - other, y - other); }
        inline Vector2f operator -(const Vector2f& other) const { return Vector2f(x - other.x, y - other.y); }

        inline Vector2f operator *(const float other) const { return Vector2f(x * other, y * other); }
        inline Vector2f operator *(const Vector2f& other) const { return Vector2f(x * other.x, y * other.y); }

        inline Vector2f operator /(const float other) const { assert(other); return Vector2f(x / other, y / other); }
        inline Vector2f operator /(const Vector2f& other) const { assert(other.x); assert(other.y); return Vector2f(x / other.x, y / other.y); }

        // Member fields and aliases
        union {
            struct { // 2D Position
                float x, y; // #NOTE Default member initialization not allowed in anonymous structs inside of unions
            };
            struct { // Texture coordinates
                float u, v;
            };
            struct {
                // #TODO Rotating alias? float roll;
            };
        };
    };

    // inline Vector2f operator /(const float otherFloat, const Vector2f& otherVector3f) { return Vector2f(otherFloat / otherVector3f.x, otherFloat / otherVector3f.y); }

}
