#pragma once

#include <assert.h>
#include <corecrt_math.h>

#include "QF_Math.h"

// #TODO More operations: http://higherorderfun.com/blog/2012/06/03/math-for-game-programmers-05-vector-cheat-sheet/

namespace QwerkE {

    struct Vector3f final
    {
        // Member fields and aliases
        union {
            struct { // 3D Position
                float x, y, z; // #NOTE Default member initialization not allowed in anonymous structs inside of unions
            };
            struct { // Texture coordinates
                float u, v, w;
            };
            struct { // Alternative texture coordinates
                float s, t, p;
            };
            struct { // Rotation
                float pitch, yaw, roll;
            };
            struct { // Colour
                float r, g, b;
            };
            struct { // Local frames
                float tangent, bitangent, normal;
            };
            // Barycentric coordinates: float a, b, c;
        };

        // Constructors
        inline Vector3f() : x(0.f), y(0.f), z(0.f) {};
        inline Vector3f(const float other) : x(other), y(other), z(other) {}
        inline Vector3f(const float otherX, const float otherY, const float otherZ) : x(otherX), y(otherY), z(otherZ) {}
        inline Vector3f(const float other[3]) : x(other[0]), y(other[1]), z(other[2]) {}
        // #TODO Circular include: inline Vector3f(const Vector3f& other) : x(other.x), y(other.y), z(other.z) { }
        // #TODO Circular include: inline Vector3f(const float otherX, const Vector2f& otherYZ) : x(otherX), y(otherYZ.x), z(otherYZ.y) { }
        // #TODO Circular include: inline Vector3f(const Vector2f& otherXY, const float otherZ) : x(otherXY.x), y(otherXY.y), z(otherZ) { }
        // #TODO Circular include: inline Vector3f(const Vector4f& other) : x(other.x), y(other.y), z(other.z) { }

        // Duplicate
        inline Vector3f Copy() { return Vector3f(x, y, z); }

        // Constants
        static const Vector3f Right() { return Vector3f(1.0f, 0.0f, 0.0f); }
        static const Vector3f Left() { return Vector3f(-1.0f, 0.0f, 0.0f); }
        static const Vector3f Up() { return Vector3f(0.0f, 1.0f, 0.0f); }
        static const Vector3f Down() { return Vector3f(0.0f, -1.0f, 0.0f); }
        static const Vector3f Forward() { return Vector3f(0.0f, 0.0f, 1.0f); }
        static const Vector3f Backward() { return Vector3f(0.0f, 0.0f, -1.0f); }

        static const Vector3f One() { return Vector3f(1.0f, 1.0f, 1.0f); }
        static const Vector3f Zero() { return Vector3f(0.0f, 0.0f, 0.0f); }

        // #TODO static const Vector3f UnitX;
        // #TODO static const Vector3f UnitY;
        // #TODO static const Vector3f UnitZ;

        // Composition
        // #TODO Circular include: inline Vector4f Vec4() { return Vector4f(x, y, z, 0.0f); }

        // Decomposition
        // #TODO Circular include: inline Vector2f XY() { return Vector2f(x, y); }
        // #TODO Circular include: inline Vector2f XZ() { return Vector2f(x, z); }
        // #TODO Circular include: inline Vector2f YZ() { return Vector2f(y, z); }

        // Single component replacement
        // #TODO Naming: "DiffX", "CopyWithX", "DupX" ...
        inline Vector3f NewX(const float other) { return Vector3f(other, y, z); }
        inline Vector3f NewY(const float other) { return Vector3f(x, other, z); }
        inline Vector3f NewZ(const float other) { return Vector3f(x, y, other); }

        // Double component replacement
        inline Vector3f NewXY(const float otherX, const float otherY) { return Vector3f(otherX, otherY, z); }
        inline Vector3f NewXZ(const float otherX, const float otherZ) { return Vector3f(otherX, y, otherZ); }
        inline Vector3f NewYZ(const float otherY, const float otherZ) { return Vector3f(x, otherY, otherZ); }

