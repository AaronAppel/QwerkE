#pragma once

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
#endif

namespace QwerkE {

	struct ComponentTransform
	{
		ComponentTransform();
		ComponentTransform(const ComponentTransform& other) = default;
		ComponentTransform(const vec3f& other);
		ComponentTransform(float newPositionX, float newPositionY, float newPositionZ);

		void SetPosition(const vec3f& newPosition);
		vec3f GetPosition() const { return vec3f(m_Matrix[12], m_Matrix[13], m_Matrix[14]); }
		vec3f GetRotation() const;
		vec3f GetScale() const;

		vec3f Forward() const;

		const float* const GetMatrix() const { return m_Matrix; }

		// #TODO Consider changing to private and using a class if direct matrix access is improper
		float m_Matrix[16];
	};

}
