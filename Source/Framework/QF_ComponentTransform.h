#pragma once

#include "QC_Matrix4.h"
#include "QC_Vector3.h"

namespace QwerkE {

	struct ComponentTransform
	{
		ComponentTransform() = default;
		ComponentTransform(const ComponentTransform& other) = default;
		ComponentTransform(const vec3f& other) { m_Matrix.SetPosition(other); }
		ComponentTransform(float newPositionX, float newPositionY, float newPositionZ) { m_Matrix.SetPosition(vec3f(newPositionX, newPositionY, newPositionZ)); }

		mat4f m_Matrix;
	};

}