        // Setters
        inline void Set(const float other) { x = other; y = other; z = other; }
        inline void Set(const float otherX, const float otherY, const float otherZ) { x = otherX; y = otherY; z = otherZ; }
        // #TODO Circular include: inline void Set(const float otherX, const Vector2f& otherYZ) { x = otherX; y = otherYZ.x; z = otherYZ.y; }
        // #TODO Circular include: inline void Set(const Vector2f& otherXY, const float otherZ) { x = otherXY.x; y = otherXY.y; z = otherZ; }
        inline void Set(const Vector3f& other) { x = other.x; y = other.y; z = other.z; }
        // #TODO Circular include: inline void Set(const Vector4f& other) { x = other.x; y = other.y; z = other.z; }

        inline void SetOne() { x = 1.f; y = 1.f; z = 1.f; }
        inline void SetZero() { x = 0.f; y = 0.f; z = 0.f; }

        // Moving component values
        inline void SwapXY() { const float temp = x; x = y; y = temp; }
        inline void SwapXZ() { const float temp = x; x = z; z = temp; }
        inline void SwapYZ() { const float temp = y; y = z; z = temp; }

        inline void CycleRight() { const float tempX = x; x = z; z = y; y = tempX; }
        inline void CycleLeft() { const float tempX = x; x = y; y = z; z = tempX; }

        // #TODO Decide on other clamp methods
        inline void ClampUnit() { Clamp(-1.f, 1.f); }
        inline void ClampPositive() { Clamp(0.f, FLT_MAX); }
        inline void ClampPositiveUnit() { Clamp(-1.f, 1.f); }
        inline void Clamp(const float min = -1.f, const float max = 1.f) { x = x<min ? min : x>max ? max : x; y = y<min ? min : y>max ? max : y; z = z<min ? min : z>max ? max : z; }
        // #TODO Clamp(const Vector3f& min = -1.f, const Vector3f& max = 1.f) { ... }

        // #TODO Identity?
        inline void Absolute() { if (x < 0) x *= -1.f; if (y < 0) y *= -1.f; if (z < 0) z *= -1.f; }
        inline void Unit() { Clamp(-1.f, 1.f); }
        inline void Saturate() { Clamp(0.f, 1.f); }

        // Producing operations
        inline Vector3f Abs() { return Vector3f(x > 0 ? x : x * -1.f, y > 0 ? y : y * -1.f, z > 0 ? z : z * -1.f); }

        // #TODO Length?
        inline float Magnitude() const { return std::sqrtf(MagnitudeSquared()); }
        inline float MagnitudeSquared() const { return x * x + y * y + z * z; }
        inline Vector3f Normalized() const { Vector3f other(*this); other.Normalize(); return other; }
        inline void Normalize() { if (const float magnitude = Magnitude(); magnitude != 0.0f) Set(x / magnitude, y / magnitude, z / magnitude); else Set(0.0f, 0.0f, 0.0f); }

        inline float Dot(const Vector3f& other) const { return x * other.x + y * other.y + z * other.z; }
        inline static float Dot(const Vector3f& lhs, const Vector3f& rhs) { return lhs.Dot(rhs); }

        inline Vector3f Cross(const Vector3f& other) const { return Vector3f(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x); }

        inline Vector3f Lerp(const Vector3f& other, float normal) const { return *this + (other - *this) * normal; }

        // #TODO Look at Unity's MoveTowards()

        // Redirection operations
        inline void Rotate(const Vector3f& normalizedAxis, float degrees) {
            if (!Math::Equal(normalizedAxis.Magnitude(), 0.f))
            {
                const float angleRadians = degrees * Math::ToRad;
                const float cosTheta = std::cos(angleRadians);
                *this = *this * cosTheta + normalizedAxis.Cross(*this) * std::sin(angleRadians) + normalizedAxis * (normalizedAxis.Dot(*this)) * (1.0f - cosTheta);
            }
        }
        inline static Vector3f Rotation(const Vector3f& other, const Vector3f& normalizedAxis, float degrees) { Vector3f newVec(other); newVec.Rotate(normalizedAxis, degrees); return newVec; }

