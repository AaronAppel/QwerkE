#pragma once

#include "QC_Time.h"

#include "QF_Settings.h"

#include "QE_Editor.h"
#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowMenuBar : public EditorWindow
		{
		public:
			EditorWindowMenuBar(GUID guid) : EditorWindow("MenuBar", EditorWindowTypes::MenuBar, guid, EditorWindowFlags::Headless) { }

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
                            if (index == 0) Editor::ToggleEditorStateFlags(EditorStateFlags::UiMainMenuShowingImGuiExample);
                        }
                        ImGui::EndMenu();
                    }

                    EngineSettings& engineSettings = Settings::GetEngineSettings();
                    if (ImGui::BeginMenu("Windows"))
                    {
                        {
                            // Menu of window types
                            // Open -> context pop out/window/whatever

                            constexpr u32 startingIndex = EditorWindowTypes::EditorWindowTypesInvalid + 1;
                            for (u32 i = startingIndex; i < EditorWindowTypes::_size_constant; i++)
                            {
                                EditorWindowTypes type = EditorWindowTypes::_from_index(i);
                                if (ImGui::Button(ENUM_TO_STR(type)))
                                {
                                    Editor::OpenEditorWindow((u32)type);
                                    // #TODO Set editor UI state to dirty
                                }
                            }
                            ImGui::Separator();
                        }

                        if (m_ShowingFps && ImGui::Button("FPS"))
                        {
                            m_ShowingFps = !m_ShowingFps;
                        }

                        ImGui::EndMenu();
                    }

                    ImGui::SameLineEnd(3);
                    if (m_ShowingFps)
                    {
                        char buffer[] = { '0', '0', '0', '\0' };
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
                            m_ShowingFps = !m_ShowingFps;
                        }
                    }
                }
                ImGui::EndMainMenuBar();
			}

            bool m_ShowingFps = true;
		};

	}

}