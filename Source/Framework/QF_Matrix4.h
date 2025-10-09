#pragma once

#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

#ifdef _QBGFX
#include <bx/math.h>
#endif

// #include "QF_Math.h"
// #include "QF_Vector3.h"

namespace QwerkE {

    // #NOTE Column-major layout
    struct Matrix4f final // #TODO Review naming mat4f instead of using type def for faster definition navigation
    {
        // #TODO Review naming. Can support single and double square brackets entry access [16], and [4][4]
        // Double [4][4] access may be preferred default, with .i[15] being required for array index access
        union {
            struct {
                float m[16];
            };
            struct {
                float cr[4][4];
            };
            struct {
                float column_row[4][4];
            };
            struct {
                float index[16];
            };
        };

        Matrix4f()
        {
            *this = Matrix4f::Identity();
        }

        Matrix4f(const float values[16])
        {
            memcpy(m, values, sizeof(float) * 16);
        }

        Matrix4f(const float value)
        {
            for (int i = 0; i < 16; ++i)
                m[i] = value;
        }

        Matrix4f(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33)
        {
            // #TODO Review if taking in row major, converting to column major?
            m[0] = m00; m[4] = m01; m[8] = m02; m[12] = m03;
            m[1] = m10; m[5] = m11; m[9] = m12; m[13] = m13;
            m[2] = m20; m[6] = m21; m[10] = m22; m[14] = m23;
            m[3] = m30; m[7] = m31; m[11] = m32; m[15] = m33;
            // #TODO or column-row indices?
            // cr[0][0] = m00; cr[1][0] = m01; m[8] = m02; m[12] = m03;
            // cr[1][0] = m10; cr[1][1] = m11; m[9] = m12; m[13] = m13;
            // cr[2][0] = m20; cr[1][2] = m21; m[10] = m22; m[14] = m23;
            // cr[3][0] = m30; cr[1][3] = m31; m[11] = m32; m[15] = m33;
        }

#ifdef _QBGFX
        // #TODO See if this conversion could be more "implicit" using overloading and move to vec3f file
        inline bx::Vec3 PositionBx() const { vec3f pos = Position(); return bx::Vec3(pos.x, pos.y, pos.z); }
        inline bx::Vec3 RotationBx() const { vec3f rot = Rotation(); return bx::Vec3(rot.x, rot.y, rot.z); }
        inline bx::Vec3 ScaleBx()	 const { vec3f scale = Scale();    return bx::Vec3(scale.x, scale.y, scale.z); }

        inline void SetPosition(const bx::Vec3& a_NewPosition) { SetPosition(a_NewPosition.x, a_NewPosition.y, a_NewPosition.z); }
#endif

        inline void SetPosition(const vec3f& newPosition) { m[12] = newPosition.x; m[13] = newPosition.y; m[14] = newPosition.z; }
        inline void SetPosition(const float newPositionX, const float newPositionY, const float newPositionZ) { m[12] = newPositionX; m[13] = newPositionY; m[14] = newPositionZ; }
        // #TODO inline void SetRotation() { ; }

        inline void Translate(const vec3f& a_Offset) { m[12] += a_Offset.x; m[13] += a_Offset.y; m[14] += a_Offset.z; }
        inline void Translate(const vec3f& a_Axis, const float a_Magnitude) { m[12] += a_Axis.x * a_Magnitude; m[13] += a_Axis.y * a_Magnitude; m[14] += a_Axis.z * a_Magnitude; }

        inline vec3f Position() const { return vec3f(m[12], m[13], m[14]); }