        inline Vector3f Project(const Vector3f& other) const { if (const float otherMagnitudeSquared = other.MagnitudeSquared(); otherMagnitudeSquared != 0.0f) return other * Dot(other) / otherMagnitudeSquared; return Vector3f(); }

        inline Vector3f Reflect(const Vector3f& normal) const { return *this - normal * (2.0f * this->Dot(normal)); }
        // #TODO Review Unity's Vector3.Reflect(inDirection, inNormal)

        Vector3f Refract(const Vector3f& normal, const float etaRatio) const
        {
            // #TODO Review
            Vector3f I = this->Normalized();
            Vector3f N = normal.Normalized();
            float cosi = -std::max(-1.0f, std::min(1.0f, I.Dot(N)));
            float sint2 = etaRatio * etaRatio * (1.0f - cosi * cosi);

            if (sint2 > 1.0f) {
                return *this;
            }

            float cost = std::sqrt(1.0f - sint2);
            Vector3f refracted = I * etaRatio + N * (etaRatio * cosi - cost);
            return refracted;
        }

        // #TODO Vector3f ProjectOnto(const Vector3& other) const;
        // #TODO Vector3f RejectFrom(const Vector3& other) const;

        // Arithmetic operations
        // #TODO Consider changing to "Plus", "Minus", "Times", "Divides".
        // I just want to make it super clear somehow that these operations don't modify, and are for return values.
        // For example, instead of needing to do Func(myVector3f + Vector3f(0, 1, 0)), we can also do Func(myVector3f.Add(0, 1, 0)) to shorten frequent operations
        // "Shifted", "Plussed", "With", "Added"
        // Or remove arithmetic methods and force using operators
        inline Vector3f Add(const float other)          const { return Vector3f(x + other, y + other, z + other); }
        inline Vector3f Add(const Vector3f& other)      const { return Vector3f(x + other.x, y + other.y, z + other.z); }

        inline Vector3f Subtract(const float other)     const { return Vector3f(x - other, y - other, z - other); }
        inline Vector3f Subtract(const Vector3f& other) const { return Vector3f(x - other.x, y - other.y, z - other.z); }

        inline Vector3f Multiply(const float other)     const { return Vector3f(x * other, y * other, z * other); }
        inline Vector3f Multiply(const Vector3f& other) const { return Vector3f(x * other.x, y * other.y, z * other.z); }

        inline Vector3f Divide(const float other)       const { return Vector3f(x / other, y / other, z / other); }
        inline Vector3f Divide(const Vector3f& other)   const { return Vector3f(x / other.x, y / other.y, z / other.z); }

        // Intra-comparison operations
        // #TODO Is* methods
        // bool IsZero(float epsilon = 1e-6f) const;
        // bool IsNormalized(float epsilon = 1e-6f) const;

        // Inter-comparison operations
        inline float Angle(const Vector3f& other) const { return AngleRadians(other) * Math::ToRad; }
        inline float AngleRadians(const Vector3f& other) const {
            const float cosine = Dot(other) / (this->Magnitude() * other.Magnitude());
            return Math::Clamp(cosine, -1.f, 1.f);
        };

        // #TODO DistanceSquared(const Vector3f& other) const { return Vector3f(x - other.x, y - other.y, z - other.z).MagnitudeSquared(); }
        inline float Distance(const Vector3f& other) const { return Vector3f(x - other.x, y - other.y, z - other.z).Magnitude(); }
        // #TODO Circular include: inline float DistanceXY(const Vector3f& other) const { return Vector2f(x - other.x, y - other.y).Magnitude(); }
        // #TODO Circular include: inline float DistanceXZ(const Vector3f& other) const { return Vector2f(x - other.x, z - other.z).Magnitude(); }

        // #TODO Can structured bindings, auto deduced types, or some other trick help here?
        // #TODO Circular include: inline float DistanceXY(const Vector2f& other) const { return Vector2f(x - other.x, y - other.y).Magnitude(); }
        // #TODO Circular include: inline float DistanceXZ(const Vector2f& other) const { return Vector2f(x - other.x, z - other.y).Magnitude(); }

        // bool ShorterThan(Vector3f), (float)
        // bool LongerThan(Vector3f), (float)
        // bool CloserThan(point, other)
        // Vector3f .Closer(a, b) return a or b;
        // bool ExactlyEquals()

