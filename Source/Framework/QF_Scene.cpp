#include "QF_Scene.h"

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QwerkE_imgui.h"
#endif

#ifdef _QENTT
#include "Libraries/entt/entt.hpp"
#endif

#ifdef _QJOLT
#include "Libraries/Jolt/Physics/PhysicsSystem.h"
#endif

#include "QC_StringHelpers.h"

#include "QF_ComponentHelpers.h"
#include "QF_EntityHandle.h"
#include "QF_Files.h"
#include "QF_Paths.h"
#include "QF_Scenes.h"
#include "QF_Serialize.h"

#include "QF_Scriptable.h"
#include "QF_ScriptCamera.h"
#include "QF_Scripting.h"
#include "../Source/Editor/QE_Settings.h" // #TODO Remove from QF_* domain

namespace QwerkE {

    template <typename... Component>
    void CopyEntityComponent(const EntityHandle& a_SourceEntity, EntityHandle& a_TargetEntity)
    {
        ([&]() {
            // const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<Component>();
            if (a_SourceEntity.HasComponent<Component>())
            {
                if (!a_TargetEntity.HasComponent<Component>())
                {
                    a_TargetEntity.AddComponent<Component>(); // #TODO Could pass component reference to copy when allocating
                }

                if (std::is_same_v<Component, ComponentInfo>)
                {
                    ComponentInfo* info = &a_TargetEntity.GetComponent<ComponentInfo>();
                    info->m_EntityName = a_SourceEntity.GetComponent<ComponentInfo>().m_EntityName;

                    // #TODO Write all other values but avoid copying guid, and string as a shallow copy to avoid multiple string deletions
                    // memcpy(&a_TargetEntity.GetComponent<Component>(), &a_SourceEntity.GetComponent<Component>(), sizeof(Component));
                }
                else
                {
                    memcpy(&a_TargetEntity.GetComponent<Component>(), &a_SourceEntity.GetComponent<Component>(), sizeof(Component));
                }
        }
        }(), ...);
    }

    template<typename... Components>
    void CopyEntityComponents(TemplateArgumentList<Components...>, const EntityHandle& a_SourceEntity, EntityHandle& a_TargetEntity)
    {
        CopyEntityComponent<Components...>(a_SourceEntity, a_TargetEntity);
    }

    Scene::~Scene()
    {
        UnloadScene();
    }

    void Scene::Update(float deltaTime)
    {
        if (m_IsPaused)
            return;

        auto scripts = m_Registry.view<ComponentScript>();
        for (auto& entity : scripts)
        {
            ComponentInfo& info = m_Registry.get<ComponentInfo>(entity);
            if (info.m_Enabled)
            {
                auto& script = m_Registry.get<ComponentScript>(entity);
                script.Update(deltaTime);
            }
        }
    }

    void Scene::Draw(u16 viewId)
    {
        if (!m_CameraEntityGuid)
        {
            auto viewCameraInfos = m_Registry.view<ComponentCamera, ComponentInfo>();
            for (const entt::entity& entity : viewCameraInfos)
            {
                ComponentInfo& info = m_Registry.get<ComponentInfo>(entity);
                m_CameraEntityGuid = info.m_Guid;
            }
        }

        if (m_CameraEntityGuid)
        {
            EntityHandle cameraHandle(this, m_GuidsToEntts[m_CameraEntityGuid]);
            if (cameraHandle && cameraHandle.HasComponent<ComponentCamera>())
            {
                auto& camera = cameraHandle.GetComponent<ComponentCamera>();
                auto& cameraTransform = cameraHandle.GetComponent<ComponentTransform>();

                camera.PreDrawSetup(viewId, cameraTransform.GetPosition());
            }

            auto viewMeshes = m_Registry.view<ComponentMesh>();
            for (const entt::entity& entity : viewMeshes)
            {
                ComponentInfo& info = m_Registry.get<ComponentInfo>(entity);
                if (info.m_Enabled)
                {
                    ComponentMesh& mesh = m_Registry.get<ComponentMesh>(entity);
                    if (m_Registry.has<ComponentTransform>(entity))
                    {
                        ComponentTransform& transform = m_Registry.get<ComponentTransform>(entity);
                        mesh.Draw(viewId, transform);
                    }
                }
            }
        }
    }