        inline vec3f Rotation() const
        {
            // Step 1: Extract scale
            float scaleX = std::sqrt(m[0] * m[0] + m[1] * m[1] + m[2] * m[2]);
            float scaleY = std::sqrt(m[4] * m[4] + m[5] * m[5] + m[6] * m[6]);
            float scaleZ = std::sqrt(m[8] * m[8] + m[9] * m[9] + m[10] * m[10]);

            // Step 2: Normalize the rotation part (upper 3x3) by removing scale
            float r00 = m[0] / scaleX;
            float r01 = m[4] / scaleY;
            float r02 = m[8] / scaleZ;

            float r10 = m[1] / scaleX;
            float r11 = m[5] / scaleY;
            float r12 = m[9] / scaleZ;

            float r20 = m[2] / scaleX;
            float r21 = m[6] / scaleY;
            float r22 = m[10] / scaleZ;

            // Step 3: Convert to Euler angles (Y-X-Z order: yaw, pitch, roll)
            vec3f returnRotation; // x=pitch, y=yaw, z=roll
            if (r20 < 1.0f) {
                if (r20 > -1.0f) {
                    returnRotation.x = std::asin(-r20);
                    returnRotation.y = std::atan2(r10, r00);
                    returnRotation.z = std::atan2(r21, r22);
                }
                else {
                    // r20 == -1
                    returnRotation.x = 3.1415927f / 2.f;
                    returnRotation.y = -std::atan2(-r12, r11);
                    returnRotation.z = 0.0f;
                }
            }
            else {
                // r20 == +1
                returnRotation.x = -3.1415927f / 2.0f;
                returnRotation.y = std::atan2(-r12, r11);
                returnRotation.z = 0.0f;
            }

            returnRotation.x *= 57.295776f;
            returnRotation.y *= 57.295776f;
            returnRotation.z *= 57.295776f;

            return returnRotation;
        }

        inline vec3f Scale() const
        {
            vec3f returnScale;

            // X axis scale = length of column 0 vector
            returnScale.x = std::sqrt(m[0] * m[0] +
                m[1] * m[1] +
                m[2] * m[2]);

            // Y axis scale = length of column 1 vector
            returnScale.y = std::sqrt(m[4] * m[4] +
                m[5] * m[5] +
                m[6] * m[6]);

            // Z axis scale = length of column 2 vector
            returnScale.z = std::sqrt(m[8] * m[8] +
                m[9] * m[9] +
                m[10] * m[10]);

            return returnScale;
        }

        // #NOTE Column major 4x4 direction vectors
        // 1, 0, 0, 0,  (X) Right
        // 0, 1, 0, 0,  (Y) Up
        // 0, 0, 1, 0,  (Z) Forward
        // 0, 0, 0, 1   (T) Translation
        inline vec3f Right() { return vec3f(m[0], m[1], m[2]); }
        inline vec3f Left() { return vec3f(-m[0], -m[1], -m[2]); }
        inline vec3f Up() { return vec3f(m[4], m[5], m[6]); }
        inline vec3f Down() { return vec3f(-m[4], -m[5], -m[6]); }
        inline vec3f Forward() { return vec3f(m[8], m[9], m[10]); }
        inline vec3f Backward() { return vec3f(-m[8], -m[9], -m[10]); }

        static Matrix4f Identity()
        {
            return Matrix4f
            {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            };
        }

        static Matrix4f Translation(float x, float y, float z)
        {
            return Matrix4f
            {
                1, 0, 0, x,
                0, 1, 0, y,
                0, 0, 1, z,
                0, 0, 0, 1
            };
        }

        static Matrix4f Scale(float sx, float sy, float sz)
        {
            return Matrix4f{
                sx, 0,  0,  0,
                0,  sy, 0,  0,
                0,  0,  sz, 0,
                0,  0,  0,  1
            };
        }

        static Matrix4f RotationX(float angleRad) // #TODO Angle in degrees
        {
            float c = std::cos(angleRad);
            float s = std::sin(angleRad);

            return Matrix4f{
                1, 0, 0, 0,
                0, c, -s, 0,
                0, s,  c, 0,
                0, 0, 0, 1
            };
        }

        static Matrix4f RotationY(float angleRad) // #TODO Angle in degrees
        {
            const float c = std::cos(angleRad);
            const float s = std::sin(angleRad);

            return Matrix4f{
                 c, 0, s, 0,
                 0, 1, 0, 0,
                -s, 0, c, 0,
                 0, 0, 0, 1
            };
        }

        static Matrix4f RotationZ(float angleRad) // #TODO Angle in degrees
        {
            float c = std::cos(angleRad);
            float s = std::sin(angleRad);

            return Matrix4f{
                c, -s, 0, 0,
                s,  c, 0, 0,
                0,  0, 1, 0,
                0,  0, 0, 1
            };
        }

