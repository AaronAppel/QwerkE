#pragma once

#include <string>
#include <vector>

#include "QF_Enums.h"

namespace QwerkE {

    class Scene;

    namespace Scenes {

        void Initialize();
        void Shutdown();

        void ActivateScene(std::string sceneName);

        void SetCurrentScene(std::string sceneName);
        void SetCurrentScene(int index);
        void SetCurrentScene(Scene* scene);

        void PauseScene(std::string sceneName);
        void UnpauseScene(std::string sceneName); // #TODO Could probably use a SetScenePaused(bool isPaused) method

        Scene* CreateSceneFromFile(const std::string& sceneFilePath, bool addToProjectsSceneFiles = true);
        Scene* CreateScene(const char* const sceneFileNamePrefix, bool addToProjectsSceneFiles = true);

        void DestroyScene(const Scene* const scene);

        void Update(float deltatime);
        void DrawCurrentScene();
        void DrawScene(std::string sceneName);

        Scene* GetCurrentScene();
        int GetCurrentSceneIndex();
        int SceneCount();
        Scene* GetScene(std::string sceneName);
        Scene* GetScene(const Scene* const scene);
        const std::vector<Scene*>& LookAtScenes();

        void SetCurrentSceneDirty();
    }

}
