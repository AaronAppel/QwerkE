#include "QF_Scene.h"

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QC_imgui.h"
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
#include "QF_Input.h"
#include "QF_Log.h"
#include "QF_Paths.h"
#include "QF_Scenes.h"
#include "QF_Serialize.h"

#include "QF_Scriptable.h"
#include "QF_ScriptCamera.h"
#include "QF_Scripting.h"
#include "../Source/Editor/QE_Settings.h" // #TODO Remove from QF_* domain

namespace QwerkE {

    Scene::Scene(const std::string& sceneFileName) :
        m_SceneFileName(sceneFileName)
    {
        // m_PhysicsSystem = new PhysicsSystem();

        return;

        // #TESTING Code below is for testing mesh generation
        const u8 rows = 11;
        const u8 columns = 11;
        const float spacingScalar = 3.f;
        const float offset = 15.f;

        for (uint32_t yy = 0; yy < columns; ++yy)
        {
            for (uint32_t xx = 0; xx < rows; ++xx)
            {
                entt::entity entityId = m_Registry.create();
                ComponentTransform& transform = m_Registry.emplace<ComponentTransform>(entityId);
                transform.SetPosition({ float(xx) * spacingScalar - offset, float(yy) * spacingScalar - offset, .0f });

                ComponentMesh& mesh = m_Registry.emplace<ComponentMesh>(entityId);
            }
        }
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
            auto& script = m_Registry.get<ComponentScript>(entity);
            script.Update(deltaTime);
        }
    }

    void Scene::Draw(u16 viewId)
    {
        if (!m_CameraEntityGuid)
        {
            auto viewCameraInfos = m_Registry.view<ComponentCamera, ComponentInfo>();
            for (auto& entity : viewCameraInfos)
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
            for (auto& entity : viewMeshes)
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

        m_SceneFileName = oldName;

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

        Serialize::FromFile("NewSerialization", *this);

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

        m_Registry.each([&](const auto entityID)
        {
            m_Registry.destroy(entityID);
        });

        m_IsLoaded = false;
        m_IsDirty = false;
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
        m_GuidsToEntts.clear(); // #TODO Error if not empty?
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
