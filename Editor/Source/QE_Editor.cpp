#include "QE_Editor.h"

#include <string> // For std::map<std::string, const char*> pairs;

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QC_imgui.h"
#endif

#include "QC_Time.h"

#include "QF_FileUtilities.h"
#include "QF_Framework.h"
#include "QF_Input.h"
#include "QF_Log.h"
#include "QF_Scenes.h"
#include "QF_Settings.h"
#include "QF_Window.h"

#include "QE_EditorInspector.h"
#include "QE_EntityEditor.h"
#include "QE_ProgramArgs.h"
#include "QE_SceneGraph.h"
#include "QE_SceneViewer.h"

namespace QwerkE {

	namespace Editor {

		static EntityEditor* s_EntityEditor = nullptr;
		static SceneGraph* s_SceneGraph = nullptr;
		static SceneViewer* s_SceneViewer = nullptr;

		static EngineSettings* s_EngineSettings = nullptr;

		QC_ENUM(eSettingsOptions, int, Null, Engine, Project, User, Renderer);
		static eSettingsOptions s_SettingsEditorOption = eSettingsOptions::Null;
		static s8 s_LastPopUpIndex = -1;

#ifdef _QDEARIMGUI
		static bool s_ShowingImGuiExampleWindow = false;
#endif

        void local_EditorInitialize();
        void local_Shutdown();
        void local_EditorDraw();
        void local_DrawMainMenuBar();
        void local_RenderDockingContext();

		void Run(unsigned int argc, char** argv)
		{
            Log::Console("-- Qwerk Editor %s --\n", std::to_string(QWERKE_VERSION).c_str());

            std::map<std::string, const char*> pairs;
            ProgramArgsToPairs_Windows(argc, argv, pairs);

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

            if (true) { OutputProgramPairsInfo_Windows(pairs); }

			Framework::Initialize();
            // Scenes::LoadFromProjectSettings();

            local_EditorInitialize();

            Scenes::Initialize();

			const EngineSettings& engineSettings = Settings::GetEngineSettings();
			const u16 FPS_MAX = (int)(engineSettings.limitFramerate) * engineSettings.maxFramesPerSecond;
			const float SAFE_FPS_MAX = 360.f;
			const float FPS_MAX_DELTA = FPS_MAX ? 1.f / (float)FPS_MAX : 1.f / SAFE_FPS_MAX;

			double lastFrameTime = Time::Now();
			double timeUntilNextFrame = 0.;

			Time::InitStartTime();

			while (StillRunning())
			{
				if (timeUntilNextFrame >= FPS_MAX_DELTA)
				{
					Framework::NewFrame();

					if (Input::FrameKeyAction(eKeys::eKeys_Escape, eKeyState::eKeyState_Press))
					{
						Stop();
					}

					Framework::Update(Time::FrameDelta());

					local_EditorDraw();

					Framework::Draw();

					Time::EndFrame();

					timeUntilNextFrame = 0.;
				}
				else
				{
					YieldProcessor();
				}

				const double now = Time::Now();
				const double frameDelta = now - lastFrameTime;
				timeUntilNextFrame += frameDelta;
				lastFrameTime = now;
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

		void local_EditorDraw()
		{
            if (Window::IsMinimized())
                return;

            local_DrawMainMenuBar();
            local_RenderDockingContext();

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
                const auto dirFileNamesUPtr = ReadDir(SchematicsFolderPath(""));
                const auto fileNames = dirFileNamesUPtr.get();

                for (size_t i = 0; i < fileNames->size(); i++)
                {
                    ImGui::Button(fileNames->at(i).c_str());
                }

                ImGui::End();
            }
#endif

            s_SceneViewer->Draw();
            s_SceneGraph->Draw();
            s_EntityEditor->Draw();
		}

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
                    if (ImGui::ListBox("", &index, d, size, 3))
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

                    if (ImGui::Button("FPS"))
                    {
                        s_EngineSettings->showingFPS = !s_EngineSettings->showingFPS;
                        Settings::SaveEngineSettings();
                    }

                    ImGui::EndMenu();
                }

                ImGui::SameLine(ImGui::GetWindowWidth() - 36); // 4 characters * 9 pixels each
                if (s_EngineSettings->showingFPS)
                {
                    char buffer[] = { '0', '0', '0', '\0' };

                    const float& deltaTime = Time::FrameDelta();
                    if (deltaTime == .0f)
                    {
                        snprintf(buffer, sizeof(buffer) / sizeof(char), "%1.1f", deltaTime);
                    }
                    else
                    {
                        snprintf(buffer, sizeof(buffer) / sizeof(char), "%1.1f", 1.f / deltaTime);
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
                //         ImGui::Text("%1.1f", 0.f);
                //     }
                //     else
                //     {
                //         ImGui::Text("%1.1f", 1.0f / deltaTime);
                //     }
                //     ImGui::PopItemWidth();
                // }

                ImGui::EndMainMenuBar();
            }
#endif
        }

        void local_RenderDockingContext()
        {
#ifdef _QDEARIMGUI
            // Copied from imgui docking example
            static bool opt_fullscreen_persistant = true;
            bool opt_fullscreen = opt_fullscreen_persistant;

            ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
            static ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

            if (opt_fullscreen) // fullscreen
            {
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->Pos);
                ImGui::SetNextWindowSize(viewport->Size);
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            }

            static bool open = true;

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace Demo", &open, window_flags);
            ImGui::PopStyleVar();

            if (opt_fullscreen)
                ImGui::PopStyleVar(2);

            ImGuiIO& io = ImGui::GetIO();
            ImGuiConfigFlags_DockingEnable;
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("QwerkEDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }

            static bool dockspaceOpen = true;

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Docking"))
                {
                    // Disabling fullscreen would allow the window to be moved to the front of other windows,
                    // which we can't undo at the moment without finer window depth/z control.
                    //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

                    if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 dockspace_flags ^= ImGuiDockNodeFlags_NoSplit;
                    if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
                    if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
                    if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0))     dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
                    if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
                    ImGui::Separator();
                    if (ImGui::MenuItem("Close DockSpace", NULL, false, dockspaceOpen != NULL))
                    {
                        dockspaceOpen = false;
                    }
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }

            ImGui::End();
#endif
        }

	}

}
