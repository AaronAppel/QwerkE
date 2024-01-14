#pragma once

// A file to extend imgui behaviour without modifying source code.
// Can also be the main #include throughout the engine to control #include flow.

#pragma warning( disable : 4244 )
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#pragma warning( default : 4244 )

namespace ImGui
{
	enum Buttons
	{
		MouseLeft = 0,
		MouseRight
	};
}