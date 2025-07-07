#pragma once

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
#endif

#include "QF_ComponentScript.h"
#include "QF_ComponentTransform.h"

namespace QwerkE {

	class ScriptablePatrol : public Scriptable
	{
	public:
		void OnCreate() override
		{
			HasRequiredComponents<ComponentTransform, ComponentScript>();
		}

		void OnUpdate(float deltaTime) override
		{
			if (!m_Entity.IsEnabled() || !HasRequiredComponents<ComponentTransform, ComponentScript>())
				return;

			ComponentTransform& transform = m_Entity.GetComponent<ComponentTransform>();

			if (m_DistanceTraveled >= m_Stride)
			{
				m_Direction = -1.f * m_Direction;
				m_DistanceTraveled = 0.f;
			}

			transform.m_Matrix[12] += m_Direction * m_Speed * deltaTime;
			m_DistanceTraveled += abs(m_Direction) * m_Speed * deltaTime;
		}

		eScriptTypes ScriptType() override
		{
			return eScriptTypes::Patrol;
		}

	private:
		MIRROR_PRIVATE_MEMBERS

		float m_Stride = 5.f;
		float m_Speed = 1.f;

		float m_Direction = 1.f;
		float m_DistanceTraveled = 0.f;
	};

}