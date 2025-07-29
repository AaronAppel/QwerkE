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
#if ROW_MAJOR
#else
		return Math::MatrixRotation(m_Matrix);
#endif // ROW_MAJOR
	}

	vec3f ComponentTransform::GetScale() const
	{
		return Math::MatrixScale(m_Matrix);
	}

	vec3f ComponentTransform::Forward() const
	{
		return vec3f(m_Matrix[8], m_Matrix[9], m_Matrix[10]);
	}

	vec3f ComponentTransform::Backward() const
	{
		return vec3f(-m_Matrix[8], -m_Matrix[9], -m_Matrix[10]);
	}

	vec3f ComponentTransform::Up() const
	{
		return vec3f(m_Matrix[4], m_Matrix[5], m_Matrix[6]);
	}

	vec3f ComponentTransform::Down() const
	{
		return vec3f(-m_Matrix[4], -m_Matrix[5], -m_Matrix[6]);
	}

	vec3f ComponentTransform::Right() const
	{
		return vec3f(m_Matrix[0], m_Matrix[1], m_Matrix[2]);
	}
	vec3f ComponentTransform::Right() const
	{
		return vec3f(m_Matrix[0], m_Matrix[1], m_Matrix[2]);
	}

	vec3f ComponentTransform::Left() const
	{
		return vec3f(-m_Matrix[0], -m_Matrix[1], -m_Matrix[2]);
	}

}
