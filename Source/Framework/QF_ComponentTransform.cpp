#include "QF_ComponentTransform.h"

#ifdef _QBGFX
#include <bx/math.h>
#endif

namespace QwerkE {

	ComponentTransform::ComponentTransform()
	{
#ifdef _QBGFX
		bx::mtxIdentity(m_Matrix);
#endif
	}

	ComponentTransform::ComponentTransform(const vec3f& other)
	{
		SetPosition(other);
	}

	ComponentTransform::ComponentTransform(float newPositionX, float newPositionY, float newPositionZ)
	{
		SetPosition(vec3f(newPositionX, newPositionY, newPositionZ));
	}

	void ComponentTransform::SetPosition(const vec3f& newPosition)
	{
		m_Matrix[12] = newPosition.x;
		m_Matrix[13] = newPosition.y;
		m_Matrix[14] = newPosition.z;
	}

	vec3f ComponentTransform::GetRotation() const
	{
		return Math::MatrixRotation(m_Matrix);
	}

	vec3f ComponentTransform::GetScale() const
	{
		return Math::MatrixScale(m_Matrix);
	}

	vec3f ComponentTransform::Forward() const
	{
		const vec3f dirFromTransform = vec3f(m_Matrix[2], m_Matrix[6], m_Matrix[10]);
		return dirFromTransform;
	}

}
