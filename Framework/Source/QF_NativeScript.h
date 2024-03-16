#pragma once

#include <functional>

#include "QF_Entity.h"
#include "QF_Log.h"

namespace QwerkE {

	class Scriptable
	{
	public:
		Scriptable(Entity* entity) : m_Entity(entity) { }
		virtual ~Scriptable() = default;

		// #TODO Add asserts
		template <typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			return m_Entity->AddComponent<T>(std::forward<Args>(args)...);
		}

		template <typename T>
		bool HasComponent()
		{
			return m_Entity->HasComponent<T>();
		}

		template <typename T>
		T& GetComponent()
		{
			return m_Entity->GetComponent<T>();
		}

		template <typename T>
		void RemoveComponent()
		{
			m_Entity->RemoveComponent<T>();
		}

	protected:
		Entity* m_Entity = nullptr;
	};

	class NativeScriptComponent
	{
	public:
		Scriptable* Instance = nullptr;

		std::function<void()> InstantiateFunction;
		std::function<void()> DeleteFunction;

		std::function<void(Scriptable*)> OnCreateFunction;
		std::function<void(Scriptable*)> OnDestroyFunction;
		std::function<void(Scriptable*, float)> OnUpdateFunction;

		template <typename T>
		void Bind(Entity* entity)
		{
			InstantiateFunction = [&]() { Instance = new T(entity); };
			DeleteFunction = [&]() { delete (T*)Instance; Instance = nullptr; };

			OnCreateFunction = [](Scriptable* instance) { ((T*)instance)->OnCreate(); };
			OnDestroyFunction = [](Scriptable* instance) { ((T*)instance)->OnDestroy(); };
			OnUpdateFunction = [](Scriptable* instance, float deltaTime) { ((T*)instance)->OnUpdate(deltaTime); };
		}

	};

}
