#include "../QwerkE.h" // include engine
#include "Engine_Enums.h"
#include "Utilities/PrintFunctions.h"

int main() // End user implementation
{
	Engine engine;
	if (engine.Startup() == eEngineMessage::_QFail)
	{
		OutputPrint("Main(): Error loading services!");
		return 0;
	}

	engine.Run();

	engine.TearDown();
}