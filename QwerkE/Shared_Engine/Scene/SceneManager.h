#ifndef __SceneManager_H__
#define __SceneManager_H__

class GameCore;
class Scene;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void Initialize(GameCore* gamecore);

	void ResetScene(eSceneTypes type);
	void ResetAll();

	void EnableScene(eSceneTypes type);
	void DisableScene(eSceneTypes type);
	void DisableAll();

	//void QueueEvent(Event* pEvent);

	void Update(double deltatime);
	void Draw();
	void DrawScene(eSceneTypes scene);

	// TODO:: OnScreenSize() call all scenes and update view matrices

	/* Getters + Setters */
	// getters
	//PlayerController* GetController() { return m_pController; };
	Scene* GetScene(eSceneTypes a_SceneType) { return m_Scenes[a_SceneType]; };
	Scene* GetCurrentScene() { return m_CurrentScene; };

private:
	// functions
	//void ProcessEvents();
	//void OnEvent(Event* pEvent);

	// variables
	bool m_IsRunning = true; // Add step-through and pause/play button functionality in debug mode
	GameCore* m_pGameCore = nullptr;
	Scene* m_CurrentScene = nullptr;
	//std::queue<Event*> m_EventQueue;
	std::map<eSceneTypes, Scene*> m_Scenes;
	//PlayerController* m_pController = nullptr;
};

#endif //__SceneManager_H__
