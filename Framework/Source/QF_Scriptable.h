#pragma once

namespace QwerkE {

	class Entity;

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

		void SetEntity(Entity* newEntity) { m_Entity = newEntity; }

	protected:
		Scriptable() = default;

		Entity* m_Entity = nullptr;
	};

}
