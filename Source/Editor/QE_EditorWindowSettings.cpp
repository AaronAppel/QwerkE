#include "QE_EditorWindowSettings.h"

#include "QC_StringHelpers.h"

#include "QF_Directory.h"
#include "QF_Input.h"

#include "QE_EditorInspector.h"
#include "QF_Paths.h"
#include "QE_Projects.h"
#include "QE_Settings.h"

namespace QwerkE {

	namespace Editor {

        void EditorWindowSettings::DrawInternal()
        {
            EngineSettings& engineSettings = Settings::GetEngineSettings();

            for (size_t i = 1; i < eSettingsOptions::_size_constant; i++)
            {
                if (i > 1) ImGui::SameLine();

                bool pushIsDirtyStyleColor = false;

                switch (i)
                {
                case eSettingsOptions::Engine:
                    pushIsDirtyStyleColor = Settings::GetEngineSettings().isDirty;
                    break;

                case eSettingsOptions::GameActions:
                    // pushIsDirtyStyleColor = Input::GetGameActions().isDirty;
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

                    case eSettingsOptions::GameActions:
                        // Save Input::GetGameActions();
                        break;

                    case eSettingsOptions::Renderer:
                        Settings::SaveRendererSettings();
                        break;

                    case eSettingsOptions::Project:
                        Projects::SaveProject();
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

                    case eSettingsOptions::GameActions:
                        // Load Input::GetGameActions();
                        break;

                    case eSettingsOptions::Renderer:
                        Settings::LoadRendererSettings("RendererSettings1.qren");
                        break;

                    case eSettingsOptions::Project:
                        Projects::LoadProject("Project1.qproj"); // #TODO Load proper project file
                        Editor::OnSceneReloaded();
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
                    ImGui::SameLine();
                    ImGui::Text("          Changes require engine restart");
                    EngineSettings& engineSettings = Settings::GetEngineSettings();
                    engineSettings.isDirty |= Inspector::InspectType(Mirror::InfoForType<EngineSettings>(), &engineSettings, buffer);
                }
                break;

                case eSettingsOptions::GameActions:
                {
                    Input::GameActions& gameActions = Input::GetGameActions();
                    // userSettings.isDirty |=
                    Inspector::InspectType(Mirror::InfoForType<Input::GameActions>(), &gameActions, buffer);
                }
                break;

                case eSettingsOptions::Renderer:
                {
                    RendererSettings& rendererSettings = Settings::GetRendererSettings();
                    rendererSettings.isDirty |= Inspector::InspectType(Mirror::InfoForType<RendererSettings>(), &rendererSettings, buffer);
                }
                break;

                case eSettingsOptions::Project:
                {
                    Project& project = Projects::CurrentProject();
                    project.isDirty |= Inspector::InspectType(Mirror::InfoForType<Project>(), &project, buffer);
                }
                break;
                }

                ImGui::PopItemWidth();
            }

            // #TODO Move to a new Schematics/Prefab Inspector EditorWindow
            if (false && ImGui::Begin("Schematics Inspector", NULL))
            {
                // #TODO Cache result to avoid constant directory info fetching

                for (auto& directoryEntry : Directory::ListDir(StringAppend(Paths::AssetsDir().c_str(), "Schematics/")))
                {
                    // #NOTE Crashes when viewing file names containing emojis
                    const auto& path = directoryEntry.path();
                    std::string filenameString = path.filename().string();

                    ImGui::Button(filenameString.c_str());
                }

                ImGui::End();
            }
        }

	}

}