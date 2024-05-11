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

                if (ImGui::BeginMainMenuBar())
                {
                    if (ImGui::BeginMenu("File"))
                    {
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

                    if (ImGui::BeginMenu("Windows"))
                    {
                        DrawMenu("Scene", {
                                EditorWindowTypes::SceneControls,
                                EditorWindowTypes::SceneGraph,
                                EditorWindowTypes::SceneView,
                                EditorWindowTypes::PrefabScene
                            }
                        );
                        DrawMenu("Theme", {
                                EditorWindowTypes::StylePicker
                            }
                        );
                        DrawMenu("Editing", {
                                EditorWindowTypes::MaterialEditor,
                                EditorWindowTypes::NodeEditor,
                                EditorWindowTypes::ShaderEditor
                            }
                        );
                        DrawMenu("Other", {
                                EditorWindowTypes::Assets,
                                EditorWindowTypes::DefaultDebug,
                                // EditorWindowTypes::DockingContext, // Hidden
                                EditorWindowTypes::EntityInspector,
                                EditorWindowTypes::ImGuiDemo,
                                // EditorWindowTypes::MenuBar, // Hidden
                                EditorWindowTypes::Settings,
                                EditorWindowTypes::FolderViewer,
                                EditorWindowTypes::Prompt
                            }
                        );
                        ImGui::EndMenu();
                    }

                    const Project& currentProject = Projects::CurrentProject();
                    if (strcmp(currentProject.projectFileName.c_str(), Constants::gc_DefaultStringValue) == 0)
                    {
                        const char defaultProjectName[] = { 'N','o',' ','P','r','o','j','e','c','t','\0'};
                        ImGui::SameLineCentered(sizeof(defaultProjectName));
                        ImGui::Text(defaultProjectName);
                    }
                    else
                    {
                        ImGui::SameLineCentered(currentProject.projectName.size());
                        ImGui::Text(currentProject.projectName.c_str());
                    }

                    {   // FPS
                        const float offset = ImGui::GetWindowWidth() - 40.f;
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

            void DrawMenu(const char* name, std::vector<EditorWindowTypes> types)
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

            bool m_CalculateFps = true;
		};

	}

}