#include "QE_Editor.h"

#include <map>      // For std::map<std::string, const char*> pairs;
#include <string>   // For std::map<std::string, const char*> pairs;

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QC_imgui.h"
#endif

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/math.h>
#include <bx/timer.h>
#ifdef _QBGFXFRAMEWORK
#include <bgfxFramework/SampleRenderData.h>
#include <bgfxFramework/LoadShader.h>
#ifdef _QDEBUG
#include <bgfxFramework/debugDraw/debugdraw.h>
#endif // _QDEBUG
#endif // _QBGFXFRAMEWORK
#endif // _QBGFX

#include "QC_Guid.h"
#include "QC_Time.h"

#include "QF_Assets.h"
#include "QF_Directory.h"
#include "QF_Framework.h"
#include "QF_Input.h"
#include "QF_Log.h"
#include "QF_Mesh.h"
#include "QF_Projects.h"
#include "QF_Renderer.h"
#include "QF_Scene.h"
#include "QF_Scenes.h"
#include "QF_Serialization.h"
#include "QF_Settings.h"
#include "QF_Shader.h"
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
        Serialization::DeserializeObjectFromFile(Settings::GetStyleFileName(), style);
    }

	namespace Editor {

        static EditorDirtyFlags s_DirtyFlags = EditorDirtyFlags::None;

		static EntityEditor* s_EntityEditor = nullptr;
		static SceneGraph* s_SceneGraph = nullptr;
		static SceneViewer* s_SceneViewer = nullptr;

        EngineSettings* s_EngineSettings = nullptr; // #TODO Static and not shared with QF_MenuBar.cpp

		QC_ENUM(eSettingsOptions, u8, Null, Engine, User, Renderer, Project, Assets); // #TODO Project isn't settings anymore. Could be reviewed to move
		static eSettingsOptions s_SettingsEditorOption = eSettingsOptions::Null;
		static s8 s_LastPopUpIndex = -1;

#ifdef _QDEARIMGUI
		bool s_ShowingImGuiExampleWindow = false; // #TODO Static and not shared with QF_MenuBar.cpp
#endif

        void local_EditorInitialize();
        void local_Shutdown();
        void local_EditorDrawImGui(bool showEditorUI);
        void local_EditorDraw();
        void local_DrawEndFrame();
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

            pairs.insert(std::pair<const char*, const char*>("WorkspaceRootDir", WorkspaceRootDir));

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

                    local_DrawEndFrame();
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
			s_EntityEditor->OnReset();
		}

        const EditorDirtyFlags& DirtyFlags()
        {
            return s_DirtyFlags;
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
#ifdef _QDEBUG
                Scenes::GetCurrentScene()->DebugDrawImgui();
#endif
            }

            if (ImGui::Begin("Assets"))
            {
                {
                    ImGui::Text("Meshes:");
                    const std::unordered_map<GUID, Mesh*>& meshes = Assets::ViewAssets<Mesh>();
                    for (auto& guidMeshPair : meshes)
                    {
                        Mesh* mesh = guidMeshPair.second;
                        ImGui::Text("GUID: ");
                        ImGui::SameLine();
                        ImGui::Text(std::to_string(mesh->m_GUID).c_str());
                    }
                }

                {
                    ImGui::Text("Shaders:");
                    const std::unordered_map<GUID, Shader*>& shaders = Assets::ViewAssets<Shader>();
                    for (auto& guidShaderPair : shaders)
                    {
                        Shader* shader = guidShaderPair.second;
                        ImGui::Text("GUID: ");
                        ImGui::SameLine();
                        ImGui::Text(std::to_string(shader->m_GUID).c_str());
                    }
                }

                {
                    ImGui::Text("Textures:");
                }

                {
                    ImGui::Text("Materials:");
                }

                {
                    ImGui::Text("Assets Registry:");
                    auto assetRegistry = Assets::ViewRegistry();
                    for (size_t i = 0; i < assetRegistry.size(); i++)
                    {
                        ImGui::Text("GUID: ");
                        ImGui::SameLine();
                        ImGui::Text(std::to_string(assetRegistry[i].first).c_str());
                        ImGui::SameLine();
                        ImGui::Text(" File: ");
                        ImGui::SameLine();
                        ImGui::Text(assetRegistry[i].second.c_str());
                    }
                }
            }
            ImGui::End();

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
                        switch (s_LastPopUpIndex)
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

            if (s_EngineSettings->showingSchematicsEditor && ImGui::Begin("Schematics Inspector", &s_EngineSettings->showingSchematicsEditor))
            {
                // #TODO Cache result to avoid constant directory info fetching
                const std::vector<std::string> dirFileNames = Directory::ListDir(StringAppend(Paths::AssetsDir().c_str(), "Schematics/"));

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

        void local_DrawEndFrame()
        {
            {   // Debug drawer calls
                const bgfx::ViewId viewIdFbo1 = 2; // #TODO Fix hard coded value
                bgfx::touch(viewIdFbo1);

                DebugDrawEncoder& debugDrawer = Renderer::DebugDrawer(); // #TESTING
                debugDrawer.begin(viewIdFbo1);

                const bx::Vec3 normal = { .0f,  1.f, .0f };
                const bx::Vec3 pos = { .0f, .0f, .0f };

                bx::Plane plane(bx::InitNone);
                bx::calcPlane(plane, normal, pos);

                debugDrawer.drawGrid(Axis::Y, pos, 50, 1.0f);

                debugDrawer.end();
            }

            Framework::DrawFrameEnd();
        }

	}

}