    void Scene::Draw(ComponentCamera camera, vec3f position, u16 viewId)
    {
        camera.PreDrawSetup(viewId, position);

        auto viewMeshes = m_Registry.view<ComponentMesh>();
        for (const entt::entity& entity : viewMeshes)
        {
            ComponentInfo& info = m_Registry.get<ComponentInfo>(entity);
            if (info.m_Enabled)
            {
                ComponentMesh& mesh = m_Registry.get<ComponentMesh>(entity);
                if (m_Registry.has<ComponentTransform>(entity))
                {
                    ComponentTransform& transform = m_Registry.get<ComponentTransform>(entity);
                    mesh.Draw(viewId, transform);
                }
            }
        }
    }

    EntityHandle Scene::CreateEntity()
    {
        EntityHandle entity = EntityHandle(this);
        const GUID guid = entity.GetComponent<ComponentInfo>().m_Guid;
        m_GuidsToEntts[guid] = entity.m_EnttId;
        return entity;
        // return m_GuidsToEntts.insert(entity->GetComponent<ComponentInfo>().m_Guid, entity).second;
    }

    EntityHandle Scene::CreateEntity(const GUID& existingGuid)
    {
        if (m_GuidsToEntts.find(existingGuid) != m_GuidsToEntts.end())
        {
            LOG_ERROR("{0} GUID already exists!", __FUNCTION__);
            return EntityHandle(this, m_GuidsToEntts[existingGuid]);
        }

        m_GuidsToEntts[existingGuid] = m_Registry.create();
        return EntityHandle(this, existingGuid);
    }

    EntityHandle Scene::DuplicateEntity(const EntityHandle& a_ExistingEntity)
    {
        if (!a_ExistingEntity)
        {
            LOG_ERROR("Invalid entity to duplicate!");
            return EntityHandle::InvalidHandle();
        }

        EntityHandle newEntity = EntityHandle(this);
        CopyEntityComponents(EntityComponentsList{}, a_ExistingEntity, newEntity);
        m_GuidsToEntts[newEntity.GetComponent<ComponentInfo>().m_Guid] = newEntity.m_EnttId;

        return newEntity;
    }


    void Scene::DestroyEntity(EntityHandle& entity)
    {
        if (m_Registry.valid(entity.m_EnttId))
        {
            if (entity.EntityGuid() == m_CameraEntityGuid)
            {
                m_CameraEntityGuid = GUID::Invalid;
            }

            m_GuidsToEntts.erase(entity.EntityGuid());
            m_Registry.destroy(entity.m_EnttId);

            entity.Invalidate();

            return;
        }
        LOG_WARN("{0} Could not destroy entity with entt ID {1}", __FUNCTION__, (u32)entity.m_EnttId);
    }

    EntityHandle Scene::GetEntityByGuid(const GUID& existingGuid)
    {
        if (m_GuidsToEntts.find(existingGuid) != m_GuidsToEntts.end())
        {
            return EntityHandle(this,  m_GuidsToEntts[existingGuid]);
        }
        return EntityHandle::InvalidHandle();
    }

    void Scene::SaveScene()
    {
        if (m_SceneFileName == Constants::gc_DefaultStringValue)
        {
            LOG_ERROR("{0} Null scene file name!", __FUNCTION__);
            return;
        }

        std::string sceneFilePath = Paths::Scene(m_SceneFileName.c_str());
        Serialize::ToFile(*this, sceneFilePath.c_str());
        LOG_TRACE("{0} Scene file {1} saved", __FUNCTION__, sceneFilePath.c_str());
        m_IsDirty = false;
    }

    void Scene::LoadSceneFromFilePath(const char* otherSceneFilePath)
    {
        if (m_IsLoaded)
        {
            LOG_ERROR("{0} Scene already loaded!", __FUNCTION__);
            return;
        }

        if (!otherSceneFilePath || otherSceneFilePath == Constants::gc_DefaultStringValue)
        {
            LOG_ERROR("{0} Could not load scene data from null scene file path!", __FUNCTION__);
            return;
        }

        std::string oldName = m_SceneFileName; // #TODO Improve scene file name overwrite logic
        if (Files::Exists(otherSceneFilePath))
        {
            Serialize::FromFile(otherSceneFilePath, *this);
        }
        else if (Files::Exists(Paths::Scene(otherSceneFilePath).c_str()))
        {
            Serialize::FromFile(Paths::Scene(otherSceneFilePath).c_str(), *this);
        }
        else
        {
            LOG_ERROR("{0} Could not find scene file path {1} to load", __FUNCTION__, otherSceneFilePath);
            return;
        }

        m_SceneFileName = oldName; // #NOTE Overwrite serialized name

        OnLoaded();

        LOG_TRACE("{0} \"{1}\" loaded from file", __FUNCTION__, otherSceneFilePath);

        m_IsLoaded = true;
        m_IsDirty = false;
    }