        // #TODO Choose between Math::Equals for everything, or this internal method
        inline bool Equals(const Vector3f& other) const { return x == other.x && y == other.y && z == other.z; }
        inline bool EqualsEpsilon(const Vector3f& other, float epsilon = 1e-4f) const { Vector3f abs = Vector3f(*this - other); abs.Absolute(); return abs.x < epsilon && abs.y < epsilon && abs.z < epsilon; }

        // Access operators
        inline operator float* () { return &x; }
        inline operator const float* () const { return &x; }

        // #NOTE Pointers like vec3* need to be dereferenced as vec3& references in order to access as vec3[i], or construction will occur
        inline float& operator [](size_t i) { assert(i >= 0 && i < 3); return *(&x + i); }
        inline const float& operator [](size_t i) const { assert(i >= 0 && i < 3); return *(&x + i); }

        // Comparison operators
        inline bool operator ==(const Vector3f& other) const { return EqualsEpsilon(other); }
        inline bool operator !=(const Vector3f& other) const { return !(*this == other); }

        // Altering operators
        inline Vector3f operator =(const Vector3f& other) { Set(other); return *this; }
        inline Vector3f operator -() const { return Vector3f(-x, -y, -z); }

        inline Vector3f operator +=(const float other) { x += other; y += other; z += other; return *this; }
        inline Vector3f operator +=(const Vector3f& other) { x += other.x; y += other.y; z += other.z; return *this; }

        inline Vector3f operator -=(const float other) { x -= other; y -= other; z -= other; return *this; }
        inline Vector3f operator -=(const Vector3f& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }

        inline Vector3f operator *=(const float other) { x *= other; y *= other; z *= other; return *this; }
        inline Vector3f operator *=(const Vector3f& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }

        inline Vector3f operator /=(const float other) { x /= other; y /= other; z /= other; return *this; }
        inline Vector3f operator /=(const Vector3f& other) { x /= other.x; y /= other.y; z /= other.z; return *this; }

        // Producing, non-altering operators
        inline Vector3f operator +(const float other) const { return Vector3f(x + other, y + other, z + other); }
        inline Vector3f operator +(const Vector3f& other) const { return Vector3f(x + other.x, y + other.y, z + other.z); }

        inline Vector3f operator -(const float other) const { return Vector3f(x - other, y - other, z - other); }
        inline Vector3f operator -(const Vector3f& other) const { return Vector3f(x - other.x, y - other.y, z - other.z); }

        inline Vector3f operator *(const float other) const { return Vector3f(x * other, y * other, z * other); }
        inline Vector3f operator *(const Vector3f& other) const { return Vector3f(x * other.x, y * other.y, z * other.z); }

        inline Vector3f operator /(const float other) const { return Vector3f(x / other, y / other, z / other); }
        inline Vector3f operator /(const Vector3f& other) const { return Vector3f(x / other.x, y / other.y, z / other.z); }

        // #TODO Consider overloading all methods support Vector4f& arguments and treat them as Vector3f, ignoring w component so no conversions to Vector3f are needed
    };

    // #TODO Explore more operators

    // Right hand side operators
    inline Vector3f operator +(const float otherFloat, const Vector3f& otherVector3f) { return Vector3f(otherFloat + otherVector3f.x, otherFloat + otherVector3f.y, otherFloat + otherVector3f.z); }
    inline Vector3f operator -(const float otherFloat, const Vector3f& otherVector3f) { return Vector3f(otherFloat - otherVector3f.x, otherFloat - otherVector3f.y, otherFloat - otherVector3f.z); }
    inline Vector3f operator *(const float otherFloat, const Vector3f& otherVector3f) { return Vector3f(otherFloat * otherVector3f.x, otherFloat * otherVector3f.y, otherFloat * otherVector3f.z); }
    inline Vector3f operator /(const float otherFloat, const Vector3f& otherVector3f) { return Vector3f(otherFloat / otherVector3f.x, otherFloat / otherVector3f.y, otherFloat / otherVector3f.z); }

}
