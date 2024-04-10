#include "QF_Scene.h"

#ifdef _QENTT
#include "Libraries/entt/entt.hpp"
#endif

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QC_imgui.h"
#endif

#include "QC_StringHelpers.h"

#include "QF_ComponentCamera.h"
#include "QF_EntityHandle.h"
#include "QF_Files.h"
#include "QF_Input.h"
#include "QF_Log.h"
#include "QF_Paths.h"
#include "QF_Scenes.h"
#include "QF_Serialization.h"
#include "QF_Settings.h"

#include "QF_ComponentInfo.h"
#include "QF_ComponentMesh.h"
#include "QF_ComponentScript.h"
#include "QF_ComponentTransform.h"

#include "QF_Scriptable.h"
#include "QF_ScriptCamera.h"
#include "QF_Scripting.h"

namespace QwerkE {

    Scene::Scene(const std::string& sceneFileName) :
        m_SceneFileName(sceneFileName)
    {
        return;

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
                mesh.Create();
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
            if (!script.m_Bound)
            {
                script.Bind(this, entity);
            }
            script.Update(deltaTime);
        }
    }

    void Scene::Draw()
    {
        EntityHandle cameraHandle(this, m_GuidsToEntts[m_CameraEntityGuid]);
        auto& camera = cameraHandle.GetComponent<ComponentCamera>();
        auto& cameraTransform = cameraHandle.GetComponent<ComponentTransform>();

        const u16 viewId = 2; // #TODO Fix hard coded value
        camera.PreDrawSetup(viewId, cameraTransform.GetPosition());

        auto viewMeshes = m_Registry.view<ComponentMesh>();
        for (auto& entity : viewMeshes)
        {
            ComponentMesh& mesh = m_Registry.get<ComponentMesh>(entity);
            if (m_Registry.has<ComponentTransform>(entity))
            {
                ComponentTransform& transform = m_Registry.get<ComponentTransform>(entity);
                mesh.Draw(transform);
            }
        }
    }

    void Scene::DrawImgui()
    {
        if (ImGui::Begin("SceneDrawImGui"))
        {
            for (auto& guidEnttPairs : m_GuidsToEntts)
            {
                EntityHandle cameraHandle(this, guidEnttPairs.second);
                if (!cameraHandle.HasComponent<ComponentCamera>())
                    continue;

                ComponentInfo& info = cameraHandle.GetComponent<ComponentInfo>();

                if (ImGui::CollapsingHeader(info.m_EditorDisplayName, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ComponentCamera& camera = cameraHandle.GetComponent<ComponentCamera>();
                    auto atCopy = camera.m_At;
                    if (ImGui::DragFloat3("CameraAt", &atCopy.x, .1f))
                    {
                        camera.m_At = atCopy;
                    }

                    ComponentTransform& transform = cameraHandle.GetComponent<ComponentTransform>();
                    vec3f eyeCopy = transform.GetPosition();
                    if (ImGui::DragFloat3("CameraEye", &eyeCopy.x, .1f))
                    {
                        transform.SetPosition(eyeCopy);
                    }

                    float moveSpeedCopy = camera.m_MoveSpeed;
                    if (ImGui::DragFloat("CameraMoveSpeed", &moveSpeedCopy, .1f))
                    {
                        camera.m_MoveSpeed = moveSpeedCopy;
                    }

                    float fovCopy = camera.m_Fov;
                    if (ImGui::DragFloat("CameraFov", &fovCopy, .1f))
                    {
                        camera.m_Fov = fovCopy;
                    }

                    float nearCopy = camera.m_Near;
                    if (ImGui::DragFloat("CameraNear", &nearCopy, .1f))
                    {
                        camera.m_Near = nearCopy;
                    }

                    float farCopy = camera.m_Far;
                    if (ImGui::DragFloat("CameraFar", &farCopy, .1f))
                    {
                        camera.m_Far = farCopy;
                    }
                }
            }
        }
        ImGui::End();
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
        }

        EntityHandle entity = EntityHandle(this, existingGuid);
        const GUID guid = entity.GetComponent<ComponentInfo>().m_Guid;
        m_GuidsToEntts[guid] = entity.m_EnttId;
        return entity;
        // return m_GuidsToEntts.insert(entity->GetComponent<ComponentInfo>().m_Guid, entity).second;
    }

    void Scene::SaveScene()
    {
        if (m_SceneFileName == Constants::gc_DefaultStringValue)
        {
            LOG_ERROR("{0} Unable to save null scene file name!", __FUNCTION__);
            return;
        }

        std::string sceneFilePath = Paths::Scene(m_SceneFileName.c_str());
        Serialization::SerializeObjectToFile(*this, sceneFilePath.c_str());
        LOG_INFO("{0} Scene file {1} saved", __FUNCTION__, sceneFilePath.c_str());
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
            Serialization::DeserializeObjectFromFile(otherSceneFilePath, *this);
        }
        else if (Files::Exists(Paths::Scene(otherSceneFilePath).c_str()))
        {
            Serialization::DeserializeObjectFromFile(Paths::Scene(otherSceneFilePath).c_str(), *this);
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

        Serialization::DeserializeObjectFromFile(Paths::Scene(m_SceneFileName.c_str()).c_str(), *this);

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

        auto viewMeshes = m_Registry.view<ComponentMesh>();
        for (auto& entity : viewMeshes)
        {
            ComponentMesh& mesh = m_Registry.get<ComponentMesh>(entity);
            mesh.Destroy(); // #TODO Assets:: should manage mesh memory
        }

        auto scripts = m_Registry.view<ComponentScript>();
        for (auto& entity : scripts)
        {
            auto& script = m_Registry.get<ComponentScript>(entity);
            script.Unbind();
        }

        m_Registry.each([&](auto entityID)
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
        // #TODO Remove camera component requirement or handle when no camera components exist
        auto viewCameras = m_Registry.view<ComponentCamera>();
        ASSERT(!viewCameras.empty(), "No camera components found in scene!");

        for (auto& guidEnttPair : m_GuidsToEntts)
        {
            if (guidEnttPair.second == viewCameras[0])
            {
                m_CameraEntityGuid = guidEnttPair.first;
                break;
            }
        }
        ASSERT(GUID::Invalid != m_CameraEntityGuid, "Could not find camera component!");

        auto viewMeshes = m_Registry.view<ComponentMesh>();
        for (auto& enttId : viewMeshes)
        {
            viewMeshes.get<ComponentMesh>(enttId).Create(); // #TODO Assets:: should manage mesh memory
        }

        auto viewScripts = m_Registry.view<ComponentScript>();
        for (auto& enttId : viewScripts)
        {
            ComponentScript& script = viewScripts.get<ComponentScript>(enttId);
            script.Bind(this, enttId);
        }
    }

    EntityHandle Scene::GetCurrentCameraEntity()
    {
        return EntityHandle(this, m_GuidsToEntts[m_CameraEntityGuid]);
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
