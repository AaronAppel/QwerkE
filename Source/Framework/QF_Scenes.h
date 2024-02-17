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
        void PauseScene(std::string sceneName);
        void UnpauseScene(std::string sceneName);

        Scene* CreateScene(const std::string& sceneName, bool addToProjectsSceneFiles = true);

        Scene* RemoveScene(Scene* scene);

        void Update(float deltatime);
        void DrawCurrentScene();
        void DrawScene(std::string sceneName);

        Scene* GetCurrentScene();
        int GetCurrentSceneIndex();
        int SceneCount();
        Scene* GetScene(std::string sceneName);
        const std::vector<Scene*>& LookAtScenes();
    }

}
