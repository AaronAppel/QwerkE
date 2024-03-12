#pragma once

#ifdef _QENTT
#include "Libraries/entt/entt.hpp"
#endif

#include "QF_Scene.h"

namespace QwerkE {

	class Entity
	{
	public:
		Entity() = default;
		Entity(Scene* scene, entt::entity entityHandle) : m_Scene(scene), m_EntityHandle(entityHandle) { }
		~Entity() = default;

		void AddComponent();
		// Component& GetComponent();

		template <typename T>
		bool HasComponent()
		{
			return m_Scene->Registry().has<T>(m_EntityHandle);
		}

	private:
		entt::entity m_EntityHandle = entt::null;
		Scene* m_Scene = nullptr;
	};

}
