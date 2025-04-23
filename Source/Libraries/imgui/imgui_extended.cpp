#include "imgui_extended.h"

#include <algorithm> // For std::max, for LoadingIndicatorCircle
#include <cmath> // For std::sin, std::cos, for LoadingIndicatorCircle

#include "imgui_internal.h" // For various internal ImGui::

#include "../imgui-knobs/imgui-knobs.h" // For Knob() and KnobInt()
#include "../imgui-spin-value/imgui_spin_value.h" // For SpinInt(), SpinFloat(), SpinDouble(), and SpinScalar()

namespace ImGui {

	// From: https://github.com/ocornut/imgui/issues/434
	// Return >= 0 on mouse release
	// Optional int* p_selected display and update a currently selected item
	int PiePopupSelectMenu(const ImVec2& center, const char* popup_id, const char** items, int items_count, int* p_selected)
	{
		int ret = -1;

		if (ImGui::BeginPopup(popup_id, ImGuiWindowFlags_NoDecoration))
		{
			const ImVec2 drag_delta = ImVec2(ImGui::GetIO().MousePos.x - center.x, ImGui::GetIO().MousePos.y - center.y);
			const float drag_dist2 = drag_delta.x * drag_delta.x + drag_delta.y * drag_delta.y;

			const ImGuiStyle& style = ImGui::GetStyle();
			const float RADIUS_MIN = 30.0f;
			const float RADIUS_MAX = 120.0f;
			const float RADIUS_INTERACT_MIN = 20.0f;    // Handle hit testing slightly below RADIUS_MIN
			const int ITEMS_MIN = 6;                    // If they are less than 6 items, we still make each item fill a 1/6 slice.

			// Draw background
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			draw_list->PushClipRectFullScreen();
			// draw_list->PathArcTo(center, (RADIUS_MIN + RADIUS_MAX) * 0.5f, 0.0f, IM_PI * 2.0f);
			// draw_list->PathStroke(IM_COL32(0, 0, 0, 255), ImDrawFlags_Closed, RADIUS_MAX - RADIUS_MIN);

			const float item_arc_span = 2 * IM_PI / ImMax(ITEMS_MIN, items_count);
			float drag_angle = ImAtan2(drag_delta.y, drag_delta.x);
			if (drag_angle < -0.5f * item_arc_span)
				drag_angle += 2.0f * IM_PI;
			//ImGui::Text("%f", drag_angle);    // [Debug]

			// Draw items
			int item_hovered = -1;
			for (int item_n = 0; item_n < items_count; item_n++)
			{
				const char* item_label = items[item_n];
				const float item_ang_min = item_arc_span * (item_n + 0.02f) - item_arc_span * 0.5f; // FIXME: Could calculate padding angle based on how many pixels they'll take
				const float item_ang_max = item_arc_span * (item_n + 0.98f) - item_arc_span * 0.5f;

				bool hovered = false;
				if (drag_dist2 >= RADIUS_INTERACT_MIN * RADIUS_INTERACT_MIN)
					if (drag_angle >= item_ang_min && drag_angle < item_ang_max)
						hovered = true;

				bool selected = p_selected && (*p_selected == item_n);

				draw_list->PathArcTo(center, RADIUS_MAX - style.ItemInnerSpacing.x, item_ang_min, item_ang_max);
				draw_list->PathArcTo(center, RADIUS_MIN + style.ItemInnerSpacing.x, item_ang_max, item_ang_min);
				draw_list->PathFillConvex(ImGui::GetColorU32(hovered ? ImGuiCol_HeaderHovered : selected ? ImGuiCol_HeaderActive : ImGuiCol_Header));

				ImVec2 text_size = ImGui::CalcTextSize(item_label);
				ImVec2 text_pos = ImVec2(
					center.x + cosf((item_ang_min + item_ang_max) * 0.5f) * (RADIUS_MIN + RADIUS_MAX) * 0.5f - text_size.x * 0.5f,
					center.y + sinf((item_ang_min + item_ang_max) * 0.5f) * (RADIUS_MIN + RADIUS_MAX) * 0.5f - text_size.y * 0.5f);
				draw_list->AddText(text_pos, ImGui::GetColorU32(ImGuiCol_Text), item_label);

				if (hovered)
					item_hovered = item_n;
			}
			draw_list->PopClipRect();

			if (ImGui::IsMouseReleased(0))
			{
				ImGui::CloseCurrentPopup();
				ret = item_hovered;
				if (p_selected)
					*p_selected = item_hovered;
			}
			ImGui::EndPopup();
		}
		return ret;
	}

