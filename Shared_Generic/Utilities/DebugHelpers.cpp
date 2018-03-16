#include "DebugHelpers.h"
#include "../Shared_Engine/Systems/GameCore.h"
#include "Libraries/glfw/GLFW/glfw3.h"
#include "Libraries/MyGLFW/MyGLFW.h"

// DebugHelpers.cpp
// *Using direct GLFW calls causes issues.
// This file exists only for aid during development
// and should be removed from any releases and final builds.*
// Input
extern Controller* g_Player1Controller;
Controller* debug_GetPlayerController()
{
	return g_Player1Controller;
}

extern MyGLFW* g_MainWindow;
bool debug_IsKeyDown(int GLFWkey)
{
	// check if keyIsDown
	int state = glfwGetKey(g_MainWindow->GetWindow(), GLFWkey);
	if (state == GLFW_PRESS)
	{
		return true;
	}
	return false;
}
//
extern int g_WindowWidth, g_WindowHeight;
void debug_GetCurrentWindowSize(int* x, int* y)
{
	g_MainWindow->GetWindowSize(x, y);
	// *x = g_WindowWidth;
	// *y = g_WindowHeight;
}

extern GameCore* g_GameCore;
GameCore* debug_GetGameCore()
{
	return g_GameCore;
}

extern double g_TimeSinceExe;
double debug_GetDeltaTime()
{
	return g_TimeSinceExe;
}