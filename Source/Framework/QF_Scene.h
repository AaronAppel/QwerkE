#pragma once

#include <vector>
#include <string>

#include "Libraries/Mirror/Source/Mirror.h"

#include "QF_Enums.h"
#include "QF_Constants.h"

namespace QwerkE {

    class GameCore;
    class MyMatrix;
    class GameObject;

    class Scene final
    {
    public:
        Scene(const char* sceneFileName);
        virtual ~Scene();

        void OnWindowResize(unsigned int width, unsigned int height);

        virtual void Update(float deltatime);
        void Draw();

        bool AddCamera(GameObject* camera);
        void RemoveCamera(GameObject* camera);
        void SetupCameras();

        bool AddLight(GameObject* light);
        void RemoveLight(GameObject* light);

        bool AddObjectToScene(GameObject* object);
        void RemoveObjectFromScene(GameObject* object);

        void SaveScene();
        void LoadScene();
        void UnloadScene();
        void ReloadScene();

        void OnLoaded();

        const GameObject* GetGameObject(const char* name);
        bool GetIsEnabled() { return m_IsEnabled; };
        bool GetIsPaused() { return m_IsPaused; };
        std::vector<GameObject*> GetCameraList() { return m_CameraList; };
        std::map<std::string, GameObject*> GetObjectList() { return m_pGameObjects; };
        int GetCurrentCamera() { return m_CurrentCamera; };
        const std::vector<GameObject*>& GetLightList() const { return m_LightList; };
        const std::string& GetSceneName() { return m_SceneFileName; }

        void SetIsEnabled(bool isEnabled) { m_IsEnabled = isEnabled; };
        void SetIsPaused(bool isPaused) { m_IsPaused = isPaused; };
        void SetCurrentCamera(int camera) { m_CurrentCamera = camera; };

        void ToggleIsPaused() { m_IsPaused = !m_IsPaused; };

    protected:
        void CameraInput(float deltatime);

        bool AddObjectToSceneDrawList(GameObject* object);
        bool RemoveObjectFromSceneDrawList(const GameObject* object);

        MIRROR_PRIVATE_MEMBERS

        bool m_IsEnabled = false;
        bool m_IsPaused = false;
        std::string m_SceneFileName = gc_DefaultCharPtrValue;

        std::map<std::string, GameObject*> m_pGameObjects;

        int m_CurrentCamera = 0;

        std::vector<GameObject*> m_CameraList;
        std::vector<GameObject*> m_LightList;
        std::vector<GameObject*> m_SceneDrawList;
    };

}
