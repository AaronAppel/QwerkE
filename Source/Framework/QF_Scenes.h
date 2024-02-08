#pragma once

#include <map>
#include <string>

#include "QF_Enums.h"

namespace QwerkE {

    class Scene;

    class Scenes final
    {
    public:

        static void Initialize();
        static void Shutdown();

        static void EnableScene(std::string sceneName);
        static void SetCurrentScene(std::string sceneName);
        static void DisableScene(std::string sceneName);

        static void AddScene(Scene* scene, bool setAsCurrentScene = false);
        static Scene* RemoveScene(Scene* scene);

        static void Update(float deltatime);
        static void DrawCurrentScene();
        static void DrawScene(std::string sceneName);

        static Scene* GetCurrentScene() { return m_CurrentScene; };
        static int SceneCount() { return (int)m_Scenes.size(); };
        static Scene* GetScene(std::string sceneName);
        static const std::vector<Scene*>& LookAtScenes() { return m_Scenes; };

    private:
        Scenes() = default;

        static bool m_IsRunning;
        static Scene* m_CurrentScene; // #FEATURE Handle more than 1 active scene at a time?
        static std::vector<Scene*> m_Scenes;
    };

}
