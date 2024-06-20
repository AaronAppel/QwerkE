#pragma once

#include <string>
#include <vector>

#include "QC_Guid.h"

#include "QF_Enums.h"

namespace QwerkE {

    class Scene;

    namespace Scenes {

        void Initialize();
        void Shutdown();

        void SetCurrentScene(Scene* scene);
        void SetCurrentScene(const GUID& sceneGuid);

        void SetScenePaused(const GUID& sceneGuid, const bool isPaused);

        // #TODO Replace scene references with GUIDS instead of strings
        Scene* CreateSceneFromFile(const std::string& sceneFilePath);
        Scene* CreateScene(const char* const sceneFileNamePrefix);
        Scene* CreateScene(const GUID& sceneGuid);

        void DestroyScene(const Scene* const scene);

        void Update(float deltatime);
        void DrawCurrentScene(u16 viewId = 0);
        void DrawScene(std::string sceneName);

        Scene* GetCurrentScene();
        int GetCurrentSceneIndex();
        int SceneCount();
        Scene* GetScene(const GUID& guid);
        Scene* GetScene(std::string sceneName);
        Scene* GetScene(const Scene* const scene);
        const std::vector<Scene*>& LookAtScenes();

        void SetCurrentSceneDirty();
    }

}
