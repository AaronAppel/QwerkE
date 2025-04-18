#include "QwerkE_imgui.h"

#include "imgui_internal.h"

namespace ImGui
{
	constexpr float g_pixelsPerCharacter = 6.f; // #TODO Consider a Constants namespace or organization

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
