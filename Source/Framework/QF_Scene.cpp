#include "QF_Scene.h"

#include "QC_StringHelpers.h"

#include "QF_CameraComponent.h"
#include "QF_Component.h"
#include "QF_GameObject.h"
#include "QF_Input.h"
#include "QF_Log.h"
#include "QF_Scenes.h"
#include "QF_Serialization.h"
#include "QF_Settings.h"

namespace QwerkE {

    Scene::Scene(const std::string& sceneFileName) :
        m_SceneFileName(sceneFileName)
    {
    }

    Scene::~Scene()
    {
        UnloadScene();
    }

    void Scene::OnWindowResize(unsigned int width, unsigned int height)
    {
        for (unsigned int i = 0; i < m_CameraList.size(); i++)
        {
            ((ComponentCamera*)m_CameraList.at(i)->GetComponent(Component_Camera))->Setup();
            ((ComponentCamera*)m_CameraList.at(i)->GetComponent(Component_Camera))->SetViewportSize(vec2((float)width, (float)height));
        }
    }

    void Scene::Update(float deltatime)
    {
        CameraInput(deltatime);

        if (m_IsPaused)
            return;

        for (auto object : m_pGameObjects)
        {
            object.second->Update(deltatime);
        }
    }

    void Scene::CameraInput(float deltatime)
    {
        // #TODO Put logic in camera component
        ComponentCamera* t_activecamera = ((ComponentCamera*)m_CameraList.at(m_CurrentCameraIndex)->GetComponent(Component_Camera));

        const UserSettings& userSettings = Settings::GetUserSettings();

        if (Input::GetIsKeyDown(userSettings.key_camera_MoveForward))
        {
            t_activecamera->ProcessKeyboard(eCamera_Movement::FORWARD, (float)deltatime);
        }
        if (Input::GetIsKeyDown(userSettings.key_camera_MoveBackward))
        {
            t_activecamera->ProcessKeyboard(eCamera_Movement::BACKWARD, (float)deltatime);
        }
        if (Input::GetIsKeyDown(userSettings.key_camera_MoveLeft))
        {
            t_activecamera->ProcessKeyboard(eCamera_Movement::LEFT, (float)deltatime);
        }
        if (Input::GetIsKeyDown(userSettings.key_camera_MoveRight))
        {
            t_activecamera->ProcessKeyboard(eCamera_Movement::RIGHT, (float)deltatime);
        }
        if (Input::GetIsKeyDown(userSettings.key_camera_MoveDown))
        {
            t_activecamera->ProcessKeyboard(eCamera_Movement::DOWN, (float)deltatime);
        }
        if (Input::GetIsKeyDown(userSettings.key_camera_MoveUp))
        {
            t_activecamera->ProcessKeyboard(eCamera_Movement::UP, (float)deltatime);
        }
        if (Input::GetIsKeyDown(userSettings.key_camera_RotateRight))
        {
            t_activecamera->ProcessKeyboard(eCamera_Movement::RROTATE, (float)deltatime);
        }
        if (Input::GetIsKeyDown(userSettings.key_camera_RotateLeft))
        {
            t_activecamera->ProcessKeyboard(eCamera_Movement::LROTATE, (float)deltatime);
        }
    }

    void Scene::Draw()
    {
        // #TODO Define draw behaviour (Highest first vs lowest first?)

        GameObject* camera = m_CameraList.at(m_CurrentCameraIndex);
        for (size_t i = 0; i < m_SceneDrawList.size(); i++)
        {
            m_SceneDrawList.at(i)->Draw(camera);
        }

        for (size_t i = 0; i < m_LightList.size(); i++)
        {
            m_LightList.at(i)->Draw(camera);
        }
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

    void Scene::SetupCameras()
    {
        for (unsigned int i = 0; i < m_CameraList.size(); i++)
        {
            ((ComponentCamera*)m_CameraList.at(i)->GetComponent(Component_Camera))->Setup();
            ((ComponentCamera*)m_CameraList.at(i)->GetComponent(Component_Camera))->SetTargetPosition(vec3(0, 0, 0));
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
                break;
            }
        }

        newGameObject->SetPosition(vec3(0, 0, 0));
        newGameObject->OnSceneLoaded(this);
        AddObjectToScene(newGameObject);
        return newGameObject;
    }

    void Scene::SaveScene()
    {
        if (m_SceneFileName == gc_DefaultCharPtrValue)
        {
            LOG_ERROR("{0} Unable to save null scene file name!", __FUNCTION__);
            return;
        }

        Serialization::SerializeObjectToFile(*this, ScenesFolderPath(m_SceneFileName.c_str()));
        LOG_INFO("{0} Scene file {1} saved", __FUNCTION__, ScenesFolderPath(m_SceneFileName.c_str()));
        m_IsDirty = false;
    }

    void Scene::LoadSceneFromFile(const char* otherSceneFileName)
    {
        if (m_IsLoaded)
        {
            LOG_ERROR("{0} Scene already loaded!", __FUNCTION__);
            return;
        }

        if (!otherSceneFileName || otherSceneFileName == gc_DefaultCharPtrValue)
        {
            LOG_ERROR("{0} Could not load scene data from null scene file path!", __FUNCTION__);
            return;
        }

        std::string oldName = m_SceneFileName; // #TODO Improve scene file name overwrite logic
        Serialization::DeserializeJsonFromFile(ScenesFolderPath(otherSceneFileName), *this);
        m_SceneFileName = oldName;

        OnLoaded();

        SetupCameras();
        LOG_TRACE("{0} \"{1}\" loaded from file", __FUNCTION__, otherSceneFileName);

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

        if (m_SceneFileName.c_str() == gc_DefaultCharPtrValue)
        {
            LOG_ERROR("Unable to load null scene! sceneFileName value is \"{0}\"", gc_DefaultCharPtrValue);
            return;
        }

        Serialization::DeserializeJsonFromFile(ScenesFolderPath(m_SceneFileName.c_str()), *this);

        OnLoaded();

        SetupCameras();
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
    }

    const GameObject* Scene::GetGameObject(const char* name)
    {
        if (m_pGameObjects.find(name) != m_pGameObjects.end())
            return m_pGameObjects[name];
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
