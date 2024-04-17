#pragma once

#ifdef _QENUM
#include "Libraries/enum/QC_enum.h"
#endif

#include "QF_Settings.h"

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

        QC_ENUM(eSettingsOptions, u8, Null, Engine, User, Renderer, Project, Assets); // #TODO Project isn't settings anymore. Could be reviewed to move

        class EditorWindowSettings : public EditorWindow
        {
        public:
            EditorWindowSettings() : EditorWindow("Settings") { }

        private:
            void DrawInternal() override
            {
                EngineSettings& engineSettings = Settings::GetEngineSettings();
                if (engineSettings.showingSettingsEditor &&
                    ImGui::Begin("Settings Inspector", &engineSettings.showingSettingsEditor))
                {
                    for (size_t i = 1; i < eSettingsOptions::_size_constant; i++)
                    {
                        if (i > 1) ImGui::SameLine();

                        bool pushIsDirtyStyleColor = false;

                        switch (i)
                        {
                        case eSettingsOptions::Engine:
                            pushIsDirtyStyleColor = Settings::GetEngineSettings().isDirty;
                            break;

                        case eSettingsOptions::User:
                            pushIsDirtyStyleColor = Settings::GetUserSettings().isDirty;
                            break;

                        case eSettingsOptions::Renderer:
                            pushIsDirtyStyleColor = Settings::GetRendererSettings().isDirty;
                            break;

                        case eSettingsOptions::Project:
                            pushIsDirtyStyleColor = Projects::CurrentProject().isDirty;
                            break;
                        }

                        if (pushIsDirtyStyleColor)
                        {
                            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.f, 0.6f, 0.6f));
                            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1.f, 0.6f, 0.6f));
                            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1.f, 0.6f, 0.6f));
                        }

                        if (ImGui::Button(ENUM_TO_STR(eSettingsOptions::_from_index(i))))
                        {
                            m_SettingsEditorOption = eSettingsOptions::_from_index(i);
                        }

                        if (ImGui::IsItemClicked(ImGui::Buttons::MouseRight))
                        {
                            m_LastPopUpIndex = (s8)i;
                            ImGui::OpenPopup("Settings Context Pop Up");
                        }

                        if (pushIsDirtyStyleColor)
                        {
                            ImGui::PopStyleColor(3);
                        }
                    }

                    if (ImGui::BeginPopup("Settings Context Pop Up"))
                    {
                        if (ImGui::MenuItem("Save"))
                        {
                            switch (m_LastPopUpIndex)
                            {
                            case eSettingsOptions::Engine:
                                Settings::SaveEngineSettings();
                                break;

                            case eSettingsOptions::User:
                                Settings::SaveUserSettings();
                                break;

                            case eSettingsOptions::Renderer:
                                Settings::SaveRendererSettings();
                                break;

                            case eSettingsOptions::Project:
                                Projects::SaveProject();
                                break;

                            case eSettingsOptions::Assets:
                                Assets::SaveRegistry();
                                break;
                            }
                        }

                        if (ImGui::MenuItem("Reload"))
                        {
                            // #TODO Load user settings instead of default
                            switch (m_LastPopUpIndex)
                            {
                            case eSettingsOptions::Engine:
                                Settings::LoadEngineSettings("");
                                break;

                            case eSettingsOptions::User:
                                Settings::LoadUserSettings("User1.qproj");
                                break;

                            case eSettingsOptions::Renderer:
                                Settings::LoadRendererSettings("RendererSettings1.qren");
                                break;

                            case eSettingsOptions::Project:
                                Projects::LoadProject("Project1.qproj"); // #TODO Load proper project file
                                break;
                            }
                        }
                        ImGui::EndPopup();
                    }

                    if (m_SettingsEditorOption != (eSettingsOptions)eSettingsOptions::Null)
                    {
                        std::string buffer = "";
                        buffer.reserve(200);
                        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.3f);

                        ImGui::Text(ENUM_TO_STR(eSettingsOptions::_from_index(m_SettingsEditorOption)));

                        switch (m_SettingsEditorOption)
                        {
                        case eSettingsOptions::Engine:
                        {
                            EngineSettings& engineSettings = Settings::GetEngineSettings();
                            engineSettings.isDirty |= Inspector::InspectFieldRecursive(Mirror::InfoForType<EngineSettings>(), &engineSettings, buffer);
                        }
                        break;

                        case eSettingsOptions::User:
                        {
                            UserSettings& userSettings = Settings::GetUserSettings();
                            userSettings.isDirty |= Inspector::InspectFieldRecursive(Mirror::InfoForType<UserSettings>(), &userSettings, buffer);
                        }
                        break;

                        case eSettingsOptions::Renderer:
                        {
                            RendererSettings& rendererSettings = Settings::GetRendererSettings();
                            rendererSettings.isDirty |= Inspector::InspectFieldRecursive(Mirror::InfoForType<RendererSettings>(), &rendererSettings, buffer);
                        }
                        break;

                        case eSettingsOptions::Project:
                        {
                            Project& project = Projects::CurrentProject();
                            project.isDirty |= Inspector::InspectFieldRecursive(Mirror::InfoForType<Project>(), &project, buffer);
                        }
                        break;
                        }

                        ImGui::PopItemWidth();
                    }

                    ImGui::End();
                }

                if (engineSettings.showingSchematicsEditor && ImGui::Begin("Schematics Inspector", &engineSettings.showingSchematicsEditor))
                {
                    // #TODO Cache result to avoid constant directory info fetching
                    const std::vector<std::string> dirFileNames = Directory::ListDir(StringAppend(Paths::AssetsDir().c_str(), "Schematics/"));

                    for (size_t i = 0; i < dirFileNames.size(); i++)
                    {
                        ImGui::Button(dirFileNames.at(i).c_str());
                    }

                    ImGui::End();
                }
            }

        private:
            eSettingsOptions m_SettingsEditorOption = eSettingsOptions::Null;
            s8 m_LastPopUpIndex = -1;
		};

	}

}