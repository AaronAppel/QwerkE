#pragma once

#ifdef _QENUM
#include "Libraries/enum/QC_enum.h"
#endif

#include "QF_EntityHandle.h"

namespace QwerkE {

	QC_ENUM(eScriptTypesStr, u8,
		Invalid = 0,
		Camera,
		Testing,
		Patrol,
		PathFinder,

		// User scripts
		// #TODO Improve architecture for user generated types
		UserScript1, // ScriptGameEntity

		COUNT)

	enum eScriptTypes : u8
	{
		Invalid = 0,
		Camera,
		Testing,
		Patrol,
		PathFinder,

		// User scripts
		// #TODO Improve architecture for user generated types
		UserScript1, // ScriptGameEntity

		// #NOTE These values are serialized so value changes matter
		COUNT,
	};

	struct ScriptGuiButton
	{
		using CallBackFunction = void(*)(void);
		CallBackFunction m_CallbackFunction = nullptr;
		std::string m_ButtonName = Constants::gc_DefaultStringValue;
	};

	class Scriptable
	{
	public:
		virtual ~Scriptable() = default;

		virtual void OnCreate() { }
		virtual void OnDestroy() { }
		virtual void OnUpdate(float deltaTime) { }

		// virtual void OnAwake() { }
		// virtual void OnActivate(true/false) { }

		// virtual void OnFixedUpdate() { }
		// virtual void OnLateUpdate() { }

		// virtual void OnTriggerCollision() { }
		// virtual void OnBodyCollision() { }

		const EntityHandle& GetEntity() { return m_Entity; }
		void SetEntity(EntityHandle newEntity) { m_Entity = newEntity; }

		virtual eScriptTypes ScriptType() = 0;

		// #TODO Check if entity has required components for this script instance
		virtual bool ValidEntityForScript(const EntityHandle& entity) { return true; }

	protected:
		Scriptable() = default;

		template <typename... Args>
		bool HasRequiredComponents()
		{
			if (!m_Entity.HasAllComponents<Args...>())
			{
				LOG_CRITICAL("Entity \"{0}\" missing required component(s)!", m_Entity.GetComponent<ComponentInfo>().m_EntityName.c_str());
				if (m_Entity.HasComponent<ComponentScript>())
				{
					ComponentScript& script = m_Entity.GetComponent<ComponentScript>();
					script.RemoveScript(eScriptTypes::Camera);
				}
				else
				{
					LOG_CRITICAL("Could not remove ComponentScript!");
				}
				return false;
			}
			return true;
		}

		EntityHandle m_Entity;
	};

}
