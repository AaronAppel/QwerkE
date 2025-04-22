#include "QwerkE_imgui.h"

#include <cmath> // For LoadingIndicatorCircle

#include "imgui_internal.h"

#include "../imgui-spin-value/imgui_spin_value.h" // For SpinInt(), SpinFloat(), SpinDouble(), and SpinScalar()

namespace ImGui {

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
		const float offset = (ImGui::GetWindowWidth() / 2.f) - contentWidth; // #TODO Try GetContentRegionAvail().x
		ImGui::SameLine(offset);
	}

	void SameLineCentered(const char* text, float horizontalScaling)
	{
		SameLineCentered(strlen(text), horizontalScaling);
	}

}
