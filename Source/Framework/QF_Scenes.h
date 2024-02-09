#pragma once

#include <string>

#include "QF_Enums.h"

namespace QwerkE {

    class Scene;

    namespace Scenes
    {
        void Initialize();
        void Shutdown();

        void EnableScene(std::string sceneName);
        void SetCurrentScene(std::string sceneName);
        void DisableScene(std::string sceneName);

        void AddScene(Scene* scene, bool setAsCurrentScene = false);
        Scene* RemoveScene(Scene* scene);

        void Update(float deltatime);
        void DrawCurrentScene();
        void DrawScene(std::string sceneName);

        Scene* GetCurrentScene();
        int SceneCount();
        Scene* GetScene(std::string sceneName);
        const std::vector<Scene*>& LookAtScenes();
    }

}