	// From thread: https://github.com/ocornut/imgui/issues/1537
	void ToggleButton(const char* str_id, bool* v) // Squared
	{
		constexpr bool square = false;
		if (square)
		{
			ImVec4* colors = ImGui::GetStyle().Colors;
			ImVec2 p = ImGui::GetCursorScreenPos();
			ImDrawList* draw_list = ImGui::GetWindowDrawList();

			float height = ImGui::GetFrameHeight();
			float width = height * 1.55f;
			float radius = height * 0.50f;
			float rounding = 0.2f;

			ImGui::InvisibleButton(str_id, ImVec2(width, height));
			if (ImGui::IsItemClicked()) *v = !*v;
			ImGuiContext& gg = *GImGui;
			float ANIM_SPEED = 0.085f;
			if (gg.LastActiveId == gg.CurrentWindow->GetID(str_id))// && g.LastActiveIdTimer < ANIM_SPEED)
				float t_anim = ImSaturate(gg.LastActiveIdTimer / ANIM_SPEED);
			if (ImGui::IsItemHovered())
				draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(*v ? colors[ImGuiCol_ButtonActive] : ImVec4(0.78f, 0.78f, 0.78f, 1.0f)), height * rounding);
			else
				draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(*v ? colors[ImGuiCol_Button] : ImVec4(0.85f, 0.85f, 0.85f, 1.0f)), height * rounding);

