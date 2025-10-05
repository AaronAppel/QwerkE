#pragma once

#include <iostream>

#include "QF_Math.h"

#define VECTOR4_SIMD 1

#if defined(VECTOR4_SIMD) && VECTOR4_SIMD
#include <xmmintrin.h>   // SSE
#include <smmintrin.h>   // SSE4.1 for _mm_dp_ps, _mm_blendv_ps
#include <cmath>
#include <cassert>
#endif // VECTOR4_SIMD

namespace QwerkE {

    struct Vector4f final
    {
#if !defined(VECTOR4_SIMD) || !VECTOR4_SIMD
        Vector4f() : x(0.f), y(0.f), z(0.f), w(0.f) {}
        Vector4f(float otherX, float otherY, float otherZ, float otherW) : x(otherX), y(otherY), z(otherZ), w(otherW) {}
#endif // !VECTOR4_SIMD

        inline Vector4f Lerp(const Vector4f& other, float normal) const { return *this + (other - *this) * normal; }

        // Decomposition
        // inline Vector2f XY() { return Vector2f(x, y); }
        // inline Vector2f XZ() { return Vector2f(x, z); }
        // inline Vector2f XW() { return Vector2f(x, w); }
        //
        // inline Vector2f YZ() { return Vector2f(y, z); }
        // inline Vector2f YW() { return Vector2f(y, w); }
        //
        // inline Vector2f ZW() { return Vector2f(z, w); }

        // #TODO Circular include dependency
        // inline Vector3f XYZ() { return Vector3f(x, y, z); }
        // inline Vector3f XYW() { return Vector3f(x, y, w); }
        // inline Vector3f XZW() { return Vector3f(x, z, w); }

        // inline Vector3f YZW() { return Vector3f(y, z, w); }

        // SIMD operations
#if defined(VECTOR4_SIMD) && VECTOR4_SIMD
    // Constructors
        Vector4f() : simd(_mm_setzero_ps()) {}
        Vector4f(float x, float y, float z, float w) : simd(_mm_set_ps(w, z, y, x)) {}
        Vector4f(float value) : simd(_mm_set1_ps(value)) {}
        Vector4f(__m128 v) : simd(v) {}

        // Access
        float operator[](int i) const {
            assert(i >= 0 && i < 4);
            return reinterpret_cast<const float*>(&simd)[i];
        }

        float& operator[](int i) {
            assert(i >= 0 && i < 4);
            return reinterpret_cast<float*>(&simd)[i];
        }

        // Arithmetic
        Vector4f operator+(const Vector4f& rhs) const { return _mm_add_ps(simd, rhs.simd); }
        Vector4f operator-(const Vector4f& rhs) const { return _mm_sub_ps(simd, rhs.simd); }
        Vector4f operator*(const Vector4f& rhs) const { return _mm_mul_ps(simd, rhs.simd); }
        Vector4f operator/(const Vector4f& rhs) const { return _mm_div_ps(simd, rhs.simd); }

        Vector4f operator*(float scalar) const { return _mm_mul_ps(simd, _mm_set1_ps(scalar)); }
        Vector4f operator/(float scalar) const { return _mm_div_ps(simd, _mm_set1_ps(scalar)); }

        // Compound assignment
        Vector4f& operator+=(const Vector4f& rhs) { simd = _mm_add_ps(simd, rhs.simd); return *this; }
        Vector4f& operator-=(const Vector4f& rhs) { simd = _mm_sub_ps(simd, rhs.simd); return *this; }
        Vector4f& operator*=(const Vector4f& rhs) { simd = _mm_mul_ps(simd, rhs.simd); return *this; }
        Vector4f& operator/=(const Vector4f& rhs) { simd = _mm_div_ps(simd, rhs.simd); return *this; }

        // Dot product
        float dot(const Vector4f& rhs) const {
            __m128 dp = _mm_dp_ps(simd, rhs.simd, 0xF1); // dot all, result in lowest float
            return _mm_cvtss_f32(dp);
        }

        // Length
        float lengthSquared() const { return dot(*this); }

        float length() const {
            return std::sqrt(lengthSquared());
        }

        // Normalization
        Vector4f normalized() const {
            __m128 dp = _mm_dp_ps(simd, simd, 0xFF);
            __m128 len = _mm_sqrt_ps(dp);
            return _mm_div_ps(simd, len);
        }

        // Min / Max / Clamp
        Vector4f min(const Vector4f& rhs) const { return _mm_min_ps(simd, rhs.simd); }
        Vector4f max(const Vector4f& rhs) const { return _mm_max_ps(simd, rhs.simd); }

        Vector4f clamp(const Vector4f& minVal, const Vector4f& maxVal) const {
            return _mm_min_ps(_mm_max_ps(simd, minVal.simd), maxVal.simd);
        }

        // Lerp
        Vector4f lerp(const Vector4f& to, float t) const {
            __m128 tVec = _mm_set1_ps(t);
            return _mm_add_ps(simd, _mm_mul_ps(_mm_sub_ps(to.simd, simd), tVec));
        }

        // Comparison (return masks)
        Vector4f cmpEQ(const Vector4f& rhs) const { return _mm_cmpeq_ps(simd, rhs.simd); }
        Vector4f cmpNEQ(const Vector4f& rhs) const { return _mm_cmpneq_ps(simd, rhs.simd); }
        Vector4f cmpGT(const Vector4f& rhs) const { return _mm_cmpgt_ps(simd, rhs.simd); }
        Vector4f cmpLT(const Vector4f& rhs) const { return _mm_cmplt_ps(simd, rhs.simd); }

        // Select (blend)
        static Vector4f select(const Vector4f& a, const Vector4f& b, const Vector4f& mask) {
            return _mm_blendv_ps(a.simd, b.simd, mask.simd);
        }
#endif // VECTOR4_SIMD

        // Member fields and aliases
        union {
#if defined(VECTOR4_SIMD) && VECTOR4_SIMD
            __m128 simd;
#endif // VECTOR4_SIMD

            struct {
                float x, y, z, w; // #NOTE Default member initialization not allowed in anonymous structs inside of unions
            };
            struct {
                float u, v; // #NOTE 'w' is accessible but in the 4th element of Vector4f, or float[4]
            };
            struct {
                float pitch, yaw, roll;
            };
            struct {
                float r, g, b, a;
            };
        };

        // #ifdef VECTOR3_STL_EXTENSIONS
    };

}
