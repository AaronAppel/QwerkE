#pragma once

#include "QF_ComponentScript.h"
#include "QF_ComponentTransform.h"
#include "QF_Scriptable.h"

namespace QwerkE {

	class ScriptablePatrol : public Scriptable
	{
	public:
		void OnCreate() override
		{
			if (!m_Entity.HasComponent<ComponentTransform>())
			{
				LOG_ERROR("Entity {0} missing ScriptableTransform!", m_Entity.GetComponent<ComponentInfo>().m_EntityName.c_str());

				if (m_Entity.HasComponent<ComponentScript>())
				{
					ComponentScript& script = m_Entity.GetComponent<ComponentScript>();
					script.RemoveScript(eScriptTypes::Camera);
				}
				else
				{
					LOG_CRITICAL("Could not remove ComponentScript!");
				}
			}
		}

		void OnUpdate(float deltaTime) override
		{
			ComponentTransform& transform = m_Entity.GetComponent<ComponentTransform>();

			if (m_DistanceTraveled >= m_Stride)
			{
				m_Direction = -1.f * m_Direction;
				m_DistanceTraveled = 0.f;
			}

			transform.m_Matrix[12] += m_Direction * m_Speed * deltaTime;
			m_DistanceTraveled += abs(m_Direction) * m_Speed * deltaTime;
		}

		eScriptTypes ScriptType()
		{
			return eScriptTypes::Patrol;
		}

		float m_Stride = 5.f;
		float m_Speed = 1.f;

	private:
		float m_Direction = 1.f;
		float m_DistanceTraveled = 0.f;
	};

}