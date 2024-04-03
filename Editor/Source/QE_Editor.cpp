#include "QE_Editor.h"

#include <string> // For std::map<std::string, const char*> pairs;

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QC_imgui.h"
#endif

#include "QC_Time.h"

#include "QF_Directory.h"
#include "QF_Files.h"
#include "QF_Framework.h"
#include "QF_Input.h"
#include "QF_Log.h"
#include "QF_Scene.h"
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

    void LoadImGuiStyleFromFile()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        Serialization::DeserializeJsonFromFile(Settings::GetStyleFileName(), style);
    }

	namespace Editor {

		static EntityEditor* s_EntityEditor = nullptr;
		static SceneGraph* s_SceneGraph = nullptr;
		static SceneViewer* s_SceneViewer = nullptr;

        EngineSettings* s_EngineSettings = nullptr; // #TODO Static and not shared with QF_MenuBar.cpp

		QC_ENUM(eSettingsOptions, int, Null, Engine, Project, User, Renderer);
		static eSettingsOptions s_SettingsEditorOption = eSettingsOptions::Null;
		static s8 s_LastPopUpIndex = -1;

#ifdef _QDEARIMGUI
		bool s_ShowingImGuiExampleWindow = false; // #TODO Static and not shared with QF_MenuBar.cpp
#endif

        void local_EditorInitialize();
        void local_Shutdown();
        void local_EditorDraw();
        void local_EditorDrawImGui(bool showEditorUI);
        void local_DrawMainMenuBar();
        void local_DrawDockingContext();

		void Run(unsigned int argc, char** argv)
		{
            Log::Console("-- Qwerk Editor %s --\n", std::to_string(QWERKE_VERSION).c_str());

            std::map<std::string, const char*> pairs;
            ProgramArgsToPairs(argc, argv, pairs);

            if (pairs.find(key_ApplicationName) == pairs.end())
            {
                pairs.insert(std::pair<const char*, const char*>(key_ApplicationName, EngineName));
            }

            if (pairs.find(key_ApplicationName) == pairs.end())
            {
                pairs.insert(std::pair<const char*, const char*>(key_ProjectFileName, "Project1"));
            }

            if (pairs.find(key_ApplicationName) == pairs.end())
            {
                pairs.insert(std::pair<const char*, const char*>(key_UserName, "User1"));
            }

            pairs.insert(std::pair<const char*, const char*>("AssetsDir", AssetsDir));
            pairs.insert(std::pair<const char*, const char*>("NullAssetsDir", NullAssetsDir));

            if (true) { OutputProgramPairsInfo(pairs); }

			Framework::Initialize();

            local_EditorInitialize();
            LoadImGuiStyleFromFile();

            Scenes::Initialize();

            const EngineSettings& engineSettings = Settings::GetEngineSettings();
            Time::SetMaximumFramerate(engineSettings.limitFramerate ? engineSettings.maxFramesPerSecond : engineSettings.defaultMaxFramesPerSecond);
            Time::WriteAppStartTime();

            bool showEditorUI = true;

			while (StillRunning())
			{
				if (Time::ShouldProcessNextFrame())
				{
                    Time::StartFrame();

					Framework::NewFrame();

					if (Input::FrameKeyAction(eKeys::eKeys_Escape, eKeyState::eKeyState_Press))
					{
						Stop();
					}
                    if (Input::FrameKeyAction(eKeys::eKeys_U, eKeyState::eKeyState_Press))
                    {
                        showEditorUI = !showEditorUI;
                    }

					Framework::Update((float)Time::PreviousFrameDuration());

                    local_EditorDrawImGui(showEditorUI);

                    Framework::DrawFrameEnd();
				}
				else
				{
					YieldProcessor();
				}
			}

			Settings::SaveEngineSettings();

			Framework::Shutdown();
            local_Shutdown();
		}

		void Stop()
		{
			Window::RequestClose();
		}

		bool StillRunning()
		{
			return !Window::CloseRequested();
		}

		void OnReset()
		{
			s_EntityEditor->SetCurrentEntity(nullptr);
		}

		void local_EditorInitialize()
		{
			s_EntityEditor = new EntityEditor();
			s_SceneGraph = new SceneGraph(s_EntityEditor);
			s_SceneViewer = new SceneViewer();

			s_EngineSettings = &Settings::GetEngineSettings();
			ASSERT(s_EngineSettings, "Null engine settings!");
		}

		void local_Shutdown()
		{
			delete s_EntityEditor;
			delete s_SceneGraph;
			delete s_SceneViewer;
		}

        void local_EditorDrawImGui(bool showEditorUI)
        {
            if (Window::IsMinimized())
                return;

            Framework::DrawImguiStart();

            if (showEditorUI)
            {
                local_EditorDraw(); // Start dock context. ImGui code above won't dock
                Scenes::GetCurrentScene()->DrawImgui();
            }

            Framework::DrawImguiEnd();
        }

        void local_DrawStylePicker(bool save);

		void local_EditorDraw()
		{
            if (Window::IsMinimized())
                return;

            local_DrawMainMenuBar();
            local_DrawDockingContext();

#ifdef _QDEARIMGUI
            if (s_ShowingImGuiExampleWindow)
            {
                ImGui::ShowDemoWindow(&s_ShowingImGuiExampleWindow);
            }

            if (s_EngineSettings->showingSettingsEditor &&
                ImGui::Begin("Settings Inspector", &s_EngineSettings->showingSettingsEditor))
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

                    case eSettingsOptions::Project:
                        pushIsDirtyStyleColor = Settings::GetProjectSettings().isDirty;
                        break;

                    case eSettingsOptions::User:
                        pushIsDirtyStyleColor = Settings::GetUserSettings().isDirty;
                        break;

                    case eSettingsOptions::Renderer:
                        pushIsDirtyStyleColor = Settings::GetRendererSettings().isDirty;
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
                        s_SettingsEditorOption = eSettingsOptions::_from_index(i);
                    }

                    if (ImGui::IsItemClicked(ImGui::Buttons::MouseRight))
                    {
                        s_LastPopUpIndex = (s8)i;
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
                        switch (s_LastPopUpIndex)
                        {
                        case eSettingsOptions::Engine:
                            Settings::SaveEngineSettings();
                            break;

                        case eSettingsOptions::Project:
                            Settings::SaveProjectSettings();
                            break;

                        case eSettingsOptions::User:
                            Settings::SaveUserSettings();
                            break;

                        case eSettingsOptions::Renderer:
                            Settings::SaveRendererSettings();
                            break;
                        }
                    }

                    if (ImGui::MenuItem("Reload"))
                    {
                        // #TODO Load user settings instead of default
                        switch (s_LastPopUpIndex)
                        {
                        case eSettingsOptions::Engine:
                            Settings::LoadEngineSettings();
                            break;

                        case eSettingsOptions::Project:
                            Settings::LoadProjectSettings(ProjectsFolderPath("Project1.qproj")); // #TODO Load proper project file
                            break;

                        case eSettingsOptions::User:
                            Settings::LoadUserSettings(SettingsFolderPath("User1.qproj"));
                            break;

                        case eSettingsOptions::Renderer:
                            Settings::LoadRendererSettings(SettingsFolderPath("RendererSettings1.qren"));
                            break;
                        }
                    }
                    ImGui::EndPopup();
                }

                if (s_SettingsEditorOption != (eSettingsOptions)eSettingsOptions::Null)
                {
                    std::string buffer = "";
                    buffer.reserve(200);
                    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.3f);

                    ImGui::Text(ENUM_TO_STR(eSettingsOptions::_from_index(s_SettingsEditorOption)));

                    switch (s_SettingsEditorOption)
                    {
                    case eSettingsOptions::Engine:
                        {
                            EngineSettings& engineSettings = Settings::GetEngineSettings();
                            engineSettings.isDirty |= Inspector::InspectFieldRecursive(Mirror::InfoForType<EngineSettings>(), &engineSettings, buffer);
                        }
                        break;

                    case eSettingsOptions::Project:
                        {
                            ProjectSettings& projectSettings = Settings::GetProjectSettings();
                            projectSettings.isDirty |= Inspector::InspectFieldRecursive(Mirror::InfoForType<ProjectSettings>(), &projectSettings, buffer);
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
                    }

                    ImGui::PopItemWidth();
                }

                ImGui::End();
            }

            if (s_EngineSettings->showingSchematicsEditor && ImGui::Begin("Schematics Inspector", &s_EngineSettings->showingSchematicsEditor))
            {
                // #TODO Cache result to avoid constant directory info fetching
                const std::vector<std::string> dirFileNames = Directory::ListDir(SchematicsFolderPath(""));

                for (size_t i = 0; i < dirFileNames.size(); i++)
                {
                    ImGui::Button(dirFileNames.at(i).c_str());
                }

                ImGui::End();
            }

            if (s_EngineSettings->showingStylePicker)
            {
                local_DrawStylePicker(true);
            }
#endif

            s_SceneViewer->Draw();
            s_SceneGraph->Draw();
            s_EntityEditor->Draw();
		}

	}

}