        // #TODO Look at point rotation method

        static Matrix4f Rotation(float angleRad, const Vector3f& axis) // #TODO Angle in degrees
        {
            return Rotation(angleRad, axis.x, axis.y, axis.z);
        }

        static Matrix4f Rotation(float angleRad, float x, float y, float z) // #TODO Angle in degrees
        {
            // Normalize the axis
            float length = std::sqrt(x * x + y * y + z * z);
            if (length == 0.0f) return Identity(); // Fall back to identity if invalid

            x /= length;
            y /= length;
            z /= length;

            float c = std::cos(angleRad);
            float s = std::sin(angleRad);
            float oneMinusC = 1.0f - c;

            float xx = x * x;
            float yy = y * y;
            float zz = z * z;
            float xy = x * y;
            float xz = x * z;
            float yz = y * z;

            return Matrix4f{
                c + xx * oneMinusC,      xy * oneMinusC - z * s, xz * oneMinusC + y * s, 0.0f,
                xy * oneMinusC + z * s,  c + yy * oneMinusC,     yz * oneMinusC - x * s, 0.0f,
                xz * oneMinusC - y * s,  yz * oneMinusC + x * s, c + zz * oneMinusC,     0.0f,
                0.0f,                    0.0f,                   0.0f,                   1.0f
            };
        }

        // Vector3 manipulation
        // Vector3f TransformPoint(const Vector3f& v) const
        // {
        //     // Multiply as if v = (x, y, z, 1.0)
        //     float x = m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12];
        //     float y = m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13];
        //     float z = m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14];
        //     float w = m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15];
        //
        //     if (w != 0.0f && w != 1.0f)
        //     {
        //         x /= w; y /= w; z /= w;
        //     }
        //
        //     return Vector3f{ x, y, z };
        // }

        // Vector3f TransformDirection(const Vector3f& v) const
        // {
        //     // Multiply as if v = (x, y, z, 0.0)
        //     float x = m[0] * v.x + m[4] * v.y + m[8] * v.z;
        //     float y = m[1] * v.x + m[5] * v.y + m[9] * v.z;
        //     float z = m[2] * v.x + m[6] * v.y + m[10] * v.z;
        //
        //     return Vector3f{ x, y, z };
        // }

        // #TODO Review: Treat v like a point:
        // Vector3f operator*(const Vector3f& v) const
        // {
        //     return TransformPoint(v);
        // }

        // Projection
        // #TODO vec3f include dependency
        static Matrix4f LookAt(const Vector3f& eye, const Vector3f& center, const Vector3f& up)
        {
            Vector3f f = (center - eye).Normalized();
            Vector3f r = f.Cross(up).Normalized();
            Vector3f u = r.Cross(f);

            return Matrix4f{
                 r.x,  u.x, -f.x, 0.0f,
                 r.y,  u.y, -f.y, 0.0f,
                 r.z,  u.z, -f.z, 0.0f,
                -Vector3f::Dot(r, eye),
                -Vector3f::Dot(u, eye),
                 Vector3f::Dot(f, eye),
                 1.0f
            };
        }

        static Matrix4f Perspective(float fovYRadians, float aspect, float nearZ, float farZ) // #TODO Angle in degrees
        {
            float f = 1.0f / std::tan(fovYRadians / 2.0f);
            float depth = nearZ - farZ;

            return Matrix4f{
                f / aspect, 0.0f, 0.0f,                               0.0f,
                0.0f,       f,    0.0f,                               0.0f,
                0.0f,       0.0f, (farZ + nearZ) / depth,            -1.0f,
                0.0f,       0.0f, (2.0f * farZ * nearZ) / depth,      0.0f
            };
        }

        static Matrix4f Orthographic(float left, float right, float bottom, float top, float nearZ, float farZ)
        {
            float rl = right - left;
            float tb = top - bottom;
            float fn = farZ - nearZ;

            return Matrix4f{
                2.0f / rl,     0.0f,         0.0f,        0.0f,
                0.0f,          2.0f / tb,    0.0f,        0.0f,
                0.0f,          0.0f,        -2.0f / fn,   0.0f,
                -(right + left) / rl,
                -(top + bottom) / tb,
                -(farZ + nearZ) / fn,
                1.0f
            };
        }

