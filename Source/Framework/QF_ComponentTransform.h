#pragma once

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
#endif

#define ROW_MAJOR 0

namespace QwerkE {

	struct ComponentTransform
	{
		ComponentTransform();
		ComponentTransform(const ComponentTransform& other) = default;
		ComponentTransform(const vec3f& other);
		ComponentTransform(float newPositionX, float newPositionY, float newPositionZ);

		void SetPosition(const vec3f& newPosition);
		inline vec3f GetPosition() const { return vec3f(m_Matrix[12], m_Matrix[13], m_Matrix[14]); }
		vec3f GetRotation() const;
		vec3f GetScale() const;

		// #NOTE Column major 4x4 direction vectors
		// 1, 0, 0, 0,  Right (X)
		// 0, 1, 0, 0,  Up (Y)
		// 0, 0, 1, 0,  Forward (Z)
		// 0, 0, 0, 1   Translation
		vec3f Forward() const;
		vec3f Backward() const;
		vec3f Up() const;
		vec3f Down() const;
		vec3f Right() const;
		vec3f Left() const;

		float m_Matrix[16];
	};

}
