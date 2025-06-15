#pragma once

#include <string>

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowWelcomeWindow : public EditorWindow
		{
		public:
			EditorWindowWelcomeWindow(GUID guid) :
				EditorWindow("Welcome Window",
					EditorWindowTypes::WelcomeWindow,
					guid,
					(EditorWindowFlags)(EditorWindowFlags::HideWindowOptions | EditorWindowFlags::InitWithWindowSettings))
			{
				vec2f windowSize = Window::GetSize();
				m_WindowOptions.m_InitialWidth = windowSize.x / 4.f;
				m_WindowOptions.m_InitialHeight = windowSize.y / 1.5f;

				m_WindowOptions.m_InitialPositionX = windowSize.x / 2.f - m_WindowOptions.m_InitialWidth / 2.f;
				m_WindowOptions.m_InitialPositionY = windowSize.y / 2.f - m_WindowOptions.m_InitialHeight / 2.f;
			}

		private:
			void DrawInternal() override
			{
				constexpr float fontScale = 1.4f; // Scale better. Could assume a default size and scale based off of difference from window size
				ImGui::SetWindowFontScale(fontScale);

				std::string cacheString = "";

				const float contentHeight = ImGui::GetContentRegionAvail().y;
				const float halfContentWidth = ImGui::GetContentRegionAvail().x / 2.f;
				const float halfPixelsPerChar = 0.49f * ImGui::g_pixelsPerCharacter * fontScale;

				ImGui::Dummy({ 0, contentHeight * 0.1f });
				ImGui::Dummy({ 0, 0.f });

				cacheString = "Welcome to QwerkE!";
				ImGui::SameLine(halfContentWidth - cacheString.size() * halfPixelsPerChar);
				ImGui::Text(cacheString.c_str());

				ImGui::Dummy({ 0, contentHeight * 0.05f });
				ImGui::Dummy({0,0});
				cacheString = "We hope you enjoy your stay.";
				ImGui::SameLine(halfContentWidth - cacheString.size() * halfPixelsPerChar);
				ImGui::Text(cacheString.c_str());

				ImGui::Dummy({ 0, contentHeight * 0.05f });
				ImGui::Dummy({ 0,0 });
				cacheString = "Please see QwerkE/Documentation for more info.";
				ImGui::SameLine(halfContentWidth - cacheString.size() * halfPixelsPerChar);
				ImGui::Text(cacheString.c_str());

				// #TODO Clickable links for opening local repo documentation files, as well as online resources such as git

				ImGui::Dummy({ 0, contentHeight * 0.20f });
				ImGui::Dummy({ 0, 0.f });
				cacheString = "Okay!";
				ImGui::SameLine(halfContentWidth - cacheString.size() * halfPixelsPerChar);
				ImGui::PushItemWidth(halfContentWidth * 0.5f);
				if (ImGui::Button(cacheString.c_str()))
				{
					ImGui::PopItemWidth();
					Editor::CloseEditorWindow(this->Guid());
					return;
				}
				ImGui::PopItemWidth();

				ImGui::Dummy({ 0, contentHeight * 0.25f });
				ImGui::Dummy({ 0, 0.f });
				cacheString = "  Show welcome window on start up";
				ImGui::SameLine(halfContentWidth - cacheString.size() * halfPixelsPerChar);
				if (ImGui::Checkbox(cacheString.c_str(), &m_UserSettings.showWelcomeWindow))
				{
					Settings::SaveUserSettings();
				}
			}

			UserSettings& m_UserSettings = Settings::GetUserSettings();
		};

	}

}
