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

		EntityHandle(Scene* scene, const GUID& existingGuid) :
			m_Scene(scene)
		{
			ASSERT(m_Scene, "Null m_Scene reference!");

			// #TODO Define behaviour. When should a new entt entity be created?
			EntityHandle handle = m_Scene->GetEntityByGuid(existingGuid);
			if (handle)
			{
				m_EnttId = handle.m_EnttId;
			}
			else
			{
				EntityHandle newHandle = m_Scene->CreateEntity();
				m_EnttId = newHandle.m_EnttId;
			}

			if (!HasComponent<ComponentInfo>())
			{
				AddComponent<ComponentInfo>(existingGuid);
			}

			if (!HasComponent<ComponentTransform>())
			{
				AddComponent<ComponentTransform>();
			}
		}

		const GUID& EntityGuid()
		{
			ASSERT(m_EnttId != entt::null, "m_EnttId is null!");
			ASSERT(m_Scene->m_Registry.has<ComponentInfo>(m_EnttId), "Entity does not have ComponentInfo!");
			return GetComponent<ComponentInfo>().m_Guid;
		}

		const char* const EntityName()
		{
			ASSERT(m_EnttId != entt::null, "m_EnttId is null!");
			ASSERT(m_Scene->m_Registry.has<ComponentInfo>(m_EnttId), "Entity does not have ComponentInfo!");
			return GetComponent<ComponentInfo>().m_EditorDisplayName;
		}

		bool IsEnabled()
		{
			ASSERT(m_EnttId != entt::null, "m_EnttId is null!");
			ASSERT(m_Scene->m_Registry.has<ComponentInfo>(m_EnttId), "Entity does not have ComponentInfo!");
			return GetComponent<ComponentInfo>().m_Enabled;
		}

		void SetIsEnabled(bool isEnabled)
		{
			ASSERT(m_EnttId != entt::null, "m_EnttId is null!");
			ASSERT(m_Scene->m_Registry.has<ComponentInfo>(m_EnttId), "Entity does not have ComponentInfo!");
			GetComponent<ComponentInfo>().m_Enabled = isEnabled;
		}

		template <typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			ASSERT(m_EnttId != entt::null, "m_EnttId is null!");
			ASSERT(!m_Scene->m_Registry.has<T>(m_EnttId), "Entity already has component!");
			return m_Scene->m_Registry.emplace<T>(m_EnttId, std::forward<Args>(args)...);
		}

		template <typename T>
		bool HasComponent() const
		{
			ASSERT(m_EnttId != entt::null, "m_EnttId is null!");
			return m_Scene->m_Registry.has<T>(m_EnttId);
		}

		template <typename T>
		T& GetComponent()
		{
			ASSERT(m_EnttId != entt::null, "m_EnttId is null!");
			ASSERT(m_Scene->m_Registry.has<T>(m_EnttId), "Entity doesn't have component!");
			return m_Scene->m_Registry.get<T>(m_EnttId);
		}

		template <typename T>
		void RemoveComponent()
		{
			ASSERT(m_EnttId != entt::null, "m_EnttId is null!");
			ASSERT(m_Scene->m_Registry.has<T>(m_EnttId), "Entity doesn't have component!");
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

		operator bool() const { return m_EnttId != entt::null && m_Scene; }

		bool EntityHandle::operator == (const EntityHandle& other) const { return other.m_EnttId == m_EnttId; }
		bool EntityHandle::operator != (const EntityHandle& other) const { return other.m_EnttId != m_EnttId; }

	private:
		friend class Scene; // To see m_EnttId

		entt::entity m_EnttId = entt::null;
		Scene* m_Scene = nullptr;
	};

}
