#pragma once

#include "QC_Time.h"

#include "QF_Files.h"

#include "QE_Editor.h"
#include "QE_EditorWindow.h"
#include "QE_Projects.h"
#include "QE_Settings.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowMenuBar : public EditorWindow
		{
		public:
            EditorWindowMenuBar(GUID guid) :
                EditorWindow("MenuBar",
                    EditorWindowTypes::MenuBar,
                    guid,
                    (EditorWindowFlags)(EditorWindowFlags::Headless | EditorWindowFlags::Singleton))
            {
                m_MinimumWidth = 0.f;
                m_MinimumHeight = 0.f;
            }

            bool IsUnique() override { return true; }

		private:

			void DrawInternal() override
			{
                CheckHotkeys();

                ImGui::SetNextWindowSize({ 2160 , 50});
                if (ImGui::BeginMainMenuBar())
                {
                    ImGui::SetWindowFontScale(m_TextScale);

                    DrawFileMenu();
                    DrawEditMenu();
                    DrawStyleMenu();
                    DrawWindowsMenu();

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

                    DrawFps();

                    ImGui::SetWindowFontScale(1.f);
                }
                ImGui::EndMainMenuBar();
			}

            void CheckHotkeys()
            {
                if (Input::FrameKeyAction(eKeys::eKeys_N, eKeyState::eKeyState_Press) &&
                    (Input::IsKeyDown(eKeys::eKeys_LCTRL) || Input::IsKeyDown(eKeys::eKeys_RCTRL)))
                {
                    Editor::OpenEditorWindow(EditorWindowTypes::Prompt);
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

            void DrawFileMenu()
            {
                if (!ImGui::BeginMenu("File"))
                    return;

                if (ImGui::MenuItem("New Project...", "Ctrl+N"))
                {
                    Editor::OpenEditorWindow(EditorWindowTypes::Prompt);
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

            void DrawEditMenu()
            {
                if (!ImGui::BeginMenu("Edit"))
                    return;

                ImGui::Text("Coming Soon!");

                ImGui::EndMenu();
            }

            void DrawStyleMenu()
            {
                if (!ImGui::BeginMenu("Style"))
                    return;

                if (ImGui::MenuItem("Style Picker"))
                {
                    Editor::OpenEditorWindow(EditorWindowTypes::StylePicker);
                }

                ImGui::EndMenu();
            }

            void DrawWindowsMenu()
            {
                if (!ImGui::BeginMenu("Windows"))
                    return;

                DrawWindowsList("Scene", {
                        EditorWindowTypes::SceneControls,
                        EditorWindowTypes::SceneGraph,
                        EditorWindowTypes::SceneView,
                        EditorWindowTypes::PrefabScene
                    }
                );
                DrawWindowsList("Theme", {
                        EditorWindowTypes::StylePicker
                    }
                );
                DrawWindowsList("Editing", {
                        EditorWindowTypes::EntityInspector,
                        EditorWindowTypes::MaterialEditor,
                        EditorWindowTypes::NodeEditor,
                        EditorWindowTypes::ShaderEditor
                    }
                );
                DrawWindowsList("Other", {
                        EditorWindowTypes::Assets,
                        EditorWindowTypes::Console,
                        EditorWindowTypes::DefaultDebug,
                        // EditorWindowTypes::DockingContext, // Hidden
                        EditorWindowTypes::ImGuiDemo,
                        // EditorWindowTypes::MenuBar, // Hidden
                        EditorWindowTypes::Settings,
                        EditorWindowTypes::FolderViewer,
                        EditorWindowTypes::Prompt
                    }
                );

                ImGui::EndMenu();
            }

            void DrawFps()
            {
                const float offset = ImGui::GetWindowWidth() - (40.f * m_TextScale);
                ImGui::SameLine(offset);
                char buffer[] = { 'F', 'P', 'S', '\0' };
                if (m_CalculateFps)
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
                    m_CalculateFps = !m_CalculateFps;
                }
            }

            void DrawWindowsList(const char* name, std::vector<EditorWindowTypes> types)
            {
                if (ImGui::BeginMenu(name))
                {
                    for (size_t i = 0; i < types.size(); i++)
                    {
                        if (ImGui::MenuItem(ENUM_TO_STR(types[i])))
                        {
                            Editor::OpenEditorWindow((u32)types[i]);
                        }
                    }
                    ImGui::EndMenu();
                }
            }

            float m_TextScale = 1.20f;
            bool m_CalculateFps = true;
		};

	}

}