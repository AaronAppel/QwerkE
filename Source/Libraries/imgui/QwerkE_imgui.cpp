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
		const float offset = (ImGui::GetWindowWidth() / 2.f) - (contentWidth / 2.f);
		ImGui::SameLine(offset);
	}

	void SameLineCentered(const char* text, float horizontalScaling)
	{
		SameLineCentered(strlen(text), horizontalScaling);
	}

	void SameLineIfSpace(float itemSize)
	{
		ImGui::SameLine();
		if (ImGui::GetContentRegionAvail().x < itemSize)
		{
			ImGui::Dummy({});
		}
	}

	bool IsAtStartOfNewLine() // #TODO Review
	{
		return ImGui::GetCursorPosX() == 8.0f;
	}

	std::string DirectoryPathAsSameLineButtons(const std::string& directory)
	{
		uint16_t index = 0;
		std::string returnString = "";
		for (size_t i = 0; i < directory.size(); i++)
		{
			if ('\\' == directory[i])
			{
				++i;
				std::string temp = directory.substr(index, i - index);
				index = i;
				if (ImGui::Button(temp.c_str()))
				{
					returnString = directory.substr(0, index);
				}
				if (i < directory.size() - 1)
				{
					ImGui::SameLine(0, 1.f); // #NOTE Reduce space between buttons
				}
			}
			else if (i == directory.size() - 1)
			{
				std::string temp = directory.substr(index, i - index + 1);
				ImGui::Button(temp.c_str());
			}
		}
		return returnString;
	}

}
