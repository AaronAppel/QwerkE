#pragma once

#include <map>

#include "QF_Scene.h"
#include "QF_Scriptable.h"
#include "QF_Scripting.h"

namespace QwerkE {

	class ComponentScript
	{
	public:
		std::unordered_map<eScriptTypes, Scriptable*> m_ScriptInstances;
		bool m_Bound = false;

		void Bind(Scene* scene, entt::entity enttId)
		{
			if (m_Bound)
			{
				LOG_WARN("{0} Scripts already bound", __FUNCTION__);
				return;
			}

			for (auto& enumScriptablePair : m_ScriptInstances)
			{
				Scriptable* newScript = Scripting::InstantiateScript(enumScriptablePair.first);
				newScript->SetEntity({ scene, enttId });
				m_ScriptInstances[enumScriptablePair.first] = newScript;
			}
			m_Bound = true;
		}

		void Unbind()
		{
			if (!m_Bound)
			{
				LOG_WARN("{0} Scripts not bound", __FUNCTION__);
				return;
			}

			for (auto& enumScriptablePair : m_ScriptInstances)
			{
				if (enumScriptablePair.second)
				{
					delete enumScriptablePair.second;
				}
			}
			m_Bound = false;
		}

		void Update(float deltaTime)
		{
			for (auto& enumScriptablePair : m_ScriptInstances)
			{
				if (enumScriptablePair.second)
				{
					enumScriptablePair.second->OnUpdate(deltaTime);
				}
			}
		}

	};

}
