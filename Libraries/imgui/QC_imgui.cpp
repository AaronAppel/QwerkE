#include "QC_imgui.h"

namespace ImGui
{

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

	void SameLineEnd(float scaling)
	{
		const float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
		const float offset = ImGui::GetContentRegionAvail().x - lineHeight * scaling;
		ImGui::SameLine(offset);
	}

	void SameLineEnd(const char* text)
	{
		const float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
		const float offset = ImGui::GetContentRegionAvail().x - lineHeight * strlen(text);
		ImGui::SameLine(offset);
	}

}