    void Scene::LoadScene()
    {
        if (m_IsLoaded)
        {
            LOG_ERROR("{0} Scene already loaded!", __FUNCTION__);
            return;
        }

        if (m_SceneFileName.c_str() == Constants::gc_DefaultStringValue)
        {
            LOG_ERROR("Unable to load null scene! sceneFileName value is \"{0}\"", Constants::gc_DefaultStringValue);
            return;
        }
        else
        {
            Serialize::FromFile(Paths::Scene(m_SceneFileName.c_str()).c_str(), *this);
        }

        OnLoaded();

        LOG_TRACE("{0} \"{1}\" loaded", __FUNCTION__, m_SceneFileName.c_str());

        m_IsLoaded = true;
        m_IsDirty = false;
    }

    void Scene::UnloadScene()
    {
        if (!m_IsLoaded)
        {
            LOG_ERROR("{0} Scene is not loaded!", __FUNCTION__);
            return;
        }

        auto scripts = m_Registry.view<ComponentScript>();
        for (auto& entity : scripts)
        {
            auto& script = m_Registry.get<ComponentScript>(entity);
            script.Unbind();
        }

        for (auto& guidEntityPair : m_GuidsToEntts)
        {
            // m_GuidsToEntts.erase(guidEntityPair.first);
            m_Registry.destroy(guidEntityPair.second);
        }
        m_GuidsToEntts.clear();

        m_Registry.each([&](const auto entityID)
        {
            m_Registry.destroy(entityID);
        });

        m_CameraEntityGuid = GUID::Invalid;

        m_IsLoaded = false;
        m_IsDirty = false;
        ASSERT(m_GuidsToEntts.empty(), "Leftover data from Scene::Unload()!");
        LOG_TRACE("{0} \"{1}\" unloaded", __FUNCTION__, m_SceneFileName.c_str());
    }

    void Scene::ReloadScene()
    {
        UnloadScene();
        LoadScene();
        LOG_TRACE("{0} \"{1}\" reloaded", __FUNCTION__, m_SceneFileName.c_str());
    }

    void Scene::OnLoaded()
    {
        ASSERT(m_GuidsToEntts.empty(), "Leftover data from Scene::Unload()!");

        m_Registry.each([&](const auto entityId) {
            ComponentInfo& info = m_Registry.get<ComponentInfo>(entityId);
            m_GuidsToEntts.insert({ info.m_Guid , entityId });

            if (m_Registry.has<ComponentScript>(entityId))
            {
                ComponentScript& script = m_Registry.get<ComponentScript>(entityId);
                script.Bind(EntityHandle(this, entityId));
            }
        });

        // #TODO Remove camera component requirement or handle when no camera components exist
        auto viewCameras = m_Registry.view<ComponentCamera>();
        // ASSERT(!viewCameras.empty(), "No camera components found in scene!");

        for (auto& guidEnttPair : m_GuidsToEntts)
        {
            if (guidEnttPair.second == viewCameras[0])
            {
                m_CameraEntityGuid = guidEnttPair.first;
                break;
            }
        }
        // ASSERT(GUID::Invalid != m_CameraEntityGuid, "Could not find camera component!");
    }

    EntityHandle Scene::GetCurrentCameraEntity()
    {
        if (m_GuidsToEntts.find(m_CameraEntityGuid) != m_GuidsToEntts.end())
        {
            return EntityHandle(this, m_GuidsToEntts[m_CameraEntityGuid]);
        }
        return EntityHandle::InvalidHandle();
    }

    void Scene::SetCurrentCameraEntity(EntityHandle newCameraEntity)
    {
        if (newCameraEntity.HasComponent<ComponentCamera>())
        {
            const GUID newCameraGuid = newCameraEntity.GetComponent<ComponentInfo>().m_Guid;
            if (m_GuidsToEntts.find(newCameraGuid) != m_GuidsToEntts.end())
            {
                m_GuidsToEntts[newCameraGuid] = newCameraEntity.m_EnttId;
            }
            m_CameraEntityGuid = newCameraGuid;
        }
    }

}
