#include "QF_Scene.h"

#include <algorithm>

#include "QC_StringHelpers.h"

#include "QF_CameraComponent.h"
#include "QF_Component.h"
#include "QF_Factory.h"
#include "QF_GameObject.h"
#include "QF_Input.h"
#include "QF_Log.h"
#include "QF_Serialization.h"
#include "QF_Settings.h"

namespace QwerkE {

    Scene::~Scene()
    {
        UnloadScene();
    }

    void Scene::Initialize()
    {
        switch (m_ID)
        {
        case QwerkE::Scene_TestScene:
            {
                GameObject* newCamera = new GameObject(this);
                Serialization::DeserializeJsonFromFile(ObjectSchematicsFolderPath("Camera.osch"), *newCamera);
                newCamera->SetPosition(vec3(0, 0, 5));
                newCamera->OnSceneLoaded(this);
                this->AddCamera(newCamera);
                Scene::SetupCameras();

                GameObject* newGameObject = new GameObject(this);
                Serialization::DeserializeJsonFromFile(ObjectSchematicsFolderPath("Nanosuit.osch"), *newGameObject);
                newGameObject->SetPosition(vec3(0, -2, 30));
                newGameObject->OnSceneLoaded(this);
                this->AddCamera(newGameObject);

                Factory::CreateTestCube(this, vec3(0, 0, 50));

                GameObject* newLight = new GameObject(this);
                Serialization::DeserializeJsonFromFile(ObjectSchematicsFolderPath("Light.osch"), *newLight);
                newLight->SetPosition(vec3(0, 5, -10));
                newLight->OnSceneLoaded(this);
                this->AddLight(newLight);
            }
            break;

        case QwerkE::Scene_ViewerScene:
            {
                GameObject* newCamera = new GameObject(this);
                Serialization::DeserializeJsonFromFile(ObjectSchematicsFolderPath("Camera.osch"), *newCamera);
                newCamera->SetPosition(vec3(0, 0, 5));
                newCamera->OnSceneLoaded(this);
                this->AddCamera(newCamera);
                Scene::SetupCameras();

                GameObject* newLight = new GameObject(this);
                Serialization::DeserializeJsonFromFile(ObjectSchematicsFolderPath("Light.osch"), *newLight);
                newLight->SetPosition(vec3(0, 0, -10));
                newLight->OnSceneLoaded(this);
                this->AddLight(newLight);
            }
            break;

        case QwerkE::Scene_Max:
        case QwerkE::Scene_Null:
        case QwerkE::Scene_GameScene:
        default:
            LOG_WARN("{0} Scene type unsupported", __FUNCTION__);
            break;
        }
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
        if (m_IsPaused)
            return;

        CameraInput(deltatime);

        for (auto object : m_pGameObjects)
        {
            object.second->Update(deltatime);
        }
    }

    void Scene::CameraInput(float deltatime)
    {
        ComponentCamera* t_activecamera = ((ComponentCamera*)m_CameraList.at(m_CurrentCamera)->GetComponent(Component_Camera));

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

        GameObject* camera = m_CameraList.at(m_CurrentCamera);
        for (int i = m_SceneDrawList.size() - 1; i >= 0; i--)
        {
            m_SceneDrawList.at(i)->Draw(camera);
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
                m_SceneDrawList.erase(m_SceneDrawList.begin() + i); // Lights are drawn
                // TODO: Deallocate memory
                return;
            }
        }
    }

    bool Scene::AddObjectToScene(GameObject* object)
    {
        if (object && m_pGameObjects.find(object->GetName()) == m_pGameObjects.end())
        {
            m_pGameObjects[object->GetName()] = object;
            AddObjectToSceneDrawList(object);
            object->OnSceneLoaded(this);
            return true;
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

    void Scene::RemoveAllObjectsFromScene()
    {
        m_LightList.clear();
        m_CameraList.clear();
        m_pGameObjects.clear();
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
    }

    void Scene::LoadScene()
    {
        if (m_SceneFileName.c_str() == gc_DefaultCharPtrValue)
        {
            LOG_ERROR("Unable to load null scene! sceneFileName value is \"{0}\"", gc_DefaultCharPtrValue);
            return;
        }

        Serialization::DeserializeJsonFromFile(ScenesFolderPath(m_SceneFileName.c_str()), *this);

        OnLoaded();

        SetupCameras();
    }

    void Scene::UnloadScene()
    {
        int size = m_SceneDrawList.size();
        for (int i = size - 1; i > -1; i--)
        {
            delete m_SceneDrawList.at(i);
        }
        m_SceneDrawList.clear();

        size = m_LightList.size();
        for (int i = size - 1; i > -1; i--)
        {
            delete m_LightList.at(i);
        }
        m_LightList.clear();

        size = m_CameraList.size();
        for (int i = size - 1; i > -1; i--)
        {
            delete m_CameraList.at(i);
        }
        m_CameraList.clear();

        for (auto object : m_pGameObjects)
        {
            // delete object.second;
        }
        m_pGameObjects.clear();

        LOG_TRACE("{0} \"{1}\" unloaded", __FUNCTION__, m_SceneFileName.c_str());
    }

    void Scene::ReloadScene()
    {
        UnloadScene();
        Initialize();
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
