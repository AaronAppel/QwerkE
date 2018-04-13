// TODO: Fix architecture error
#include <sdkddkver.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "../Shared_Engine/Engine_Platform.h"

#include "../Shared_Engine/Engine_Enums.h"

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
	sceneManager->GetCurrentScene()->SetIsEnabled(true);

	engine.Run();

	engine.TearDown();

	return 0;
}