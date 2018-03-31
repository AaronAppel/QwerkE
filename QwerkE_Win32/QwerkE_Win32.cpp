#include "../QwerkE.h" // include engine
#include "Systems/Graphics/OpenGLHelpers.h"
#include "Engine_Enums.h"
#include "Utilities/PrintFunctions.h"
#include "Engine.h"
#include "Application.h"
#include "Libraries_Initialize.h"

// TODO: No Globals!
extern MyGLFW* g_MainWindow = nullptr;
extern GameCore* g_GameCore = nullptr;
extern const int g_NumPlayers; // Defined in InputManager.h
extern InputManager* g_InputManager = nullptr;
extern Controller* g_Player1Controller = nullptr;
extern XinputHandler* g_XinputHandler = nullptr;
extern bool g_Debugging = false;

int main() // End user implementation
{
	Engine engine;
	if (engine.Startup() == eEngineMessage::_QFail)
	{
		ConsolePrint("Main(): Error loading services!");
		return 0;
	}

	engine.Run();

	engine.TearDown();
}