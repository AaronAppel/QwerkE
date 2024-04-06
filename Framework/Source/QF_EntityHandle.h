#pragma once

#ifdef _QENTT
#include "Libraries/entt/entt.hpp"
#endif

#include "QF_ComponentInfo.h"
#include "QF_ComponentTransform.h"

#include "QF_Scene.h"

namespace QwerkE {

	class EntityHandle final
	{
	public:
		EntityHandle() = default; // #REVIEW Hide if proper. Compile error in EntityEditor and InstantiateScript
		EntityHandle(const EntityHandle& other) = default;

		EntityHandle(Scene* scene) :
			m_Scene(scene)
		{
			ASSERT(m_Scene, "Null m_Scene reference!");
			m_EnttId = m_Scene->m_Registry.create();
			if (!HasComponent<ComponentInfo>())
			{
				AddComponent<ComponentInfo>();
			}

			if (!HasComponent<ComponentTransform>())
			{
				AddComponent<ComponentTransform>();
			}
		}

		EntityHandle(Scene* scene, entt::entity entityId)
			: m_Scene(scene), m_EnttId(entityId)
		{
			ASSERT(m_EnttId != entt::null, "m_EnttId is null!");

			// #TODO Decide where required components are added. If an entt id is valid,
			if (!HasComponent<ComponentInfo>() ||
				!HasComponent<ComponentTransform>())
			{
				LOG_WARN("{0} ", __FUNCTION__);
			}
		}

		// #TODO Add asserts

		const GUID& EntityGuid()
		{
			return GetComponent<ComponentInfo>().m_Guid;
		}

		const char* const EntityName()
		{
			return GetComponent<ComponentInfo>().m_EditorDisplayName;
		}

		bool IsEnabled()
		{
			return GetComponent<ComponentInfo>().m_Enabled;
		}

		void SetIsEnabled(bool isEnabled)
		{
			GetComponent<ComponentInfo>().m_Enabled = isEnabled;
		}

		template <typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			return m_Scene->m_Registry.emplace<T>(m_EnttId, std::forward<Args>(args)...);
		}

		template <typename T>
		bool HasComponent() const
		{
			return m_Scene->m_Registry.has<T>(m_EnttId);
		}

		template <typename T>
		T& GetComponent()
		{
			return m_Scene->m_Registry.get<T>(m_EnttId);
		}

		template <typename T>
		void RemoveComponent()
		{
			if (std::is_same_v<T, ComponentTransform> ||
				std::is_same_v<T, ComponentInfo>)
			{
				LOG_ERROR("Removing essential component from entity {0}", (u32)m_EnttId);
			}
			m_Scene->m_Registry.remove<T>(m_EnttId);
		}

		Scene* GetScene() { return m_Scene; }

		void Invalidate() { m_EnttId != entt::null; m_Scene = nullptr; }
		static EntityHandle InvalidHandle() { return EntityHandle(); }

		operator bool() const { return m_EnttId != entt::null; }

		bool EntityHandle::operator == (const EntityHandle& other) const { return other.m_EnttId == m_EnttId; }
		bool EntityHandle::operator != (const EntityHandle& other) const { return other.m_EnttId != m_EnttId; }

	private:
		friend class Scene; // To see m_EnttId

		entt::entity m_EnttId = entt::null;
		Scene* m_Scene = nullptr;
	};

}
