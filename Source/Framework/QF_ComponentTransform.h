#pragma once

#ifdef _QBGFX
#include <bx/math.h>
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
#endif

namespace QwerkE {

	struct ComponentTransform
	{
		ComponentTransform() = default;
		ComponentTransform(const ComponentTransform& other) = default;
		ComponentTransform(const vec3f& other) { SetPosition(other); }
		ComponentTransform(float newPositionX, float newPositionY, float newPositionZ) { SetPosition(vec3f(newPositionX, newPositionY, newPositionZ)); }

#ifdef _QBGFX
		inline bx::Vec3 PositionBx() const { vec3f pos   = Position(); return bx::Vec3(pos.x, pos.y, pos.z); }
		inline bx::Vec3 RotationBx() const { vec3f rot   = Rotation(); return bx::Vec3(rot.x, rot.y, rot.z); }
		inline bx::Vec3 ScaleBx()	 const { vec3f scale = Scale();    return bx::Vec3(scale.x, scale.y, scale.z); }

		inline void SetPosition(const bx::Vec3& a_NewPosition) { m_Matrix[12] = a_NewPosition.x; m_Matrix[13] = a_NewPosition.y; m_Matrix[14] = a_NewPosition.z; }
#endif

		inline void SetPosition(const vec3f& newPosition) { m_Matrix[12] = newPosition.x; m_Matrix[13] = newPosition.y; m_Matrix[14] = newPosition.z; }
		// #TODO inline void SetRotation() { ; }

        inline void Translate(const vec3f& a_Offset) { m_Matrix[12] += a_Offset.x; m_Matrix[13] += a_Offset.y; m_Matrix[14] += a_Offset.z; }
        inline void Translate(const vec3f& a_Axis, const float a_Magnitude) { m_Matrix[12] += a_Axis.x * a_Magnitude; m_Matrix[13] += a_Axis.y * a_Magnitude; m_Matrix[14] += a_Axis.z * a_Magnitude; }

		inline vec3f Position() const { return vec3f(m_Matrix[12], m_Matrix[13], m_Matrix[14]); }
		vec3f Rotation() const;
		vec3f Scale() const;

		/*
		vec3f MatrixRotation(const float a_Matrix[16])
        {
            vec3f returnRotation; // x=pitch, y=yaw, z=roll

            // Step 1: Extract scale
            float scaleX = std::sqrt(a_Matrix[0] * a_Matrix[0] + a_Matrix[1] * a_Matrix[1] + a_Matrix[2] * a_Matrix[2]);
            float scaleY = std::sqrt(a_Matrix[4] * a_Matrix[4] + a_Matrix[5] * a_Matrix[5] + a_Matrix[6] * a_Matrix[6]);
            float scaleZ = std::sqrt(a_Matrix[8] * a_Matrix[8] + a_Matrix[9] * a_Matrix[9] + a_Matrix[10] * a_Matrix[10]);

            // Step 2: Normalize the rotation part (upper 3x3) by removing scale
            float r00 = a_Matrix[0] / scaleX;
            float r01 = a_Matrix[4] / scaleY;
            float r02 = a_Matrix[8] / scaleZ;

            float r10 = a_Matrix[1] / scaleX;
            float r11 = a_Matrix[5] / scaleY;
            float r12 = a_Matrix[9] / scaleZ;

            float r20 = a_Matrix[2] / scaleX;
            float r21 = a_Matrix[6] / scaleY;
            float r22 = a_Matrix[10] / scaleZ;

            // Step 3: Convert to Euler angles (Y-X-Z order: yaw, pitch, roll)
            if (r20 < 1.0f) {
                if (r20 > -1.0f) {
                    returnRotation.x = std::asin(-r20);
                    returnRotation.y = std::atan2(r10, r00);
                    returnRotation.z = std::atan2(r21, r22);
                }
                else {
                    // r20 == -1
                    returnRotation.x = PI / 2.0f;
                    returnRotation.y = -std::atan2(-r12, r11);
                    returnRotation.z = 0.0f;
                }
            }
            else {
                // r20 == +1
                returnRotation.x = -PI / 2.0f;
                returnRotation.y = std::atan2(-r12, r11);
                returnRotation.z = 0.0f;
            }

            // Convert radians to degrees (optional)
            const float RAD2DEG = 180.0f / static_cast<float>(PI);
            returnRotation.x *= RAD2DEG;
            returnRotation.y *= RAD2DEG;
            returnRotation.z *= RAD2DEG;

            return returnRotation;
        }

        vec3f MatrixScale(const float a_Matrix[16]) // Column Major
        {
            vec3f returnScale;

            // X axis scale = length of column 0 vector
            returnScale.x = std::sqrt(a_Matrix[0] * a_Matrix[0] +
                a_Matrix[1] * a_Matrix[1] +
                a_Matrix[2] * a_Matrix[2]);

            // Y axis scale = length of column 1 vector
            returnScale.y = std::sqrt(a_Matrix[4] * a_Matrix[4] +
                a_Matrix[5] * a_Matrix[5] +
                a_Matrix[6] * a_Matrix[6]);

            // Z axis scale = length of column 2 vector
            returnScale.z = std::sqrt(a_Matrix[8] * a_Matrix[8] +
                a_Matrix[9] * a_Matrix[9] +
                a_Matrix[10] * a_Matrix[10]);

            return returnScale;
        }
        */

		// #NOTE Column major 4x4 direction vectors
		// 1, 0, 0, 0,  (X) Right
		// 0, 1, 0, 0,  (Y) Up
		// 0, 0, 1, 0,  (Z) Forward
		// 0, 0, 0, 1   (T) Translation
		inline vec3f Right() { return vec3f(m_Matrix[0], m_Matrix[1], m_Matrix[2]); }
		inline vec3f Left() { return vec3f(-m_Matrix[0], -m_Matrix[1], -m_Matrix[2]); }
		inline vec3f Up() { return vec3f(m_Matrix[4], m_Matrix[5], m_Matrix[6]); }
		inline vec3f Down() { return vec3f(-m_Matrix[4], -m_Matrix[5], -m_Matrix[6]); }
		inline vec3f Forward() { return vec3f(m_Matrix[8], m_Matrix[9], m_Matrix[10]); }
		inline vec3f Back() { return vec3f(-m_Matrix[8], -m_Matrix[9], -m_Matrix[10]); }

		mat4f m_Matrix;
	};

}
