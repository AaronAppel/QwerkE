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

namespace QwerkE {

    Scene::Scene(const std::string& sceneFileName) :
        m_SceneFileName(sceneFileName)
    {
        EntityHandle cameraEntity = EntityHandle(this);
        ComponentInfo& info = cameraEntity.GetComponent<ComponentInfo>();
        info.m_EditorDisplayName = "Camera 0";
        m_GuidsToEntts[info.m_Guid] = cameraEntity.m_EnttId;
        m_CameraEntityGuid = info.m_Guid;

        ComponentCamera& camera = cameraEntity.AddComponent<ComponentCamera>();
        cameraEntity.AddComponent<ComponentScript>().Bind<ScriptableCamera>();

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

        // #TODO Should UnloadScene() also do the work below?
        auto viewMeshes = m_Registry.view<ComponentMesh>();
        for (auto entity : viewMeshes)
        {
            ComponentMesh& mesh = m_Registry.get<ComponentMesh>(entity);
            mesh.Destroy();
        }

        auto viewScripts = m_Registry.view<ComponentScript>();
        for (auto entity : viewScripts)
        {
            ComponentScript& script = m_Registry.get<ComponentScript>(entity);
            if (script.Instance)
            {
                script.Instance->OnDestroy();
                script.DeleteScript(&script); // #TODO Bug check self delete
            }
        }
    }

    void Scene::Update(float deltaTime)
    {
        if (m_IsPaused)
            return;

        auto scripts = m_Registry.view<ComponentScript>();
        for (auto& entity : scripts)
        {
            auto& script = m_Registry.get<ComponentScript>(entity);
            if (!script.Instance)
            {
                auto& info = m_Registry.get<ComponentInfo>(entity);
                script.Instance = script.InstantiateScript();
                script.Instance->SetEntity(EntityHandle(this, m_GuidsToEntts[info.m_Guid]));
                script.Instance->OnCreate();
            }
            script.Instance->OnUpdate(deltaTime);
        }
    }

    void Scene::Draw()
    {
        const u8 viewId = 2; // #TODO Fill viewId properly
        auto& camera = m_Registry.get<ComponentCamera>(m_GuidsToEntts[m_CameraEntityGuid]);
        camera.PreDrawSetup(viewId);

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
            if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
            {
                auto& camera = m_Registry.get<ComponentCamera>(m_GuidsToEntts[m_CameraEntityGuid]);

                auto atCopy = camera.m_At;
                if (ImGui::DragFloat3("CameraAt", &atCopy.x, .1f))
                {
                    camera.m_At = atCopy;
                }

                auto eyeCopy = camera.m_Eye;
                if (ImGui::DragFloat3("CameraEye", &eyeCopy.x, .1f))
                {
                    camera.m_Eye = eyeCopy;
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

    void Scene::SaveScene()
    {
        if (m_SceneFileName == Constants::gc_DefaultStringValue)
        {
            LOG_ERROR("{0} Unable to save null scene file name!", __FUNCTION__);
            return;
        }

        std::string sceneFilePath = Paths::Scene(m_SceneFileName.c_str());
        Serialization::SerializeScene(*this, sceneFilePath.c_str());
        // Serialization::SerializeScene(*this, StringAppend(ScenesFolderPath(m_SceneFileName.c_str()), ".", scene_ext));

        Serialization::SerializeObjectToFile(*this, Paths::Schematic(m_SceneFileName.c_str()).c_str());
        LOG_INFO("{0} Scene file {1} saved", __FUNCTION__, Paths::Scene(m_SceneFileName.c_str()));
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
            // Serialization::DeserializeJsonFromFile(otherSceneFilePath, *this);

            std::string sceneFilePath = Paths::Scene(m_SceneFileName.c_str());
            Serialization::DeserializeScene(sceneFilePath.c_str(), *this);
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

        std::string sceneFilePath = Paths::Scene(m_SceneFileName.c_str());
        Serialization::DeserializeScene(sceneFilePath.c_str(), *this);

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
