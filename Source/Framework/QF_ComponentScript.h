#pragma once

#include <map>

#include "QF_EntityHandle.h"
#include "QF_Scene.h"
#include "QF_Scriptable.h"
#include "QF_Scripting.h"

namespace QwerkE {

	class ComponentScript
	{
	public:
		ComponentScript() = default;

		~ComponentScript()
		{
			// #TODO Find out why destructor is immediately called
			// Unbind();
		}

		std::unordered_map<eScriptTypes, Scriptable*> m_ScriptInstances;

		void Bind(const EntityHandle& entityHandle)
		{
			for (auto& enumScriptablePair : m_ScriptInstances)
			{
				if (!enumScriptablePair.second)
				{
					AddScript(enumScriptablePair.first, entityHandle);
				}
				else
				{
					enumScriptablePair.second->SetEntity(entityHandle);
				}
			}
		}

		void Unbind()
		{
			std::vector<eScriptTypes> scriptTypes;
			for (auto& pair : m_ScriptInstances)
			{
				scriptTypes.push_back(pair.first);
			}
			for (size_t i = 0; i < scriptTypes.size(); i++)
			{
				RemoveScript(scriptTypes[i]);
			}
		}

		void Update(float deltaTime)
		{
			for (auto& enumScriptablePair : m_ScriptInstances)
			{
				if (enumScriptablePair.second)
				{
					// #TODO Fix crash "Exception thrown: read access violation. enumScriptablePair.second->was 0xFFFFFFFFFFFFFFE7"
					enumScriptablePair.second->OnUpdate(deltaTime); // #TODO Fix Update() removes script from map
				}
			}
		}

		bool HasScript(const eScriptTypes scriptType) // #NOTE Editor only
		{
			return m_ScriptInstances.find(scriptType) != m_ScriptInstances.end();
		}

		void AddScript(const eScriptTypes scriptType, const EntityHandle& entityHandle)
		{
			Scriptable* newScript = Scripting::InstantiateScript(scriptType);
			newScript->SetEntity(entityHandle);
			newScript->OnCreate();
			m_ScriptInstances[scriptType] = newScript;
		}

		bool RemoveScript(const eScriptTypes scriptType)
		{
			// #TODO Removing from map causes crashed in external loops
			if (m_ScriptInstances.find(scriptType) != m_ScriptInstances.end())
			{
				if (Scriptable* scriptable = m_ScriptInstances[scriptType])
				{
					delete scriptable;
				}
				m_ScriptInstances.erase(scriptType);
				return true;
			}
			LOG_ERROR("{0} Unable to remove script!", __FUNCTION__);
			return false;
		}

	};

}
