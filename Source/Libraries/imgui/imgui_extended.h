#pragma once

// #TODO Review where this file should live

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"

typedef int ImGuiKnobFlags;
typedef int ImGuiKnobVariant;

namespace ImGui {

	int PiePopupSelectMenu(const ImVec2& center, const char* popup_id, const char** items, int items_count, int* p_selected);
	void ToggleButton(const char* str_id, bool* v);

	bool ScrollerInt(const char* label, int* v, int step = 1, int step_fast = 100, ImGuiInputTextFlags flags = 0);
	bool ScrollerFloat(const char* label, float* v, float step = 0.1f, float step_fast = 1.f, const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
	bool ScrollerDouble(const char* label, double* v, double step = 0.1, double step_fast = 1.0, const char* format = "%.6f", ImGuiInputTextFlags flags = 0);

	bool BufferingBar(const char* label, float value, const ImVec2& size_arg, const ImU32& bg_col, const ImU32& fg_col);
	bool Spinner(const char* label, float radius, int thickness, const ImU32& color);

	void LoadingIndicatorCircle(const char* label, const float indicator_radius, const ImVec4& main_color, const ImVec4& backdrop_color, const int circle_count, const float speed);

    bool Knob(const char* label, float* p_value, float v_min, float v_max, float speed = 0,
        const char* format = "%.3f", ImGuiKnobVariant variant = 1, float size = 0,
        ImGuiKnobFlags flags = 0, int steps = 10, float angle_min = -1, float angle_max = -1);
    bool KnobInt(const char* label, int* p_value, int v_min, int v_max, float speed = 0,
        const char* format = "%i", ImGuiKnobVariant variant = 1, float size = 0,
        ImGuiKnobFlags flags = 0, int steps = 10, float angle_min = -1, float angle_max = -1);

}
