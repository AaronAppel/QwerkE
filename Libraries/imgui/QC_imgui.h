#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include "imgui.h"

#include "../enum/QC_enum.h"
QC_ENUM(ImGuiCol_qw, int, // For stringification
	ImGuiCol_Text,
	ImGuiCol_TextDisabled,
	ImGuiCol_WindowBg,              // Background of normal windows
	ImGuiCol_ChildBg,               // Background of child windows
	ImGuiCol_PopupBg,               // Background of popups, menus, tooltips windows
	ImGuiCol_Border,
	ImGuiCol_BorderShadow,
	ImGuiCol_FrameBg,               // Background of checkbox, radio button, plot, slider, text input
	ImGuiCol_FrameBgHovered,
	ImGuiCol_FrameBgActive,
	ImGuiCol_TitleBg,               // Title bar
	ImGuiCol_TitleBgActive,         // Title bar when focused
	ImGuiCol_TitleBgCollapsed,      // Title bar when collapsed
	ImGuiCol_MenuBarBg,
	ImGuiCol_ScrollbarBg,
	ImGuiCol_ScrollbarGrab,
	ImGuiCol_ScrollbarGrabHovered,
	ImGuiCol_ScrollbarGrabActive,
	ImGuiCol_CheckMark,             // Checkbox tick and RadioButton circle
	ImGuiCol_SliderGrab,
	ImGuiCol_SliderGrabActive,
	ImGuiCol_Button,
	ImGuiCol_ButtonHovered,
	ImGuiCol_ButtonActive,
	ImGuiCol_Header,                // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
	ImGuiCol_HeaderHovered,
	ImGuiCol_HeaderActive,
	ImGuiCol_Separator,
	ImGuiCol_SeparatorHovered,
	ImGuiCol_SeparatorActive,
	ImGuiCol_ResizeGrip,            // Resize grip in lower-right and lower-left corners of windows.
	ImGuiCol_ResizeGripHovered,
	ImGuiCol_ResizeGripActive,
	ImGuiCol_Tab,                   // TabItem in a TabBar
	ImGuiCol_TabHovered,
	ImGuiCol_TabActive,
	ImGuiCol_TabUnfocused,
	ImGuiCol_TabUnfocusedActive,
	ImGuiCol_DockingPreview,        // Preview overlay color when about to docking something
	ImGuiCol_DockingEmptyBg,        // Background color for empty node (e.g. CentralNode with no window docked into it)
	ImGuiCol_PlotLines,
	ImGuiCol_PlotLinesHovered,
	ImGuiCol_PlotHistogram,
	ImGuiCol_PlotHistogramHovered,
	ImGuiCol_TableHeaderBg,         // Table header background
	ImGuiCol_TableBorderStrong,     // Table outer and header borders (prefer using Alpha=1.0 here)
	ImGuiCol_TableBorderLight,      // Table inner borders (prefer using Alpha=1.0 here)
	ImGuiCol_TableRowBg,            // Table row background (even rows)
	ImGuiCol_TableRowBgAlt,         // Table row background (odd rows)
	ImGuiCol_TextSelectedBg,
	ImGuiCol_DragDropTarget,        // Rectangle highlighting a drop target
	ImGuiCol_NavHighlight,          // Gamepad/keyboard: current highlighted item
	ImGuiCol_NavWindowingHighlight, // Highlight window when using CTRL+TAB
	ImGuiCol_NavWindowingDimBg,     // Darken/colorize entire screen behind the CTRL+TAB window list, when active
	ImGuiCol_ModalWindowDimBg,      // Darken/colorize entire screen behind a modal window, when one is active
	ImGuiCol_COUNT
);

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

	constexpr ImGuiID InvalidId() { return 0; }

	ImGuiKey KeyCodeToImGuiKey(int keyCode);

	void PushFontQw(Fonts font);
	void PopFontQw();

	void SameLineEnd(int16_t numberOfChars= 0 , float horizontalScaling = 1.f);
	void SameLineEnd(const char* text, float horizontalScaling = 1.f);

	void SameLineCentered(int16_t numberOfChars, float horizontalScaling = 1.f);
	void SameLineCentered(const char* text, float horizontalScaling = 1.f);

#ifdef _QEDITOR // #TODO This shouldn't be in the QC domain
	template <typename Function>
	void DefaultDebugWindow(Function function)
	{
		if (ImGui::Begin("Default Debug Window"))
		{
			ImGui::Separator();
			function();
		}
		ImGui::End();
	}
#endif

}
