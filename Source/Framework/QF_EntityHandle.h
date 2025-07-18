#pragma once

#ifdef _QENTT
#include "Libraries/entt/entt.hpp"
#endif

#include "QF_ComponentInfo.h"
#include "QF_ComponentTransform.h"
#include "QF_Debug.h"
#include "QF_Log.h"
#include "QF_Scene.h"
#include "QF_Scenes.h"

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
			m_EnttId = m_Scene->m_Registry.create(); // #TODO Review if EntityHandle should create new scene entities
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

			EntityHandle handle = m_Scene->GetEntityByGuid(existingGuid);
			if (handle)
			{
				m_EnttId = handle.m_EnttId;

				if (!HasComponent<ComponentInfo>())
				{
					AddComponent<ComponentInfo>(existingGuid);
				}

				if (!HasComponent<ComponentTransform>())
				{
					AddComponent<ComponentTransform>();
				}
			}
			else
			{
				LOG_WARN("{0} Scene missing entity with guid {1}", __FUNCTION__, existingGuid);
			}
		}

		GUID& EntityGuid()
		{
			IsValid();
			ASSERT(m_Scene->m_Registry.has<ComponentInfo>(m_EnttId), "Entity does not have ComponentInfo!");
			return GetComponent<ComponentInfo>().m_Guid;
		}

		std::string& EntityName()
		{
			IsValid();
			ASSERT(m_Scene->m_Registry.has<ComponentInfo>(m_EnttId), "Entity does not have ComponentInfo!");
			return GetComponent<ComponentInfo>().m_EntityName;
		}

		void SetEntityName(const std::string& newEntityName)
		{
			IsValid();
			ASSERT(m_Scene->m_Registry.has<ComponentInfo>(m_EnttId), "Entity does not have ComponentInfo!");
			GetComponent<ComponentInfo>().m_EntityName = newEntityName;
		}

		bool IsEnabled()
		{
			IsValid();
			ASSERT(m_Scene->m_Registry.has<ComponentInfo>(m_EnttId), "Entity does not have ComponentInfo!");
			return GetComponent<ComponentInfo>().m_Enabled;
		}

		void SetIsEnabled(bool isEnabled)
		{
			IsValid();
			ASSERT(m_Scene->m_Registry.has<ComponentInfo>(m_EnttId), "Entity does not have ComponentInfo!");
			GetComponent<ComponentInfo>().m_Enabled = isEnabled;
		}

		template <typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			IsValid();
			ASSERT(!m_Scene->m_Registry.has<T>(m_EnttId), "Entity already has component!");
			return m_Scene->m_Registry.emplace<T>(m_EnttId, std::forward<Args>(args)...);
		}

		template <typename T>
		bool HasComponent() const
		{
			IsValid();
			return m_Scene->m_Registry.has<T>(m_EnttId);
		}

		template <typename... Args>
		bool HasAllComponents() const // #TODO Would be cool to pass in multiple components to check
		{
			IsValid();
			return m_Scene->m_Registry.has<Args...>(m_EnttId);
		}

		template <typename T>
		T& GetComponent()
		{
			IsValid();
			ASSERT(m_Scene->m_Registry.has<T>(m_EnttId), "Entity doesn't have component!");
			return m_Scene->m_Registry.get<T>(m_EnttId);
		}

		template <typename T>
		const T& GetComponent() const // #NOTE Keep in sync with non-const method above
		{
			IsValid();
			ASSERT(m_Scene->m_Registry.has<T>(m_EnttId), "Entity doesn't have component!");
			return m_Scene->m_Registry.get<T>(m_EnttId);
		}

		template <typename T>
		void RemoveComponent()
		{
			IsValid();
			ASSERT(m_Scene->m_Registry.has<T>(m_EnttId), "Entity doesn't have component!");
			if (std::is_same_v<T, ComponentTransform> ||
				std::is_same_v<T, ComponentInfo>)
			{
				LOG_ERROR("Removing essential component from entity {0}", (u32)m_EnttId);
			}
			m_Scene->m_Registry.remove<T>(m_EnttId);
		}

		Scene* GetScene() { return m_Scene; }

		inline bool IsValid() const { return m_EnttId != entt::null && m_Scene && m_Scene->m_Registry.valid(m_EnttId); } // #TODO Consider using operator overload
		void Invalidate() { m_EnttId = entt::null; m_Scene = nullptr; }
		static EntityHandle InvalidHandle() { return EntityHandle(); }

		operator bool() const {
			bool result =
				m_EnttId != entt::null &&
				m_Scene &&
				Scenes::GetScene(m_Scene) && // #NOTE Scene transition changes removes this line
				m_Scene->m_Registry.valid(m_EnttId);
				// #TODO Look at adding another case to verify GUID or enttId for object in m_Scene
				// && m_Scene->GetEntityByGuid(EntityGuid()).EntityGuid() != GUID::Invalid;
				// auto& a = m_Scene->GetEntityByGuid(this->EntityGuid()).EntityGuid();
				return result;
		}

		bool EntityHandle::operator == (const EntityHandle& other) const { return other.m_EnttId == m_EnttId; }
		bool EntityHandle::operator != (const EntityHandle& other) const { return other.m_EnttId != m_EnttId; }

	private:
		friend class Scene; // To see m_EnttId

		entt::entity m_EnttId = entt::null;
		Scene* m_Scene = nullptr;
	};

}