			ImVec2 center = ImVec2(radius + (*v ? 1 : 0) * (width - radius * 2.0f), radius);
			draw_list->AddRectFilled(ImVec2((p.x + center.x) - 9.0f, p.y + 1.5f),
				ImVec2((p.x + (width / 2) + center.x) - 9.0f, p.y + height - 1.5f), IM_COL32(255, 255, 255, 255), height * rounding);
		}
		else
		{
			ImVec4* colors = ImGui::GetStyle().Colors;
			ImVec2 p = ImGui::GetCursorScreenPos();
			ImDrawList* draw_list = ImGui::GetWindowDrawList();

			float height = ImGui::GetFrameHeight();
			float width = height * 1.55f;
			float radius = height * 0.50f;

			ImGui::InvisibleButton(str_id, ImVec2(width, height));
			if (ImGui::IsItemClicked()) *v = !*v;
			ImGuiContext& gg = *GImGui;
			float ANIM_SPEED = 0.085f;
			if (gg.LastActiveId == gg.CurrentWindow->GetID(str_id))// && g.LastActiveIdTimer < ANIM_SPEED)
				float t_anim = ImSaturate(gg.LastActiveIdTimer / ANIM_SPEED);
			if (ImGui::IsItemHovered())
				draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(*v ? colors[ImGuiCol_ButtonActive] : ImVec4(0.78f, 0.78f, 0.78f, 1.0f)), height * 0.5f);
			else
				draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(*v ? colors[ImGuiCol_Button] : ImVec4(0.85f, 0.85f, 0.85f, 1.0f)), height * 0.50f);
			draw_list->AddCircleFilled(ImVec2(p.x + radius + (*v ? 1 : 0) * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
		}
	}

	// From thread: https://github.com/ocornut/imgui/issues/1537
	bool ScrollerInt(const char* label, int* v, int step, int step_fast, ImGuiInputTextFlags flags)
	{
		const bool result = ImGui::SpinInt(label, v, step, step_fast, flags);
		if (IsItemHovered())
		{
			*v += GetIO().MouseWheel * step;
		}
		return result;
	}

	// From thread: https://github.com/ocornut/imgui/issues/1537
	bool ScrollerFloat(const char* label, float* v, float step, float step_fast, const char* format, ImGuiInputTextFlags flags)
	{
		const bool result = SpinFloat(label, v, step, step_fast, format, flags);
		if (IsItemHovered())
		{
			*v += GetIO().MouseWheel * step;
		}
		return result;
	}

	// From thread: https://github.com/ocornut/imgui/issues/1537
	bool ScrollerDouble(const char* label, double* v, double step, double step_fast, const char* format, ImGuiInputTextFlags flags)
	{
		const bool result = SpinDouble(label, v, step, step_fast, format, flags);
		if (IsItemHovered())
		{
			*v += GetIO().MouseWheel * step;
		}
		return result;
	}

	// From: https://github.com/ocornut/imgui/issues/1901
	bool BufferingBar(const char* label, float value, const ImVec2& size_arg, const ImU32& bg_col, const ImU32& fg_col)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);

		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size = size_arg;
		size.x -= style.FramePadding.x * 2;

		const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
		ItemSize(bb, style.FramePadding.y);
		if (!ItemAdd(bb, id))
			return false;

		// Render
		const float circleStart = size.x * 0.7f;
		const float circleEnd = size.x;
		const float circleWidth = circleEnd - circleStart;

		window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart, bb.Max.y), bg_col);
		window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart * value, bb.Max.y), fg_col);

		const float t = g.Time;
		const float r = size.y / 2;
		const float speed = 1.5f;

		const float a = speed * 0;
		const float b = speed * 0.333f;
		const float c = speed * 0.666f;

		const float o1 = (circleWidth + r) * (t + a - speed * (int)((t + a) / speed)) / speed;
		const float o2 = (circleWidth + r) * (t + b - speed * (int)((t + b) / speed)) / speed;
		const float o3 = (circleWidth + r) * (t + c - speed * (int)((t + c) / speed)) / speed;

		window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o1, bb.Min.y + r), r, bg_col);
		window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o2, bb.Min.y + r), r, bg_col);
		window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o3, bb.Min.y + r), r, bg_col);
	}

	// From: https://github.com/ocornut/imgui/issues/1901
	bool Spinner(const char* label, float radius, int thickness, const ImU32& color)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);

		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size((radius) * 2, (radius + style.FramePadding.y) * 2);

		const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
		ItemSize(bb, style.FramePadding.y);
		if (!ItemAdd(bb, id))
			return false;

		// Render
		window->DrawList->PathClear();

		int num_segments = 30;
		int start = abs(ImSin(g.Time * 1.8f) * (num_segments - 5));

		const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
		const float a_max = IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;

		const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

		for (int i = 0; i < num_segments; i++) {
			const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
			window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a + g.Time * 8) * radius,
				centre.y + ImSin(a + g.Time * 8) * radius));
		}

		window->DrawList->PathStroke(color, false, thickness);
	}

	// From: https://github.com/ocornut/imgui/issues/1901
	void LoadingIndicatorCircle(const char* label, const float indicator_radius,
		const ImVec4& main_color, const ImVec4& backdrop_color,
		const int circle_count, const float speed)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems) {
			return;
		}

		ImGuiContext& g = *GImGui;
		const ImGuiID id = window->GetID(label);

		const ImVec2 pos = window->DC.CursorPos;
		const float circle_radius = indicator_radius / 15.0f;
		const float updated_indicator_radius = indicator_radius - 4.0f * circle_radius;
		const ImRect bb(pos, ImVec2(pos.x + indicator_radius * 2.0f, pos.y + indicator_radius * 2.0f));
		ItemSize(bb);
		if (!ItemAdd(bb, id)) {
			return;
		}
		const float t = g.Time;
		const auto degree_offset = 2.0f * IM_PI / circle_count;
		for (int i = 0; i < circle_count; ++i) {
			const auto x = updated_indicator_radius * std::sin(degree_offset * i);
			const auto y = updated_indicator_radius * std::cos(degree_offset * i);
			const auto growth = std::max(0.0f, std::sin(t * speed - i * degree_offset));
			ImVec4 color;
			color.x = main_color.x * growth + backdrop_color.x * (1.0f - growth);
			color.y = main_color.y * growth + backdrop_color.y * (1.0f - growth);
			color.z = main_color.z * growth + backdrop_color.z * (1.0f - growth);
			color.w = 1.0f;
			window->DrawList->AddCircleFilled(ImVec2(pos.x + indicator_radius + x,
				pos.y + indicator_radius - y),
				circle_radius + growth * circle_radius, GetColorU32(color));
		}
	}

	bool Knob(const char* label, float* p_value, float v_min, float v_max, float speed,
		const char* format, ImGuiKnobVariant variant, float size,
		ImGuiKnobFlags flags, int steps, float angle_min, float angle_max)
	{
		return ImGuiKnobs::Knob(label, p_value, v_min, v_max, speed,
			format, variant, size,
			flags, steps, angle_min, angle_max);
	}

	bool KnobInt(const char* label, int* p_value, int v_min, int v_max, float speed,
		const char* format, ImGuiKnobVariant variant, float size,
		ImGuiKnobFlags flags, int steps, float angle_min, float angle_max)
	{
		return ImGuiKnobs::KnobInt(label, p_value, v_min, v_max, speed,
			format, variant, size,
			flags, steps, angle_min, angle_max);
	}

}