        // Operators
        Matrix4f operator*(const Matrix4f& rhs) const
        {
            Matrix4f result;

            for (int col = 0; col < 4; ++col)
            {
                for (int row = 0; row < 4; ++row)
                {
                    float sum = 0.0f;
                    for (int k = 0; k < 4; ++k)
                    {
                        sum += (*this)(row, k) * rhs(k, col);
                    }
                    result(row, col) = sum;
                }
            }

            return result;
        }

        // Vector4f operator*(const Vector4f& v) const
        // {
        //     Vector4f result;
        //     result.x = m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12] * v.w;
        //     result.y = m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13] * v.w;
        //     result.z = m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * v.w;
        //     result.w = m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * v.w;
        //     return result;
        // }

        float& operator[](int col) {
            return m[col * 4];
        }

        const float& operator[](int col) const {
            return m[col * 4];
        }

        float& operator()(int col, int row)
        {
            return m[col * 4 + row];
        }

        const float& operator()(int col, int row) const
        {
            return m[col * 4 + row];
        }

        // Decomposition
        // Vector4f GetColumn(int col) const
        // {
        //     return Vector4f(
        //         m[col * 4 + 0],
        //         m[col * 4 + 1],
        //         m[col * 4 + 2],
        //         m[col * 4 + 3]
        //     );
        // }

        // Vector4f GetRow(int row) const
        // {
        //     return Vector4f(
        //         m[0 * 4 + row],
        //         m[1 * 4 + row],
        //         m[2 * 4 + row],
        //         m[3 * 4 + row]
        //     );
        // }

        // void SetColumn(int col, const Vector4f& v)
        // {
        //     m[col * 4 + 0] = v.x;
        //     m[col * 4 + 1] = v.y;
        //     m[col * 4 + 2] = v.z;
        //     m[col * 4 + 3] = v.w;
        // }

        // void SetRow(int row, const Vector4f& v)
        // {
        //     m[0 * 4 + row] = v.x;
        //     m[1 * 4 + row] = v.y;
        //     m[2 * 4 + row] = v.z;
        //     m[3 * 4 + row] = v.w;
        // }

        // #TODO Resolve circular dependency. May need to use .cpp files
        // static Matrix4 Compose(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
        // {
        //     Matrix4 rotMat = rotation.ToMatrix();
        //
        //     // Scale rotation matrix columns
        //     rotMat.data[0] *= scale.x;
        //     rotMat.data[1] *= scale.x;
        //     rotMat.data[2] *= scale.x;
        //
        //     rotMat.data[4] *= scale.y;
        //     rotMat.data[5] *= scale.y;
        //     rotMat.data[6] *= scale.y;
        //
        //     rotMat.data[8] *= scale.z;
        //     rotMat.data[9] *= scale.z;
        //     rotMat.data[10] *= scale.z;
        //
        //     // Set translation
        //     rotMat.data[12] = position.x;
        //     rotMat.data[13] = position.y;
        //     rotMat.data[14] = position.z;
        //     rotMat.data[15] = 1.0f;
        //
        //     return rotMat;
        // }

        // #TODO Resolve circular dependency. May need to use .cpp files
        // bool Decompose(Vector3& position, Quaternion& rotation, Vector3& scale) const
        // {
        //     // Extract position
        //     position = Vector3(data[12], data[13], data[14]);
        //
        //     // Extract scale
        //     Vector3 col0(data[0], data[1], data[2]);
        //     Vector3 col1(data[4], data[5], data[6]);
        //     Vector3 col2(data[8], data[9], data[10]);
        //
        //     scale.x = col0.Length();
        //     scale.y = col1.Length();
        //     scale.z = col2.Length();
        //
        //     if (scale.x == 0.0f || scale.y == 0.0f || scale.z == 0.0f)
        //         return false; // Singular matrix
        //
        //     // Normalize columns to get rotation matrix
        //     Matrix4 rotMat = *this;
        //
        //     for (int i = 0; i < 3; ++i)
        //     {
        //         rotMat.data[0 + i * 4] /= scale.x;
        //         rotMat.data[1 + i * 4] /= scale.y;
        //         rotMat.data[2 + i * 4] /= scale.z;
        //     }
        //
        //     rotation = Quaternion::FromMatrix(rotMat);
        //     return true;
        // }

