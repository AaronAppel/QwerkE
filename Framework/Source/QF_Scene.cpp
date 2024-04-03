#include "QF_Scene.h"

#ifdef _QENTT
#include "Libraries/entt/entt.hpp"
#endif

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QC_imgui.h"
#endif

#include "QC_StringHelpers.h"

#include "QF_ComponentCamera.h"
#include "QF_Entity.h"
#include "QF_Files.h"
#include "QF_GameObject.h"
#include "QF_Input.h"
#include "QF_Log.h"
#include "QF_Scenes.h"
#include "QF_Serialization.h"
#include "QF_Settings.h"

#include "QF_ComponentMesh.h"
#include "QF_ComponentScript.h"
#include "QF_ComponentTransform.h"

#include "QF_Scriptable.h"
#include "QF_ScriptCamera.h"

namespace QwerkE {

    Scene::Scene(const std::string& sceneFileName) :
        m_SceneFileName(sceneFileName)
    {
        m_EntityCamera = m_Registry.create();
        m_CameraEntity = new Entity(this, m_EntityCamera);

        const bool hasComponent = m_CameraEntity->HasComponent<ComponentCamera>();

        m_Entities.insert(std::pair(m_EntityCamera, m_CameraEntity));

        m_Registry.emplace<ComponentCamera>(m_EntityCamera, ComponentCamera());
        const bool nowHasComponent = m_CameraEntity->HasComponent<ComponentCamera>();

        m_CameraEntity->AddComponent<ComponentTransform>();
        m_CameraEntity->AddComponent<ComponentScript>().Bind<ScriptableCamera>();

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

                m_SelectedObject = entityId;
            }
        }
    }

    Scene::~Scene()
    {
        UnloadScene();

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

        for (auto& pair : m_Entities)
        {
            if (pair.second)
            {
                delete pair.second;
                pair.second = nullptr;
            }
        }
        m_Entities.clear();

        // delete m_CameraEntity;
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
                script.Instance = script.InstantiateScript();
                script.Instance->SetEntity(m_Entities[entity]);
                script.Instance->OnCreate();
            }
            script.Instance->OnUpdate(deltaTime);
        }

        if (false && Input::GetIsKeyDown(eKeys_W))
        {
            ComponentTransform& transform = m_Registry.get<ComponentTransform>(m_SelectedObject);
            const float moveSpeed = 3.f;
            vec3f tempTransform = transform.GetPosition();
            tempTransform.x = tempTransform.x + (moveSpeed * deltaTime);
            transform.SetPosition(tempTransform);
        }

        for (auto object : m_pGameObjects)
        {
            object.second->Update(deltaTime);
        }
    }

    void Scene::Draw()
    {
        auto camera = m_Registry.get<ComponentCamera>(m_EntityCamera);
        camera.PreDrawSetup(2);

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
                auto& camera = m_Registry.get<ComponentCamera>(m_EntityCamera);
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

    bool Scene::AddCamera(GameObject* camera)
    {
        m_CameraList.push_back(camera);
        return true;
    }

    void Scene::RemoveCamera(GameObject* camera)
    {
        if (m_CameraList.size() < 2)
        {
            LOG_ERROR("Unable to remove camera as 1 must remain");
            return;
        }

        for (unsigned int i = 0; i < m_CameraList.size(); i++)
        {
            if (m_CameraList.at(i) == camera)
            {
                m_CameraList.erase(m_CameraList.begin() + i);
                // #TODO Delete?
                return;
            }
        }
    }

    bool Scene::AddLight(GameObject* light)
    {
        // #TODO Insert in/by render order, or sort after all objects have been added

        m_LightList.push_back(light);
        m_SceneDrawList.push_back(light);
        return true;
    }

    void Scene::RemoveLight(GameObject* light)
    {
        if (m_LightList.size() < 2)
        {
            LOG_ERROR("Unable to remove light as 1 must remain"); // #BUG B0001
            return;
        }

        for (unsigned int i = 0; i < m_LightList.size(); i++)
        {
            if (m_LightList.at(i) == light)
            {
                m_LightList.erase(m_LightList.begin() + i);
                auto it = m_SceneDrawList.erase(m_SceneDrawList.begin() + i);
                // #TODO Delete object
                return;
            }
        }
    }

    bool Scene::ObjectWithNameExists(GameObject* object)
    {
        if (!object)
            return true;

        return m_pGameObjects.find(object->GetName()) != m_pGameObjects.end();
    }

    bool Scene::AddObjectToScene(GameObject* object)
    {
        if (object)
        {
            if (ObjectWithNameExists(object))
            {
                LOG_WARN("{0} Object with name \"{1}\" already exists in scene{2}", __FUNCTION__, object->GetName().c_str(), this->GetSceneName().c_str());
            }
            else
            {
                m_pGameObjects[object->GetName()] = object;
                AddObjectToSceneDrawList(object);
                object->OnSceneLoaded(this);
                SetDirty();
                return true;
            }
        }
        else
        {
            LOG_ERROR("{0} Object was null!", __FUNCTION__);
        }
        return false;
    }

    void Scene::RemoveObjectFromScene(GameObject* object)
    {
        if (m_pGameObjects.find(object->GetName().c_str()) != m_pGameObjects.end())
        {
            RemoveObjectFromSceneDrawList(object);
            m_pGameObjects.erase(object->GetName());
            delete object;
        }
    }

    Entity* Scene::CreateEntity()
    {
        entt::entity enttEntity = m_Registry.create();
        // #TODO Add anycomponents like a transform. Also look at a no-transform version for directors
        // Could take in an optional tag/enum value to assign
        Entity* entity = new Entity(this, enttEntity);
        m_Entities.insert(std::pair(enttEntity, entity));
        return m_Entities.find(enttEntity)->second;
    }

    GameObject* Scene::CreateNewObject()
    {
        GameObject* newObject = new GameObject(this);

        return newObject;
    }

    GameObject* Scene::CreateNewObjectFromSchematic(const char* const schematicFileName)
    {
        GameObject* newGameObject = new GameObject(this);
        Serialization::DeserializeJsonFromFile(SchematicsFolderPath(schematicFileName), *newGameObject);

        while (ObjectWithNameExists(newGameObject))
        {
            char* newName = NumberAppendOrIncrement(newGameObject->GetName().c_str());
            if (newName)
            {
                newGameObject->SetName(newName); // #TODO Handle memory allocations
                delete[] newName;
            }
            else
            {
                LOG_ERROR("{0} Unable to name new game object!", __FUNCTION__);
                delete newGameObject;
                return nullptr;
            }
        }

        newGameObject->OnSceneLoaded(this);
        AddObjectToScene(newGameObject);
        return newGameObject;
    }

    void Scene::SaveScene()
    {
        if (m_SceneFileName == Constants::gc_DefaultStringValue)
        {
            LOG_ERROR("{0} Unable to save null scene file name!", __FUNCTION__);
            return;
        }

        Serialization::SerializeScene(*this, StringAppend(ScenesFolderPath(m_SceneFileName.c_str()), ".", scene_ext));

        Serialization::SerializeObjectToFile(*this, ScenesFolderPath(m_SceneFileName.c_str()));
        LOG_INFO("{0} Scene file {1} saved", __FUNCTION__, ScenesFolderPath(m_SceneFileName.c_str()));
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
            Serialization::DeserializeJsonFromFile(otherSceneFilePath, *this);
        }
        else if (Files::Exists(ScenesFolderPath(otherSceneFilePath)))
        {
            Serialization::DeserializeJsonFromFile(ScenesFolderPath(otherSceneFilePath), *this);
        }
        else
        {
            LOG_ERROR("{0} Could not find scene file path {1} to load", __FUNCTION__, otherSceneFilePath);
            return;
        }

        m_SceneFileName = oldName;

        Serialization::DeserializeScene(ScenesFolderPath(otherSceneFilePath), *this);

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

        Serialization::DeserializeJsonFromFile(ScenesFolderPath(m_SceneFileName.c_str()), *this);

        Serialization::DeserializeScene(StringAppend(ScenesFolderPath(m_SceneFileName.c_str()), ".", scene_ext), *this);

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

        for (size_t i = 0; i < m_CameraList.size(); i++)
        {
            delete m_CameraList.at(i);
        }
        m_CameraList.clear();

        for (auto object : m_pGameObjects)
        {
            delete object.second;
        }
        m_pGameObjects.clear();

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

    void Scene::OnLoaded() // #TODO Improve load logic. These callbacks are to resolve serialization effects
    {
        for (size_t i = 0; i < m_SceneDrawList.size(); i++)
        {
            m_SceneDrawList[i]->OnSceneLoaded(this);
        }
        for (size_t i = 0; i < m_CameraList.size(); i++)
        {
            m_CameraList[i]->OnSceneLoaded(this);
        }
        for (size_t i = 0; i < m_LightList.size(); i++)
        {
            m_LightList[i]->OnSceneLoaded(this);
        }

        for (size_t i = 0; i < m_SceneDrawList.size(); i++)
        {
            if (m_pGameObjects.find(m_SceneDrawList[i]->GetName()) == m_pGameObjects.end())
            {
                if (m_SceneDrawList[i]->GetTag() != eGameObjectTags::GO_Tag_Light)
                {
                    m_pGameObjects[m_SceneDrawList[i]->GetName()] = m_SceneDrawList[i];
                }
            }
        }

        auto viewCameras = m_Registry.view<ComponentCamera>();
        for (auto& entity : viewCameras)
        {
            // #TODO Setup cameras and references
        }
        m_EntityCamera = viewCameras[0];
    }

    const GameObject* Scene::GetGameObject(const char* name)
    {
        if (m_pGameObjects.find(name) != m_pGameObjects.end())
            return m_pGameObjects[name];
        return nullptr;
    }

    Entity* Scene::CreateEntityFromSchematic(const char* schematicFilePath)
    {
        GameObject obj;
        Serialization::DeserializeJsonFromFile(schematicFilePath, obj);
        return nullptr;
    }

    bool Scene::AddObjectToSceneDrawList(GameObject* object)
    {
        // #TODO Sort by draw order
        m_SceneDrawList.push_back(object);
        return true;
    }

    bool Scene::RemoveObjectFromSceneDrawList(const GameObject* object)
    {
        for (auto it = m_SceneDrawList.begin(); it != m_SceneDrawList.end(); it++)
        {
            if (*it == object)
            {
                m_SceneDrawList.erase(it);
                return true;
            }
        }
        return false;
    }

}
