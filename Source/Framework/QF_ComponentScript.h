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
			// #NOTE Used to detect removed scripts from Update() calls
			size_t mapSize = m_ScriptInstances.size();

			for (auto it = m_ScriptInstances.begin(); it != m_ScriptInstances.end(); )
			{
				if (it->second)
				{
					it->second->OnUpdate(deltaTime);

					if (mapSize != m_ScriptInstances.size())
					{
						// #TODO Avoid skipping or duplicating Update() calls on remaining scripts
						it = m_ScriptInstances.end();
						continue;
					}
				}
				// #ifdef _QDEBUG
				else
				{
					// #TODO Test
					// LOG_ERROR();
					// RemoveScript(it->first);
				}
				it++;
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
					scriptable->OnDestroy();
					delete scriptable;
				}
				m_ScriptInstances.erase(scriptType);
				return true;
			}
			LOG_ERROR("{0} Unable to remove script!", __FUNCTION__);
			return false;
		}

		std::unordered_map<eScriptTypes, Scriptable*>& ScriptInstances() { return m_ScriptInstances; }

	private:
		MIRROR_PRIVATE_MEMBERS

		std::unordered_map<eScriptTypes, Scriptable*> m_ScriptInstances;
	};

}
