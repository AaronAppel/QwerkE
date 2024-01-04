#pragma once

#include <map>

#include "../../Headers/QwerkE_Enums.h"

namespace QwerkE {

    class Scene;

    class Scenes
    {
    public:
        Scenes();
        ~Scenes();

        static void Initialize();

        static void EnableScene(eSceneTypes type);
        static void SetCurrentScene(eSceneTypes type);
        static void DisableScene(eSceneTypes type);
        static void DisableAll();

        static void AddScene(Scene* scene);
        static Scene* RemoveScene(Scene* scene);

        static void Update(float deltatime);
        static void DrawCurrentScene();
        static void DrawScene(eSceneTypes scene);

        static Scene* GetScene(eSceneTypes a_SceneType) { return m_Scenes[a_SceneType]; };
        static Scene* GetCurrentScene() { return m_CurrentScene; };
        static int SceneCount() { return (int)m_Scenes.size(); };
        static const std::map<eSceneTypes, Scene*>* LookAtScenes() { return &m_Scenes; };

    private:
        static bool m_IsRunning;
        static Scene* m_CurrentScene;
        static std::map<eSceneTypes, Scene*> m_Scenes;
    };

}
