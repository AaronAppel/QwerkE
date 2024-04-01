#pragma once

namespace QwerkE {

	class ComponentTransform
	{
	public:
		ComponentTransform();
		ComponentTransform(const ComponentTransform& other) = default;
		ComponentTransform(const vec3f& other);
		ComponentTransform(float newPositionX, float newPositionY, float newPositionZ);

		void SetPosition(const vec3f& newPosition);
		vec3f GetPosition() const { return vec3f(m_Matrix[12], m_Matrix[13], m_Matrix[14]); }

		const float* const GetMatrix() const { return m_Matrix; }

	private:
		float m_Matrix[16];
	};

}
