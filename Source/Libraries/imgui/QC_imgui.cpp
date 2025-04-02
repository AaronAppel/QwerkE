#include "QC_imgui.h"

namespace ImGui
{
	constexpr float g_pixelsPerCharacter = 6.f; // #TODO Consider a Constants namespace or organization

	ImGuiKey KeyCodeToImGuiKey(int keyCode)
	{
		if (keyCode == 65)
		{
			return ImGuiKey_A;
		}
		return ImGuiKey_Z;
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
