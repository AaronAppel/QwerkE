#ifndef _Libraries_Initialize_H_
#define _Libraries_Initialize_H_

//////////////////////////////
// Setup any libraries that have program wide states
// that need to be initialized before used
//////////////////////////////

// TODO: Use _QFail and other error messages?
bool Libs_Setup()
{
	bool errorFree = true; // if error occurs set to false

	// Setup/Load libraries based on platform, architecture, configuration
	// TODO: Clean up #ifs
	#ifdef _Q32bit // architecture
	// platform
	#ifdef _QWindows
	// TODO: setup libraries

	#elif _Android
	#elif _Mac
	#elif _Linux
	#endif

	#elif _Q64bit // architecture
	// platform
	#ifdef _QWindows
	#elif _Android
	#elif _Mac
	#elif _Linux
	#endif

	#else
	#pragma warning "Define Platform!"
	#endif // !platform

	//////////////////////////////
	// FILES //
	//////////////////////////////
	// font loading/rendering
	// freetype2
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		ConsolePrint("\nError loading freetype2!\n");
		errorFree = false;
	}
	else
		FT_Done_FreeType(ft);
	//////////////////////////////

	//////////////////////////////
	// Graphics //
	//////////////////////////////
	// window creation //
	// GLFW
	if (!glfwInit())
	{
		ConsolePrint("\nError loading GLFW 1!\n");
		errorFree = false;
	}
	else
	{
		GLFWwindow* window;
		window = glfwCreateWindow(100, 100, "Test", NULL, NULL);
		if (!window)
		{
			ConsolePrint("\nError loading GLFW 2!\n");
			errorFree = false;
		}
		else
		{
			// openGL extensions wrangler //
			// GLEW
			glfwMakeContextCurrent(window);
			if (glewInit() != GLEW_OK)
			{
				ConsolePrint("\nError loading GLEW!\n");
				errorFree = false;
			}
			glfwDestroyWindow(window);
		}
	}
	//////////////////////////////

	//////////////////////////////
	// GUIs //
	//////////////////////////////
	// immediate mode //
	// imgui
	ImGuiContext* context= nullptr;
	context = ImGui::CreateContext();
	if (context == nullptr)
	{
		ConsolePrint("\nError loading imgui!\n");
		errorFree = false;
	}
	else
	{
		ImGuiIO& io = ImGui::GetIO(); (void)io;
	}
	//////////////////////////////

	//////////////////////////////
	// threading //
	//////////////////////////////
	// pthreads
	// Load anything?
	//////////////////////////////

	//////////////////////////////
	// networking
	//////////////////////////////
	// RakNet
	// Load anything?
	//////////////////////////////

	return errorFree;
}

void Libs_TearDown()
{
	ImGui_ImplGlfwGL3_Shutdown(); // shutdown imgui
	ImGui::DestroyContext(); // destroy imgui
	glfwTerminate(); // shutdown glfw
}

#endif // !_Libraries_Initialize_H_
