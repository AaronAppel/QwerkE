#include "QwerkE_imgui.h"

#include "imgui_internal.h"

namespace ImGui
{
	constexpr float g_pixelsPerCharacter = 6.f; // #TODO Consider a Constants namespace or organization

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

	void PushFontQw(Fonts font)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::PushFont(io.Fonts->Fonts[(int)font]);
	}

	void PopFontQw()
	{
		ImGui::PopFont();
	}

	void SameLineEnd(uint16_t numberOfChars, float horizontalScaling)
	{
		const float padding = ImGui::GetStyle().FramePadding.x;
		const float lineHeight = ImGui::GetFontSize() + padding * 2.0f;
		const float contentWidth = lineHeight + (numberOfChars * g_pixelsPerCharacter * horizontalScaling);
		const float offset = ImGui::GetContentRegionAvail().x - contentWidth + padding;
		ImGui::SameLine(offset);
	}

	void SameLineEnd(const char* text, float horizontalScaling)
	{
		SameLineEnd(strlen(text), horizontalScaling);
	}

	void SameLineCentered(uint16_t numberOfChars, float horizontalScaling)
	{
		const float padding = ImGui::GetStyle().FramePadding.x;
		const float lineHeight = ImGui::GetFontSize() + padding * 2.0f;
		const float contentWidth = lineHeight + (numberOfChars * g_pixelsPerCharacter * horizontalScaling);
		const float offset = (ImGui::GetWindowWidth() / 2.f) - contentWidth;
		ImGui::SameLine(offset);
	}

	void SameLineCentered(const char* text, float horizontalScaling)
	{
		SameLineCentered(strlen(text), horizontalScaling);
	}

}
