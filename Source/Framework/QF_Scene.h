#pragma once

#include <map>
#include <vector>
#include <string>

#include "QF_Enums.h"
#include "QF_Constants.h"

namespace QwerkE {

    class GameCore;
    class MyMatrix;
    class Scenes;
    class GameObject;

    class Scene // #TODO Move all scenes to files as data and declare Scene as final
    {
    public:
        Scene(const char* sceneFileName) { m_SceneFileName = sceneFileName; }
        virtual ~Scene();

        virtual void Initialize() {} // #TODO Deprecate/combine with Load*() methods

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

        void RemoveAllObjectsFromScene();

        void SaveScene();
        void LoadScene();
        void UnloadScene();
        void ReloadScene();

        const GameObject* GetGameObject(const char* name);
        bool GetIsEnabled() { return m_IsEnabled; };
        bool GetIsPaused() { return m_IsPaused; };
        std::vector<GameObject*> GetCameraList() { return m_CameraList; };
        std::map<std::string, GameObject*> GetObjectList() { return m_pGameObjects; };
        int GetCurrentCamera() { return m_CurrentCamera; };
        const std::vector<GameObject*>& GetLightList() const { return m_LightList; };
        eSceneTypes GetSceneID() { return m_ID; }
        const std::string& GetSceneFileName() { return m_SceneFileName; }

        void SetIsEnabled(bool isEnabled) { m_IsEnabled = isEnabled; };
        void SetIsPaused(bool isPaused) { m_IsPaused = isPaused; };
        void SetCurrentCamera(int camera) { m_CurrentCamera = camera; };

        void ToggleIsPaused() { m_IsPaused = !m_IsPaused; };

    protected:
        void CameraInput(float deltatime);

        bool AddObjectToSceneDrawList(GameObject* object);
        bool RemoveObjectFromSceneDrawList(const GameObject* object);

        bool m_IsEnabled = false;
        bool m_IsPaused = false;
        std::string m_SceneFileName = gc_DefaultCharPtrValue;

        std::map<std::string, GameObject*> m_pGameObjects;
        eSceneTypes m_ID = eSceneTypes::Scene_Null; // #TODO Deprecate ids and use unique file names/paths instead

        int m_CurrentCamera = 0;
        std::vector<GameObject*> m_CameraList;

        std::vector<GameObject*> m_LightList;

        std::vector<GameObject*> m_SceneDrawList;
    };

}
