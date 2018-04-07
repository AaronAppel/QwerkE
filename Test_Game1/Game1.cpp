// platform
#include <sdkddkver.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Game1.h"
#include "../QwerkE.h"
#include "GameScene.h"

int main()
{
	// load

	Engine engine;
	engine.Startup();

    SceneManager* sceneManager = (SceneManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Scene_Manager);
    sceneManager->AddScene(new GameScene());
    sceneManager->SetCurrentScene(eSceneTypes::Scene_GameScene);

	engine.Run();

	engine.TearDown();

	return 0;
}