#include "QE_Editor.h"

#include <string> // For std::map<std::string, const char*> pairs;

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QC_imgui.h"
#endif

#include "QC_Time.h"

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

    void LoadImGuiStyleFromFile()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        Serialization::DeserializeJsonFromFile(Settings::GetStyleFileName(), style);
    }

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

                    Framework::DrawImguiStart();

                    if (showEditorUI)
                    {
                        local_EditorDraw();
                    }

					Framework::DrawImguiEnd();

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

        void local_DrawStylePicker(bool save);

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

            if (s_EngineSettings->showingStylePicker)
            {
                local_DrawStylePicker(true);
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

        void local_DrawStylePicker(bool save) // #TODO Create a new StyleEditor class/UI window
        {
            // #TODO Add presets and load from file (presets can be files too instead of hard coded)

            static bool isOpen = true;
            bool edited = false;
            ImGuiStyle& style = ImGui::GetStyle();

            if (ImGui::Begin("Style Picker", &isOpen))
            {
                const Mirror::TypeInfo* styleTypeInfo = Mirror::InfoForType<ImGuiStyle>();

                for (size_t i = 0; i < styleTypeInfo->fields.size(); i++)
                {
                    // #TODO Look at using the inspector to render widgets for ImGuiStyle members

                    const Mirror::Field& field = styleTypeInfo->fields[i];

                    if (field.typeInfo->enumType == MirrorTypes::m_float &&
                        strcmp(styleTypeInfo->fields[i].name.c_str(), "Alpha") == 0)
                    {
                        ImGui::Text("Alpha");
                        ImGui::SameLine();
                        ImGui::DragFloat("##Alpha", &style.Alpha, .01f, .01f, 1.f); // #NOTE .01f minimum or ImGui will say End() wasn't called
                        continue;
                    }

                    // #TODO Edit save if () { edited = true; }

                    switch (field.typeInfo->enumType)
                    {
                    case MirrorTypes::m_float:
                        ImGui::Text(field.name.c_str());
                        // ImGui::SameLine();
                        // ImGui::DragFloat(field.name.c_str(), (float*)((char*)&style + field.offset), 0.1f, 0.f, 1.f);
                        break;

                    case MirrorTypes::ImVec2:
                        ImGui::Text(field.name.c_str());
                        // ImGui::SameLine();
                        // ImGui::DragFloat("##WindowPaddingX", &style.WindowPadding.x, 0.1f, 0.f, 1000.f);
                        // ImGui::SameLine();
                        // ImGui::DragFloat("##WindowPaddingY", &style.WindowPadding.y, 0.1f, 0.f, 1000.f);
                        break;

                    case MirrorTypes::m_int:
                        ImGui::Text(field.name.c_str());
                        break;

                    case MirrorTypes::m_imvec4_array:
                        if (ImGui::CollapsingHeader(styleTypeInfo->fields[i].name.c_str(), ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_SpanAvailWidth))
                        {
                            ImGui::Text("Info:");

                            ImGui::SameLine();
                            static bool uiOptionsEnabled = false;
                            ImGui::Checkbox("ColourPickerOptionsCheck", &uiOptionsEnabled);

                            ImGui::SameLine();
                            static float uiScalar = .8f;
                            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                            ImGui::SliderFloat("ColourPickerSlider", &uiScalar, .1f, 1.2f);
                            ImGui::PopItemWidth();

                            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * uiScalar);

                            ImGuiColorEditFlags_ flags = static_cast<ImGuiColorEditFlags_>(
                                ImGuiColorEditFlags_NoSidePreview |
                                ImGuiColorEditFlags_PickerHueWheel |
                                ImGuiColorEditFlags_NoLabel
                                );
                            flags = static_cast<ImGuiColorEditFlags_>(flags | (uiOptionsEnabled ? ImGuiColorEditFlags_None : ImGuiColorEditFlags_NoInputs));

                            const size_t range = ImGuiCol_qw::_size_constant - 1;
                            for (int i = 0; i < range; i++)
                            {
                                ImGui::Text(ENUM_TO_STR(ImGuiCol_qw::_from_index(i)));
                                if (ImGui::ColorPicker4(ENUM_TO_STR(ImGuiCol_qw::_from_index(i)), (float*)&style.Colors[i], flags)) { edited = true; }
                            }

                            ImGui::PopItemWidth();
                        }
                        break;

                    default:
                        break;
                    }

                    if (strcmp(styleTypeInfo->fields[i].name.c_str(), "DisabledAlpha") == 0)
                    {
                        ImGui::Text("DisabledAlpha");
                        ImGui::SameLine();
                        ImGui::DragFloat("##DisabledAlpha", &style.DisabledAlpha, 0.1f, 0.f, 1.f);
                    }
                }
            }
            ImGui::End();

            // style.FrameRounding = 4.0f;
            // style.WindowBorderSize = 0.0f;

            // if (save && edited)
            if (edited)
            {
                Serialization::SerializeObjectToFile(style, Settings::GetStyleFileName());
            }
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
