#include "../QwerkE.h" // include engine

#include "../Shared_Engine/Engine_Enums.h"
#include "../Shared_Generic/Utilities/PrintFunctions.h"

int main() // End user implementation
{
	Engine engine;
	if (engine.Startup() == eEngineMessage::_QFail)
	{
		OutputPrint("\nMain(): Error loading services!\n");
		return 0;
	}

	SceneManager* sceneManager = (SceneManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Scene_Manager);
	sceneManager->Initialize(); // Setup test scene
	sceneManager->GetCurrentScene()->SetIsEnabled(true);

	engine.Run();

	engine.TearDown();
}