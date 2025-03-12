#pragma once

#include <string>
#include <vector>

#include "QC_Guid.h"

#include "QF_Enums.h" // #NOTE Scene transition changes removes line

namespace QwerkE {

    class Scene;

    namespace Scenes {

        void Initialize(); // #NOTE Scene transition changes removes line
        void Shutdown();

        void SetCurrentScene(Scene* scene); // #NOTE Scene transition changes removes line
        void SetCurrentScene(const GUID& sceneGuid); // #NOTE Scene transition changes removes line

        void SetScenePaused(const GUID& sceneGuid, const bool isPaused); // #NOTE Scene transition changes removes line

        // #TODO Replace scene references with GUIDS instead of strings
        Scene* CreateSceneFromFile(const std::string& sceneFilePath);
        Scene* CreateScene(const char* const sceneFileNamePrefix); // #NOTE Scene transition changes removes line
        Scene* CreateScene(const GUID& sceneGuid);
        // #NOTE Scene transition changes Scene* CreateScene(const std::string& sceneFileName);

        void DestroyScene(const Scene* const scene);

        // #NOTE Scene transition changes rename to UpdateScenes()
        void Update(float deltatime); // #NOTE Scene transition changes removes line
        void DrawCurrentScene(u16 viewId = 0); // #NOTE Scene transition changes removes line
        void DrawScene(std::string sceneName); // #NOTE Scene transition changes removes line
        // #NOTE Scene transition changes void UpdateScenes(float deltatime);
        // #NOTE Scene transition changes void DrawScenes(u16 viewId = 0);

        Scene* GetCurrentScene(); // #NOTE Scene transition changes removes line
        int GetCurrentSceneIndex(); // #NOTE Scene transition changes removes line
        int SceneCount();
        Scene* GetScene(const GUID& guid);
        Scene* GetScene(std::string sceneName); // #NOTE Scene transition changes removes line
        Scene* GetScene(const Scene* const scene); // #NOTE Scene transition changes removes line
        // #NOTE Scene transition changes bool HasScene(const Scene* scene);
        const std::vector<Scene*>& LookAtScenes();

        void SetCurrentSceneDirty(); // #NOTE Scene transition changes removes line
    }

}
