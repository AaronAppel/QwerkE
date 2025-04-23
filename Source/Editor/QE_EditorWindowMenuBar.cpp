#include "QE_EditorWindowMenuBar.h"

#include "QC_Time.h"

#include "QF_Directory.h"
#include "QF_Files.h"
#include "QF_Input.h"
#include "QF_PathDefines.h"
#include "QF_Paths.h"
#include "QF_Serialize.h"

#include "QE_Projects.h"
#include "QE_Settings.h"

namespace QwerkE {

	namespace Editor {

        void local_CheckHotkeys();
        void local_DrawFileMenu();
        void local_DrawEditMenu();
        void local_DrawStyleMenu(std::vector<const char*>& styleFiles, int& currentStyleFileIndex);
        void local_DrawWindowsMenu();
        void local_DrawFps(bool& calculateFps, const float textScale);

        void local_DrawWindowsList(const char* name, std::vector<EditorWindowTypes> types);

        static constexpr float s_SubMenuWidth = 125.f;

        static void local_CleanUpStyleFiles(std::vector<const char*>& styleFiles)
        {
            for (size_t i = 0; i < styleFiles.size(); i++)
            {
                delete[] styleFiles[i];
            }
            styleFiles.clear();
        }

        static void local_UpdateStyleFilesList(std::vector<const char*>& styleFiles, int& currentStyleFileIndex)
        {
            local_CleanUpStyleFiles(styleFiles);

            styleFiles = Directory::ListFiles(Paths::StylesDir().c_str(), style_file_ext, true);

            if (const char* currentStyleFileName = Settings::GetStyleFileName())
            {
                for (size_t i = 0; i < styleFiles.size(); i++)
                {
                    if (strcmp(styleFiles[i], currentStyleFileName) == 0)
                    {
                        currentStyleFileIndex = i;
                    }
                }

                if (currentStyleFileName && styleFiles.empty())
                {
                    styleFiles.emplace_back(currentStyleFileName);
                }
            }
        }

        void local_CheckHotkeys()
        {
            if (Input::FrameKeyAction(eKeys::eKeys_N, eKeyState::eKeyState_Press) &&
                (Input::IsKeyDown(eKeys::eKeys_LCTRL) || Input::IsKeyDown(eKeys::eKeys_RCTRL)))
            {
                Editor::NewEditorWindow(EditorWindowTypes::Prompt);
            }

            if (Input::FrameKeyAction(eKeys::eKeys_O, eKeyState::eKeyState_Press) &&
                (Input::IsKeyDown(eKeys::eKeys_LCTRL) || Input::IsKeyDown(eKeys::eKeys_RCTRL)))
            {
                Projects::LoadProjectFromExplorer();
            }

            if (Input::FrameKeyAction(eKeys::eKeys_S, eKeyState::eKeyState_Press) &&
                (Input::IsKeyDown(eKeys::eKeys_LCTRL) || Input::IsKeyDown(eKeys::eKeys_RCTRL)) &&
                (Input::IsKeyDown(eKeys::eKeys_LSHIFT) || Input::IsKeyDown(eKeys::eKeys_RSHIFT)))
            {
                Projects::SaveProjectFromExplorer();
            }
            else if (Input::FrameKeyAction(eKeys::eKeys_S, eKeyState::eKeyState_Press) &&
                (Input::IsKeyDown(eKeys::eKeys_LCTRL) || Input::IsKeyDown(eKeys::eKeys_RCTRL)))
            {
                Projects::SaveProject();
            }
        }

        EditorWindowMenuBar::~EditorWindowMenuBar()
        {
            local_CleanUpStyleFiles(m_StyleFiles);
        }

		void EditorWindowMenuBar::DrawInternal()
		{
            local_CheckHotkeys();

            ImGui::SetNextWindowSize({ 2160 , 50});
            if (ImGui::BeginMainMenuBar())
            {
                ImGui::SetWindowFontScale(m_TextScale);

                local_DrawFileMenu();
                local_DrawEditMenu();
                local_DrawStyleMenu(m_StyleFiles, m_CurrentStyleFileIndex);
                local_DrawWindowsMenu();

                const Project& currentProject = Projects::CurrentProject();
                if (strcmp(currentProject.projectFileName.c_str(), Constants::gc_DefaultStringValue) == 0)
                {
                    const char defaultProjectName[] = { 'N','o',' ','P','r','o','j','e','c','t','\0'};
                    ImGui::SameLineCentered(sizeof(defaultProjectName));
                    ImGui::Text(defaultProjectName);
                }
                else
                {
                    ImGui::SameLineCentered((u16)currentProject.projectName.size());
                    ImGui::Text(currentProject.projectName.c_str());
                }

                ImGui::SameLine(0.f, ImGui::GetContentRegionAvail().x - 100.f);
                if (ImGui::SmallButton("Restart"))
                {
                    Editor::RequestRestart();
                }

                local_DrawFps(m_CalculateFps, m_TextScale);

                ImGui::SetWindowFontScale(1.f);
            }
            ImGui::EndMainMenuBar();
		}

