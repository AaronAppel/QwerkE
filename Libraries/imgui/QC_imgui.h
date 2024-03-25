#pragma once

#ifdef _QDEARIMGUI
#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include "imgui.h"
// imgui handled by bgfx for now. Look to remove this deprecated code
// #include "imgui_impl_glfw.h"
// #include "imgui_impl_opengl3.h"

namespace ImGui
{
	enum Buttons
	{
		MouseLeft = 0,
		MouseRight
	};

	ImGuiKey KeyCodeToImGuiKey(int keyCode);

}
#endif
