#pragma once

#include <map>
#include <string>
#include <vector>

#ifdef _QENTT
#include "Libraries/entt/entt.hpp"
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
#endif

#include "QC_Guid.h"

class PhysicsSystem;

namespace QwerkE {

    class EntityHandle;

    class Scene final
    {
    public:
        Scene(const std::string& sceneFileName);
        ~Scene();

        void Update(float deltatime);
        void Draw(u16 viewId = 0);

        EntityHandle CreateEntity();
        EntityHandle CreateEntity(const GUID& existingGuid);

        void DestroyEntity(EntityHandle& entity);

        EntityHandle GetEntityByGuid(const GUID& existingGuid);

        void SaveScene();
        void LoadSceneFromFilePath(const char* otherSceneFileName);
        void LoadScene();
        void UnloadScene();
        void ReloadScene();

        void OnLoaded();

        bool GetIsPaused() { return m_IsPaused; }
        const std::string& GetSceneName() const { return m_SceneFileName; }

        void SetIsPaused(bool isPaused) { m_IsPaused = isPaused; }

        void ToggleIsPaused() { m_IsPaused = !m_IsPaused; }

        void SetDirty() { m_IsDirty = true; }
        bool IsDirty() { return m_IsDirty; }

        // #NOTE Scene transition changes
        // bool IsActive() { return m_IsActive; }
        // void SetIsActive(bool isActive) { m_IsActive = isActive; }

        template<typename... Components>
        auto ViewComponents()
        {
            return m_Registry.view<Components...>();
        }

        const std::unordered_map<GUID, entt::entity>& EntitiesMap() { return m_GuidsToEntts; }

        EntityHandle GetCurrentCameraEntity();
        void SetCurrentCameraEntity(EntityHandle newCameraEntity);

        const GUID& GetGuid() { return m_SceneGuid; }

        // #NOTE Scene transition changes
        // const GUID& GetGuid() const { return m_SceneGuid; }

        // void SetDirty() { m_IsDirty = true; } // #TODO Editor only state. Move out of here and into some QE_ file domain
        // bool IsDirty() { return m_IsDirty; } // #TODO Editor only state. Move out of here and into some QE_ file domain

    private:
        MIRROR_PRIVATE_MEMBERS
        friend class EntityHandle; // #TODO Review. Remove public registry if proper, and expose entity map instead
        friend class SceneCreator;

        GUID m_CameraEntityGuid = GUID::Invalid;

        entt::registry m_Registry;
        std::unordered_map<GUID, entt::entity> m_GuidsToEntts;

        std::string m_SceneFileName = Constants::gc_DefaultStringValue;

        GUID m_SceneGuid;

        PhysicsSystem* m_PhysicsSystem = nullptr;

        bool m_IsLoaded = false;
        bool m_IsDirty = false; // #TODO Editor only state. Move out of here
        bool m_IsPaused = false; // #TODO Move state to editor as it's really for higher control (editor, or cutscene state, etc)
    };

}