        // Transpose + inversion
        Matrix4f Transposed() const
        {
            Matrix4f result;
            for (int row = 0; row < 4; ++row)
            {
                for (int col = 0; col < 4; ++col)
                {
                    result(row, col) = (*this)(col, row);
                }
            }
            return result;
        }

        void Transpose()
        {
            for (int row = 0; row < 4; ++row)
            {
                for (int col = row + 1; col < 4; ++col)
                {
                    std::swap((*this)(row, col), (*this)(col, row));
                }
            }
        }

        Matrix4f InverseTranspose() const
        {
            Matrix4f inv = *this;
            if (!inv.Invert())
            {
                // Return identity if not invertible
                return Matrix4f::Identity();
            }

            inv.Transpose();

            // Zero out translation (normals shouldn't be translated)
            inv.m[12] = inv.m[13] = inv.m[14] = 0.0f;

            return inv;
        }

        bool Invert()
        {
            Matrix4f inv;
            const float* floatArray = m;

            inv.m[0] = floatArray[5] * floatArray[10] * floatArray[15] -
                floatArray[5] * floatArray[11] * floatArray[14] -
                floatArray[9] * floatArray[6] * floatArray[15] +
                floatArray[9] * floatArray[7] * floatArray[14] +
                floatArray[13] * floatArray[6] * floatArray[11] -
                floatArray[13] * floatArray[7] * floatArray[10];

            inv.m[4] = -floatArray[4] * floatArray[10] * floatArray[15] +
                floatArray[4] * floatArray[11] * floatArray[14] +
                floatArray[8] * floatArray[6] * floatArray[15] -
                floatArray[8] * floatArray[7] * floatArray[14] -
                floatArray[12] * floatArray[6] * floatArray[11] +
                floatArray[12] * floatArray[7] * floatArray[10];

            inv.m[8] = floatArray[4] * floatArray[9] * floatArray[15] -
                floatArray[4] * floatArray[11] * floatArray[13] -
                floatArray[8] * floatArray[5] * floatArray[15] +
                floatArray[8] * floatArray[7] * floatArray[13] +
                floatArray[12] * floatArray[5] * floatArray[11] -
                floatArray[12] * floatArray[7] * floatArray[9];

            inv.m[12] = -floatArray[4] * floatArray[9] * floatArray[14] +
                floatArray[4] * floatArray[10] * floatArray[13] +
                floatArray[8] * floatArray[5] * floatArray[14] -
                floatArray[8] * floatArray[6] * floatArray[13] -
                floatArray[12] * floatArray[5] * floatArray[10] +
                floatArray[12] * floatArray[6] * floatArray[9];

            inv.m[1] = -floatArray[1] * floatArray[10] * floatArray[15] +
                floatArray[1] * floatArray[11] * floatArray[14] +
                floatArray[9] * floatArray[2] * floatArray[15] -
                floatArray[9] * floatArray[3] * floatArray[14] -
                floatArray[13] * floatArray[2] * floatArray[11] +
                floatArray[13] * floatArray[3] * floatArray[10];

            inv.m[5] = floatArray[0] * floatArray[10] * floatArray[15] -
                floatArray[0] * floatArray[11] * floatArray[14] -
                floatArray[8] * floatArray[2] * floatArray[15] +
                floatArray[8] * floatArray[3] * floatArray[14] +
                floatArray[12] * floatArray[2] * floatArray[11] -
                floatArray[12] * floatArray[3] * floatArray[10];

            inv.m[9] = -floatArray[0] * floatArray[9] * floatArray[15] +
                floatArray[0] * floatArray[11] * floatArray[13] +
                floatArray[8] * floatArray[1] * floatArray[15] -
                floatArray[8] * floatArray[3] * floatArray[13] -
                floatArray[12] * floatArray[1] * floatArray[11] +
                floatArray[12] * floatArray[3] * floatArray[9];

            inv.m[13] = floatArray[0] * floatArray[9] * floatArray[14] -
                floatArray[0] * floatArray[10] * floatArray[13] -
                floatArray[8] * floatArray[1] * floatArray[14] +
                floatArray[8] * floatArray[2] * floatArray[13] +
                floatArray[12] * floatArray[1] * floatArray[10] -
                floatArray[12] * floatArray[2] * floatArray[9];

            inv.m[2] = floatArray[1] * floatArray[6] * floatArray[15] -
                floatArray[1] * floatArray[7] * floatArray[14] -
                floatArray[5] * floatArray[2] * floatArray[15] +
                floatArray[5] * floatArray[3] * floatArray[14] +
                floatArray[13] * floatArray[2] * floatArray[7] -
                floatArray[13] * floatArray[3] * floatArray[6];

            inv.m[6] = -floatArray[0] * floatArray[6] * floatArray[15] +
                floatArray[0] * floatArray[7] * floatArray[14] +
                floatArray[4] * floatArray[2] * floatArray[15] -
                floatArray[4] * floatArray[3] * floatArray[14] -
                floatArray[12] * floatArray[2] * floatArray[7] +
                floatArray[12] * floatArray[3] * floatArray[6];

            inv.m[10] = floatArray[0] * floatArray[5] * floatArray[15] -
                floatArray[0] * floatArray[7] * floatArray[13] -
                floatArray[4] * floatArray[1] * floatArray[15] +
                floatArray[4] * floatArray[3] * floatArray[13] +
                floatArray[12] * floatArray[1] * floatArray[7] -
                floatArray[12] * floatArray[3] * floatArray[5];

            inv.m[14] = -floatArray[0] * floatArray[5] * floatArray[14] +
                floatArray[0] * floatArray[6] * floatArray[13] +
                floatArray[4] * floatArray[1] * floatArray[14] -
                floatArray[4] * floatArray[2] * floatArray[13] -
                floatArray[12] * floatArray[1] * floatArray[6] +
                floatArray[12] * floatArray[2] * floatArray[5];

            inv.m[3] = -floatArray[1] * floatArray[6] * floatArray[11] +
                floatArray[1] * floatArray[7] * floatArray[10] +
                floatArray[5] * floatArray[2] * floatArray[11] -
                floatArray[5] * floatArray[3] * floatArray[10] -
                floatArray[9] * floatArray[2] * floatArray[7] +
                floatArray[9] * floatArray[3] * floatArray[6];

            inv.m[7] = floatArray[0] * floatArray[6] * floatArray[11] -
                floatArray[0] * floatArray[7] * floatArray[10] -
                floatArray[4] * floatArray[2] * floatArray[11] +
                floatArray[4] * floatArray[3] * floatArray[10] +
                floatArray[8] * floatArray[2] * floatArray[7] -
                floatArray[8] * floatArray[3] * floatArray[6];

            inv.m[11] = -floatArray[0] * floatArray[5] * floatArray[11] +
                floatArray[0] * floatArray[7] * floatArray[9] +
                floatArray[4] * floatArray[1] * floatArray[11] -
                floatArray[4] * floatArray[3] * floatArray[9] -
                floatArray[8] * floatArray[1] * floatArray[7] +
                floatArray[8] * floatArray[3] * floatArray[5];

            inv.m[15] = floatArray[0] * floatArray[5] * floatArray[10] -
                floatArray[0] * floatArray[6] * floatArray[9] -
                floatArray[4] * floatArray[1] * floatArray[10] +
                floatArray[4] * floatArray[2] * floatArray[9] +
                floatArray[8] * floatArray[1] * floatArray[6] -
                floatArray[8] * floatArray[2] * floatArray[5];

            float det = floatArray[0] * inv.m[0] + floatArray[1] * inv.m[4] + floatArray[2] * inv.m[8] + floatArray[3] * inv.m[12];
            if (det == 0.0f)
                return false;

            det = 1.0f / det;

            for (int i = 0; i < 16; i++)
                m[i] = inv.m[i] * det;

            return true;
        }

        // Utility
        std::string ToString(int precision = 2) const
        {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(precision);
            for (int row = 0; row < 4; ++row)
            {
                oss << "[ ";
                for (int col = 0; col < 4; ++col)
                {
                    oss << (*this)(row, col) << " ";
                }
                oss << "]\n";
            }
            return oss.str();
        }
    };

}