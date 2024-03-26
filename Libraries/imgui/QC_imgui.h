#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include "imgui.h"

namespace ImGui
{
	enum Buttons
	{
		MouseLeft = 0,
		MouseRight
	};

	enum Fonts
	{
		Regular = 0,

		Bold,
		BoldItalic,
		ExtraBold,
		ExtraBoldItalic,
		SemiBold,
		SemiBoldItalic,

		Italic,

		Light,
		LightItalic,

		Medium,
		MediumItalic,

		Count,
	};

	ImGuiKey KeyCodeToImGuiKey(int keyCode);

	void PushFontQw(Fonts font);
	void PopFontQw();
}
