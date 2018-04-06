#include "../QwerkE.h" // include engine

#include "../Shared_Engine/Engine_Enums.h"
#include "../Shared_Generic/Utilities/PrintFunctions.h"

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