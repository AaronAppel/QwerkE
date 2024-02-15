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

        void AddScene(Scene* scene, bool setAsCurrentScene = false); // #TODO Consider restricting scene creation to only Scenes
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
