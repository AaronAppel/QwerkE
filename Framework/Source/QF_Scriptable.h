#pragma once

#include "QF_EntityHandle.h"

namespace QwerkE {

	// #NOTE These values are serialized so value changes matter
	enum eScriptTypes : u8
	{
		Invalid = 0,
		Camera,
		Testing,
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

		void SetEntity(EntityHandle newEntity) { m_Entity = newEntity; }

		virtual eScriptTypes ScriptType() = 0;

	protected:
		Scriptable() = default;

		EntityHandle m_Entity;
	};

}
