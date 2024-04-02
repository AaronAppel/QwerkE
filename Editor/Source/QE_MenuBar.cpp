#include "QE_Editor.h"

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QC_imgui.h"
#endif

#include "QC_Time.h"

#include "QF_Directory.h"
#include "QF_Files.h"
#include "QF_Framework.h"
#include "QF_Input.h"
#include "QF_Log.h"
#include "QF_Scenes.h"
#include "QF_Serialization.h"
#include "QF_Settings.h"
#include "QF_Window.h"

#include "QE_EditorInspector.h"
#include "QE_EntityEditor.h"
#include "QE_ProgramArgs.h"
#include "QE_SceneGraph.h"
#include "QE_SceneViewer.h"

namespace QwerkE {

    namespace Editor {

        extern EngineSettings* s_EngineSettings;
        extern bool s_ShowingImGuiExampleWindow;

        void local_DrawMainMenuBar()
        {
#ifdef _QDEARIMGUI
            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("Menu"))
                {
                    static int index = 0;
                    static const int size = 5;
                    const char* d[size] = { "ExampleWindow", "two", "three", "four", "five" };
                    if (ImGui::ListBox("MainMenuBar", &index, d, size, 3))
                    {
                        if (index == 0) s_ShowingImGuiExampleWindow = !s_ShowingImGuiExampleWindow;
                    }
                    ImGui::EndMenu();
                }

                // if (ImGui::BeginMenu("Tools"))
                // {
                //     const int size = 1;
                //     static const char* toolsList[size] = { "Shader Editor" };
                //     static bool* toolsStates[size] = { &s_EngineSettings->showingShaderEditor };
                //
                //     for (int i = 0; i < size; i++)
                //     {
                //         if (ImGui::Checkbox(toolsList[i], toolsStates[i]))
                //         {
                //             s_EngineSettings->showingShaderEditor = *toolsStates[i];
                //         }
                //     }
                //     ImGui::EndMenu();
                // }

                if (ImGui::BeginMenu("Windows"))
                {
                    if (ImGui::Button("Settings Inspector"))
                    {
                        s_EngineSettings->showingSettingsEditor = !s_EngineSettings->showingSettingsEditor;
                        Settings::SaveEngineSettings();
                    }

                    if (ImGui::Button("Schematics Inspector"))
                    {
                        s_EngineSettings->showingSchematicsEditor = !s_EngineSettings->showingSchematicsEditor;
                        Settings::SaveEngineSettings();
                    }

                    if (ImGui::Button("Style Picker"))
                    {
                        s_EngineSettings->showingStylePicker = !s_EngineSettings->showingStylePicker;
                        Settings::SaveEngineSettings();
                    }

                    if (ImGui::Button("FPS"))
                    {
                        s_EngineSettings->showingFPS = !s_EngineSettings->showingFPS;
                        Settings::SaveEngineSettings();
                    }

                    ImGui::EndMenu();
                }

                char buffer[] = { '0', '0', '0', '\0' };
                ImGui::SameLine(ImGui::GetWindowWidth() - sizeof(buffer) * 9); // n characters * 9 pixels each (width)
                if (s_EngineSettings->showingFPS)
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
                        s_EngineSettings->showingFPS = !s_EngineSettings->showingFPS;
                        Settings::SaveEngineSettings();
                    }
                }

                // ImGui::SameLine();
                // if (s_EngineSettings->showingFPS)
                // {
                //     // #TODO Review deltatime calculation and text positioning to avoid "flickering" and frequent resizing
                //     ImGui::PushItemWidth(200.f);
                //     const float deltaTime = Time::FrameDelta();
                //     if (deltaTime == 0.f)
                //     {
                //         ImGui::Text("%1.1f", flags.f);
                //     }
                //     else
                //     {
                //         ImGui::Text("%1.1f", 1.flagsf / deltaTime);
                //     }
                //     ImGui::PopItemWidth();
                // }

                ImGui::EndMainMenuBar();
            }
#endif
        }

    }

}
