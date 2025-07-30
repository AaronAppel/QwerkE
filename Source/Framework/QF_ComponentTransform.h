#pragma once

#ifdef _QBGFX
#include <bx/math.h>
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
#endif

#include "QC_Math.h"

namespace QwerkE {

	struct ComponentTransform
	{
		ComponentTransform()
		{
#ifdef _QBGFX
			bx::mtxIdentity(m_Matrix);
#endif
		}
		ComponentTransform(const ComponentTransform& other) = default;
		ComponentTransform(const vec3f& other) { SetPosition(other); }
		ComponentTransform(float newPositionX, float newPositionY, float newPositionZ) { SetPosition(vec3f(newPositionX, newPositionY, newPositionZ)); }

#ifdef _QBGFX
		inline bx::Vec3 PositionBx() const { vec3f pos   = Math::MatrixPosition(m_Matrix); return bx::Vec3(pos.x, pos.y, pos.z); }
		inline bx::Vec3 RotationBx() const { vec3f rot   = Math::MatrixRotation(m_Matrix); return bx::Vec3(rot.x, rot.y, rot.z); }
		inline bx::Vec3 ScaleBx()	 const { vec3f scale = Math::MatrixScale(m_Matrix);    return bx::Vec3(scale.x, scale.y, scale.z); }

		inline void SetPosition(const bx::Vec3& a_NewPosition) { m_Matrix[12] = a_NewPosition.x; m_Matrix[13] = a_NewPosition.y; m_Matrix[14] = a_NewPosition.z; }
#endif

		inline void SetPosition(const vec3f& newPosition) { m_Matrix[12] = newPosition.x; m_Matrix[13] = newPosition.y; m_Matrix[14] = newPosition.z; }
		// #TODO inline void SetRotation() { ; }

		inline void Translate(const vec3f& a_Offset)
			{ m_Matrix[12] += a_Offset.x; m_Matrix[13] += a_Offset.y; m_Matrix[14] += a_Offset.z; }
		inline void Translate(const vec3f& a_Axis, float a_Magnitude)
			{ m_Matrix[12] += a_Axis.x * a_Magnitude; m_Matrix[13] += a_Axis.y * a_Magnitude; m_Matrix[14] += a_Axis.z * a_Magnitude; }

		inline vec3f Position() const { return Math::MatrixPosition(m_Matrix); }
		inline vec3f Rotation() const { return Math::MatrixRotation(m_Matrix); }
		inline vec3f Scale() const { return Math::MatrixScale(m_Matrix); }

		inline vec3f Forward() const { return Math::MatrixForward(m_Matrix); }
		inline vec3f Backward() const { return Math::MatrixBackward(m_Matrix); }
		inline vec3f Up() const { return Math::MatrixUp(m_Matrix); }
		inline vec3f Down() const { return Math::MatrixDown(m_Matrix); }
		inline vec3f Right() const { return Math::MatrixRight(m_Matrix); }
		inline vec3f Left() const { return Math::MatrixLeft(m_Matrix); }

		float m_Matrix[16] = { 0.0f };
	};

}
