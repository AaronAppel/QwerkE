#pragma once

#include "QF_Settings.h"

#include "QE_Editor.h"
#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowMenuBar : public EditorWindow
		{
		public:
			EditorWindowMenuBar() : EditorWindow("MenuBar", EditorWindowFlags::Hidden) { }

            bool IsUnique() override { return true; }

		private:
			void DrawInternal() override
			{
                if (ImGui::BeginMainMenuBar())
                {
                    if (ImGui::BeginMenu("Menu"))
                    {
                        static int index = 0;
                        static const int size = 5;
                        const char* d[size] = { "ExampleWindow", "two", "three", "four", "five" };
                        if (ImGui::ListBox("MainMenuBar", &index, d, size, 3))
                        {
                            if (index == 0) Editor::SetEditorStateFlags(EditorStateFlags::UiMainMenuShowingImGuiExample);
                        }
                        ImGui::EndMenu();
                    }

                    EngineSettings& engineSettings = Settings::GetEngineSettings();
                    if (ImGui::BeginMenu("Windows"))
                    {
                        if (ImGui::Button("Settings Inspector"))
                        {
                            engineSettings.showingSettingsEditor = !engineSettings.showingSettingsEditor;
                            Settings::SaveEngineSettings();
                        }

                        if (ImGui::Button("Schematics Inspector"))
                        {
                            engineSettings.showingSchematicsEditor = !engineSettings.showingSchematicsEditor;
                            Settings::SaveEngineSettings();
                        }

                        if (ImGui::Button("Style Picker"))
                        {
                            engineSettings.showingStylePicker = !engineSettings.showingStylePicker;
                            Settings::SaveEngineSettings();
                        }

                        if (ImGui::Button("FPS"))
                        {
                            engineSettings.showingFPS = !engineSettings.showingFPS;
                            Settings::SaveEngineSettings();
                        }

                        ImGui::EndMenu();
                    }

                    char buffer[] = { '0', '0', '0', '\0' };
                    ImGui::SameLine(ImGui::GetWindowWidth() - sizeof(buffer) * 9); // n characters * 9 pixels each (width)
                    if (engineSettings.showingFPS)
                    {
                        const float& deltaTime = (float)Time::PreviousFrameDuration();
                        if (deltaTime == .0f)
                        {
                            snprintf(buffer, sizeof(buffer) / sizeof(char), "%1.1f", deltaTime);
                        }
                        else
                        {
                            snprintf(buffer, sizeof(buffer) / sizeof(char), "%3.2f", 1.f / deltaTime);
                        }

                        ImGui::PushItemWidth(200.f);
                        if (ImGui::Button(buffer))
                        {
                        }
                        ImGui::PopItemWidth();

                        if (ImGui::IsItemClicked())
                        {
                            engineSettings.showingFPS = !engineSettings.showingFPS;
                            Settings::SaveEngineSettings();
                        }
                    }
                }
                ImGui::EndMainMenuBar();
			}
		};

	}

}