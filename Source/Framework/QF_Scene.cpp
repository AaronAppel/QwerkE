#include "QF_Scene.h"

#include <algorithm>

#include "QC_StringHelpers.h"

#include "QF_CameraComponent.h"
#include "QF_Component.h"
#include "QF_ConfigHelper.h"
#include "QF_DataManager.h"
#include "QF_GameObject.h"
#include "QF_Input.h"
#include "QF_Log.h"

namespace QwerkE {

    Scene::~Scene()
    {
        UnloadScene();
    }

    void Scene::OnWindowResize(unsigned int width, unsigned int height)
    {
        // update camera view and projection matrices
        for (unsigned int i = 0; i < m_CameraList.size(); i++)
        {
            // ((ComponentCamera*)m_CameraList.At(i)->GetComponent(Component_Camera))->Setup();
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

        const UserData& userData = ConfigHelper::GetUserData();

        if (Input::GetIsKeyDown(userData.controls.Camera_MoveForward))
        {
            t_activecamera->ProcessKeyboard(eCamera_Movement::FORWARD, (float)deltatime);
        }
        if (Input::GetIsKeyDown(userData.controls.Camera_MoveBackward))
        {
            t_activecamera->ProcessKeyboard(eCamera_Movement::BACKWARD, (float)deltatime);
        }
        if (Input::GetIsKeyDown(userData.controls.Camera_MoveLeft))
        {
            t_activecamera->ProcessKeyboard(eCamera_Movement::LEFT, (float)deltatime);
        }
        if (Input::GetIsKeyDown(userData.controls.Camera_MoveRight))
        {
            t_activecamera->ProcessKeyboard(eCamera_Movement::RIGHT, (float)deltatime);
        }
        if (Input::GetIsKeyDown(userData.controls.Camera_MoveDown))
        {
            t_activecamera->ProcessKeyboard(eCamera_Movement::DOWN, (float)deltatime);
        }
        if (Input::GetIsKeyDown(userData.controls.Camera_MoveUp))
        {
            t_activecamera->ProcessKeyboard(eCamera_Movement::UP, (float)deltatime);
        }
        if (Input::GetIsKeyDown(eKeys::eKeys_T))
        {
            t_activecamera->ProcessKeyboard(eCamera_Movement::RROTATE, (float)deltatime);
        }
        if (Input::GetIsKeyDown(eKeys::eKeys_R))
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
            // #TODO Reduce minimum light instance dependency
            LOG_ERROR("Unable to remove light as 1 must remain");
            return;
        }

        for (unsigned int i = 0; i < m_LightList.size(); i++)
        {
            if (m_LightList.at(i) == light) // Pointer comparison
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
        if (object)
            if (m_pGameObjects.find(object->GetName()) == m_pGameObjects.end())
            {
                m_pGameObjects[object->GetName()] = object;
                // TODO: Sort by draw order
                m_SceneDrawList.push_back(object);
                return true; // Success
            }
        return false; // Failure
    }

    void Scene::RemoveObjectFromScene(GameObject* object)
    {
        // TODO: Fix implementation
        // Check object tag
        if (m_pGameObjects.find(object->GetName().c_str()) != m_pGameObjects.end())
        {
            if (object->GetTag() == eGameObjectTags::GO_Tag_Player)
            {
                // Player specific
            }

            std::vector<GameObject*>::iterator it = m_SceneDrawList.begin();
            m_SceneDrawList.erase(std::remove(m_SceneDrawList.begin(), m_SceneDrawList.end(), object), m_SceneDrawList.end());

            m_pGameObjects.erase(object->GetName());
            // TODO: Push object into a pool to be deleted at an appropriate time.
            // Object may be owned externally and should not always be deleted by the scene.
            delete object; // TODO:: Should deletion be handled differently?
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
            LOG_ERROR("Unable to Save scene! m_LevelFileName is \"{0}\"", gc_DefaultCharPtrValue);
            return;
        }

        DataManager::SaveScene(this, ScenesFolderPath(m_SceneFileName.c_str()));
    }

    void Scene::LoadScene()
    {
        LoadScene(m_SceneFileName.c_str());
    }

    void Scene::LoadScene(const char* sceneFileName)
    {
        if (sceneFileName == gc_DefaultCharPtrValue)
        {
            LOG_ERROR("Unable to Load scene! sceneFileName is \"{0}\"", gc_DefaultCharPtrValue);
            return;
        }
        m_SceneFileName = sceneFileName;

        eEngineMessage result = DataManager::LoadScene(this, ScenesFolderPath(m_SceneFileName.c_str()));
        if (result == eEngineMessage::_QSuccess)
        {
            LOG_TRACE("{0} \"{1}\" loaded", __FUNCTION__, m_SceneFileName.c_str());
        }
        else
        {
            LOG_TRACE("{0} error loading \"{1}\"", __FUNCTION__, m_SceneFileName.c_str());
        }
        SetupCameras();
    }

    void Scene::UnloadScene()
    {
        int size = m_LightList.size();
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
            delete object.second;
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

    GameObject* Scene::GetGameObject(const char* name)
    {
        if (m_pGameObjects.find(name) != m_pGameObjects.end())
            return m_pGameObjects[name];

        return nullptr;
    }

}
