#pragma once

#include <map>
#include <string>
#include <vector>

#include "Libraries/entt/entt.hpp"
#include "Libraries/Mirror/Source/Mirror.h"

#include "QF_GameObject.h" // #TODO Remove

#include "QF_Constants.h"

namespace QwerkE {

    class GameObject;

    class Scene final
    {
    public:
        Scene(const std::string& sceneFileName);
        ~Scene();

        void OnWindowResize(unsigned int width, unsigned int height);

        void Update(float deltatime);
        void Draw();

        bool AddCamera(GameObject* camera);
        void RemoveCamera(GameObject* camera);
        void SetupCameras();

        bool AddLight(GameObject* light);
        void RemoveLight(GameObject* light);

        bool ObjectWithNameExists(GameObject* object);
        bool AddObjectToScene(GameObject* object);
        void RemoveObjectFromScene(GameObject* object);

        GameObject* CreateNewObject();
        GameObject* CreateNewObjectFromSchematic(const char* const schematicFileName);

        void SaveScene();
        void LoadSceneFromFile(const char* otherSceneFileName);
        void LoadScene();
        void UnloadScene();
        void ReloadScene();

        void OnLoaded();

        const GameObject* GetGameObject(const char* name);
        bool GetIsPaused() { return m_IsPaused; };
        std::vector<GameObject*> GetCameraList() { return m_CameraList; };
        GameObject* GetCurrentCamera() { return m_CameraList[m_CurrentCameraIndex]; }
        const std::map<std::string, GameObject*>& GetObjectList() { return m_pGameObjects; };
        const std::vector<GameObject*>& GetLightList() const { return m_LightList; };
        const std::string& GetSceneName() const { return m_SceneFileName; }

        void SetIsPaused(bool isPaused) { m_IsPaused = isPaused; };
        void SetCurrentCamera(int newCameraIndex) { m_CurrentCameraIndex = newCameraIndex; };

        void ToggleIsPaused() { m_IsPaused = !m_IsPaused; };

        void SetDirty() { m_IsDirty = true; }
        bool IsDirty() { return m_IsDirty; }

    private:
        void CameraInput(float deltatime);

        bool AddObjectToSceneDrawList(GameObject* object);
        bool RemoveObjectFromSceneDrawList(const GameObject* object);

        entt::registry m_Registry;
        entt::entity m_EntityCamera = entt::null;

        MIRROR_PRIVATE_MEMBERS

        bool m_IsPaused = false;
        std::string m_SceneFileName = gc_DefaultCharPtrValue;

        std::map<std::string, GameObject*> m_pGameObjects; // #TODO Deprecate

        int m_CurrentCameraIndex = 0;

        std::vector<GameObject*> m_CameraList;
        std::vector<GameObject*> m_LightList;
        std::vector<GameObject*> m_SceneDrawList;

        bool m_IsLoaded = false;
        bool m_IsDirty = false; // #TODO Editor only
    };

}
