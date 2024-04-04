#pragma once

#ifdef _QENTT
#include "Libraries/entt/entt.hpp"
#endif

#include "QF_Scene.h"

namespace QwerkE {

	class Entity final
	{
	public:
		Entity() = default;
		Entity(Scene* scene, entt::entity entityHandle)
			: m_Scene(scene), m_EntityHandle(entityHandle) { }

		// #TODO Add asserts
		template <typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			return m_Scene->Registry().emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template <typename T>
		bool HasComponent()
		{
			return m_Scene->Registry().has<T>(m_EntityHandle);
		}

		template <typename T>
		T& GetComponent()
		{
			return m_Scene->Registry().get<T>(m_EntityHandle);
		}

		template <typename T>
		void RemoveComponent()
		{
			if (std::is_same_v<T, ComponentTransform>)
			{
				LOG_WARN("Removing ComponentTransform from entity {0}", (u32)m_EntityHandle);
			}
			m_Scene->Registry().remove<T>(m_EntityHandle);
		}

		Scene* GetScene() { return m_Scene; }

	private:
		entt::entity m_EntityHandle = entt::null;
		Scene* m_Scene = nullptr;
	};

}
