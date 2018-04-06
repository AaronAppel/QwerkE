// platform
#include <sdkddkver.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Game1.h"
#include "../QwerkE.h"

int main()
{
	// load

	Engine engine;
	engine.Startup();

	engine.Run();

	engine.TearDown();

	return 0;
}