        void local_DrawFileMenu()
        {
            if (!ImGui::BeginMenu("File"))
                return;

            if (ImGui::MenuItem("New Project...", "Ctrl+N"))
            {
                Editor::NewEditorWindow(EditorWindowTypes::Prompt);
            }

            if (ImGui::MenuItem("Open Project...", "Ctrl+O"))
            {
                Projects::LoadProjectFromExplorer();
            }

            if (ImGui::BeginMenu("Open Recent"))
            {
                const ProjectsData& projectsData = Projects::GetProjectsData();
                for (size_t i = 0; i < projectsData.PreviousProjectFileNames.size(); i++)
                {
                    if (ImGui::MenuItem(projectsData.PreviousProjectFileNames[i].c_str()))
                    {
                        Projects::LoadProject(projectsData.PreviousProjectFileNames[i]);
                    }
                }
                ImGui::EndMenu();
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Save Project", "Ctrl+S"))
            {
                Projects::SaveProject();
            }

            if (ImGui::MenuItem("Save Project As...", "Ctrl+Shift+S"))
            {
                Projects::SaveProjectFromExplorer();
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Unload Project", ""))
            {
                Projects::UnloadCurrentProject();
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Exit", "Esc"))
            {
                Window::RequestClose();
            }

            ImGui::EndMenu();
        }

        void local_DrawEditMenu()
        {
            if (!ImGui::BeginMenu("Edit"))
                return;

            if (ImGui::MenuItem("Settings"))
            {
                Editor::NewEditorWindow(EditorWindowTypes::Settings);
            }

            ImGui::EndMenu();
        }

        void local_DrawStyleMenu(std::vector<const char*>& styleFiles, int& currentStyleFileIndex)
        {
            if (!ImGui::BeginMenu("Styles"))
                return;

            // #TODO

            ImGui::PushItemWidth(s_SubMenuWidth);
            if (ImGui::MenuItem("Style Picker"))
            {
                Editor::NewEditorWindow(EditorWindowTypes::StylePicker);
            }

            ImGui::Separator();
            if (styleFiles.empty())
            {
                local_UpdateStyleFilesList(styleFiles, currentStyleFileIndex);
            }

            if (ImGui::ListBox("##StylesListBox", &currentStyleFileIndex, styleFiles.data(), styleFiles.size(), 5))
            {
                ImGuiStyle& style = ImGui::GetStyle();
                Serialize::FromFile(Paths::Style(styleFiles[currentStyleFileIndex]).c_str(), style);
            }
            ImGui::PopItemWidth();

            ImGui::EndMenu();
        }

        void local_DrawWindowsMenu()
        {
            if (!ImGui::BeginMenu("Windows"))
                return;

            if (ImGui::BeginMenu("Open"))
            {
                const auto& windows = Editor::GetOpenWindows();
                for (size_t i = 0; i < windows.size(); i++)
                {
                    if (ImGui::MenuItem(ENUM_TO_STR(windows[i]->Type())))
                    {
                        windows[i]->Focus();
                        break;
                    }
                }
                ImGui::EndMenu();
            }
            ImGui::Separator();

            local_DrawWindowsList("Scene", {
                    EditorWindowTypes::SceneControls,
                    EditorWindowTypes::SceneGraph,
                    EditorWindowTypes::SceneView,
                    EditorWindowTypes::PrefabScene
                }
            );
            local_DrawWindowsList("Editing", {
                    EditorWindowTypes::EntityInspector,
                    EditorWindowTypes::MaterialEditor,
                    EditorWindowTypes::NodeEditor,
                    EditorWindowTypes::ShaderEditor,
                    EditorWindowTypes::HexEditor
                }
            );
            local_DrawWindowsList("Other", {
                    EditorWindowTypes::Assets,
                    EditorWindowTypes::Console,
                    EditorWindowTypes::DefaultDebug,
                    EditorWindowTypes::ImGuiDemo,
                    EditorWindowTypes::ImGuiExtendedDemo,
                    EditorWindowTypes::Settings,
                    EditorWindowTypes::FolderViewer,
                    EditorWindowTypes::Prompt,
                    EditorWindowTypes::StylePicker
                }
            );
            ImGui::Separator();

            if (ImGui::Button("Close All"))
            {
                ImGui::OpenPopup("CloseAllPrompt");
            }
            if (ImGui::BeginPopup("CloseAllPrompt"))
            {
                ImGui::Text("Are you sure?");
                if (ImGui::Button("Yes"))
                {
                    const auto& windows = Editor::GetOpenWindows();
                    for (int i = windows.size() - 1; i >= 0; i--)
                    {
                        Editor::CloseEditorWindow(windows[i]->Guid());
                    }
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::Button("Cancel"))
                {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }


            ImGui::EndMenu();
        }

        void local_DrawFps(bool& calculateFps, const float textScale)
        {
            const float offset = ImGui::GetWindowWidth() - (40.f * textScale);
            ImGui::SameLine(offset);
            char buffer[] = { 'F', 'P', 'S', '\0' };
            if (calculateFps)
            {
                float deltaTime = (float)Time::PreviousFrameDuration();
                const char* format = "%1.1f";
                if (deltaTime != .0f)
                {
                    format = "%3.2f";
                    deltaTime = 1.f / deltaTime;
                }
                snprintf(buffer, sizeof(buffer) / sizeof(char), format, deltaTime);
            }

            if (ImGui::Button(buffer))
            {
                calculateFps = !calculateFps;
            }
        }

        void local_DrawWindowsList(const char* name, std::vector<EditorWindowTypes> types)
        {
            if (ImGui::BeginMenu(name))
            {
                for (size_t i = 0; i < types.size(); i++)
                {
                    if (ImGui::MenuItem(ENUM_TO_STR(types[i])))
                    {
                        Editor::NewEditorWindow((u32)types[i]);
                    }
                }
                ImGui::EndMenu();
            }
        }

	}

}