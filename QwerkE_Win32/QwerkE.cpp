// include platform libraries
#include "QwerkE.h"

// include engine framework
#include "QwerkE_Include.h"
#include "Systems/GameCore.h"
#include "Input/Controller.h"
#include "Libraries/MyGLFW/MyGLFW.h"

int g_WindowWidth = 1280, g_WindowHeight = 720;
double g_TimeSinceExe = 0.0;
GameCore* g_GameCore = new GameCore();
Controller* g_Player1Controller = new Controller();
MyGLFW* g_MainWindow = new MyGLFW("Title", vec2(g_WindowWidth, g_WindowHeight), vec2(400, 400));

int main()
{
	while (g_MainWindow->CloseRequested() == false)
	{
		// input
		// logic
		ImGui::Button("Close");
		// render
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui::Render();

		g_MainWindow->SwapBuffers();
	}
	return 0;
}