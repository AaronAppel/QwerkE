// It is required to add the following preprocessor defines to this project properties.
// Win32Bit/Win64Bit, and DEBUG/RELEASE

#include "../QwerkE.h" // include engine

#include "../Shared_Engine/Engine_Enums.h"
#include "../Shared_Generic/Utilities/PrintFunctions.h"

int main() // End user implementation
{
	Engine engine;
	if (engine.Startup() == eEngineMessage::_QFail)
	{
		OutputPrint("\nMain(): Error loading services!\n");
		assert(false);
		return 0;
	}

	SceneManager* sceneManager = (SceneManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Scene_Manager);
	sceneManager->Initialize(); // Setup test scene
	sceneManager->GetCurrentScene()->SetIsEnabled(true);

	engine.Run();

	engine.TearDown();
}