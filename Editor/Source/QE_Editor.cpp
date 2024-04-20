#include "QE_Editor.h"

#include <map>      // For std::map<std::string, const char*> pairs; and EditorWindows collection
#include <string>   // For std::map<std::string, const char*> pairs;
#include <vector>   // For s_EditorWindowsQueuedForDelete

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
#include "QC_System.h"
#include "QC_Time.h"

#include "QF_Assets.h"
#include "QF_Directory.h"
#include "QF_Files.h"
#include "QF_Framework.h"
#include "QF_Input.h"
#include "QF_Log.h"
#include "QF_Mesh.h"
#include "QF_PathDefines.h"
#include "QF_Paths.h"
#include "QF_Projects.h"
#include "QF_Renderer.h"
#include "QF_Scene.h"
#include "QF_Scenes.h"
#include "QF_Serialization.h"
#include "QF_Settings.h"
#include "QF_Window.h"

#include "QE_ProgramArgs.h"

// #TODO Remove after adding indirection
#include "QE_EditorWindow.h"
#include "QE_EditorWindowAssets.h"
#include "QE_EditorWindowDefaultDebug.h"
#include "QE_EditorWindowDockingContext.h"
#include "QE_EditorWindowEntityInspector.h"
#include "QE_EditorWindowFolderViewer.h"
#include "QE_EditorWindowImGuiDemo.h"
#include "QE_EditorWindowMaterialEditor.h"
#include "QE_EditorWindowMenuBar.h"
#include "QE_EditorWindowNodeEditor.h"
#include "QE_EditorWindowSceneControls.h"
#include "QE_EditorWindowSceneGraph.h"
#include "QE_EditorWindowSceneView.h"
#include "QE_EditorWindowSettings.h"
#include "QE_EditorWindowShaderEditor.h"
#include "QE_EditorWindowStylePicker.h"

namespace QwerkE {

    void LoadImGuiStyleFromFile() // #TODO Move somewhere else
    {
        ImGuiStyle& style = ImGui::GetStyle();
        Serialization::DeserializeObjectFromFile(Settings::GetStyleFileName(), style);
    }

	namespace Editor {

        static bool s_ShowingEditorUI = true;

        static EditorWindowDefaultDebug s_EditorWindowDefaultDebug(GUID::Invalid); // #TODO Review serializaing and using persistent guids
        static EditorWindowDockingContext s_EditorWindowDockingContext(GUID::Invalid);
        static EditorWindowImGuiDemo s_EditorWindowImGuiDemo(GUID::Invalid);
        static EditorWindowMenuBar s_EditorWindowMenuBar(GUID::Invalid);

        static std::unordered_map<GUID, EditorWindow*> s_EditorWindows;
        static std::vector<EditorWindow*> s_EditorWindowsQueuedForDelete;

        constexpr char* s_EditorWindowDataFileName = "EditorWindowData.qdata";

        static EditorStateFlags s_EditorStateFlags = EditorStateFlags::EditorStateFlagsNone;

        void local_Initialize();
        void local_Shutdown();
        void local_Update();
        void local_EndFrame();

		void Run(unsigned int argc, char** argv)
		{
            Log::Console("-- Qwerk Editor %s --\n", std::to_string(QWERKE_VERSION).c_str());

            std::map<std::string, const char*> pairs;
            ProgramArgsToPairs(argc, argv, pairs);

            if (pairs.find(key_ApplicationName) == pairs.end())
            {
                pairs.insert(std::pair<const char*, const char*>(key_ApplicationName, EngineName));
            }

            if (pairs.find(key_ProjectFileName) == pairs.end())
            {
                pairs.insert(std::pair<const char*, const char*>(key_ProjectFileName, "Project1"));
            }
            else
            {
                // #TODO Load last opened project
            }

            if (pairs.find(key_UserName) == pairs.end())
            {
                std::string userName = System::UserName();
                pairs.insert(std::pair<const char*, const char*>(key_UserName, strdup(userName.c_str())));
            }

            pairs.insert(std::pair<const char*, const char*>("WorkspaceRootDir", WorkspaceRootDir));

            if (true) { OutputProgramPairsInfo(pairs); }

			Framework::Initialize();

             // #TODO Move somewhere better
            std::string userSettingsFileName = pairs[key_UserName];
            if (userSettingsFileName == Constants::gc_DefaultStringValue)
            {
                userSettingsFileName = "User1"; // Rename "DefaultUser"
            }

            userSettingsFileName += ".";
            userSettingsFileName += preferences_ext;
            Settings::LoadUserSettings(userSettingsFileName.c_str());

            const UserSettings& userSettings = Settings::GetUserSettings();
            std::string projectFileName = userSettings.lastOpenedProjectFileName;
            if (projectFileName == Constants::gc_DefaultStringValue)
            {
                projectFileName = "Project1.qproj";
            }
            Projects::LoadProject(projectFileName.c_str());
            const Project& project = Projects::CurrentProject();
            std::string engineSettingsFileName = project.lastOpenedEngineSettingsFileName;
            if (engineSettingsFileName == Constants::gc_DefaultStringValue)
            {
                engineSettingsFileName = null_config;
            }
            Settings::LoadEngineSettings(engineSettingsFileName.c_str());

            {   // Load scenes
                const std::vector<std::string> sceneFileNames = project.sceneFileNames; // #NOTE Copied not referenced

                for (size_t i = 0; i < sceneFileNames.size(); i++)
                {
                    const char* sceneFileName = sceneFileNames[i].c_str();
                    if (!Files::Exists(Paths::Scene(sceneFileName).c_str()))
                    {
                        LOG_WARN("Initialize(): File not found: {0}", sceneFileName);
                        continue;
                    }

                    Scenes::CreateSceneFromFile(Paths::Scene(sceneFileName).c_str(), true);
                }

                if (Scenes::SceneCount() < 1)
                {
                    Scenes::CreateSceneFromFile(Paths::NullAsset(null_scene), true);
                    LOG_WARN("Null scene loaded as no scene files found for project: {0}", project.projectName);
                }
            }

            LoadImGuiStyleFromFile();

            local_Initialize();

            const EngineSettings& engineSettings = Settings::GetEngineSettings();
            Time::SetMaximumFramerate(engineSettings.limitFramerate ? engineSettings.maxFramesPerSecond : engineSettings.defaultMaxFramesPerSecond);
            Time::WriteAppStartTime();

			while (StillRunning())
			{
				if (Time::ShouldProcessNextFrame())
				{
                    Time::StartFrame();

					Framework::StartFrame();

                    Renderer::StartImGui();

                    if (s_ShowingEditorUI)
                    {
                        s_EditorWindowDockingContext.Draw();
                        s_EditorWindowMenuBar.Draw();
                        s_EditorWindowDefaultDebug.Draw();
                        s_EditorWindowImGuiDemo.Draw();
                    }

                    local_Update();

					Framework::Update((float)Time::PreviousFrameDuration());

                    Renderer::EndImGui();

                    local_EndFrame();
				}
				else
				{
					YieldProcessor();
				}
			}

			Settings::SaveEngineSettings();

            // #TODO Save editor state for next launch

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

        const EditorStateFlags& GetEditorStateFlags()
        {
            return s_EditorStateFlags;
        }

        void ToggleEditorStateFlags(const EditorStateFlags& flags)
        {
            s_EditorStateFlags = (EditorStateFlags)(s_EditorStateFlags ^ flags);
        }

        bool ShowingEditorUI()
        {
            return s_ShowingEditorUI;
        }

        void OpenEditorWindow(u32 enumToInt)
        {
            EditorWindowTypes editorWindowType = EditorWindowTypes::_from_index(enumToInt);
            EditorWindow* newWindow = nullptr;

            // #TODO Handle unique windows?
            switch (editorWindowType)
            {
            case EditorWindowTypes::DefaultDebug:
                s_EditorWindowDefaultDebug.ToggleHidden();
                break;
            case EditorWindowTypes::DockingContext:
                s_EditorWindowDockingContext.ToggleHidden();
                break;
            case EditorWindowTypes::ImGuiDemo:
                s_EditorWindowImGuiDemo.ToggleHidden();
                break;
            case EditorWindowTypes::MenuBar:
                s_EditorWindowMenuBar.ToggleHidden();
                break;

            /////////////////////

            case EditorWindowTypes::Assets:
                newWindow = new EditorWindowAssets();
                break;
            case EditorWindowTypes::EntityInspector:
                newWindow = new EditorWindowEntityInspector();
                break;
            case EditorWindowTypes::SceneControls:
                newWindow = new EditorWindowSceneControls();
                break;
            case EditorWindowTypes::SceneGraph:
                newWindow = new EditorWindowSceneGraph();
                break;
            case EditorWindowTypes::SceneView:
                {
                    constexpr u8 defaultTextureId = 4;
                    constexpr u8 defaultViewId = 2;
                    newWindow = new EditorWindowSceneView(defaultTextureId, defaultViewId);
                }
                break;
            case EditorWindowTypes::Settings:
                newWindow = new EditorWindowSettings();
                break;
            case EditorWindowTypes::StylePicker:
                newWindow = new EditorWindowStylePicker();
                break;
            case EditorWindowTypes::MaterialEditor:
                newWindow = new EditorWindowMaterialEditor();
                break;
            case EditorWindowTypes::FolderViewer:
                newWindow = new EditorWindowFolderViewer();
                break;
            case EditorWindowTypes::NodeEditor:
                newWindow = new EditorWindowNodeEditor();
                break;
            case EditorWindowTypes::ShaderEditor:
                newWindow = new EditorWindowShaderEditor();
                break;

            case EditorWindowTypes::EditorWindowTypesInvalid:
            default:
                LOG_ERROR("{0} Unsupported EditorWindow type!", __FUNCTION__);
                break;
            }

            if (newWindow)
            {
                s_EditorWindows[newWindow->Guid()] = newWindow;
            }
        }

        void CloseEditorWindow(const GUID& guid)
        {
            if (s_EditorWindows.find(guid) != s_EditorWindows.end())
            {
                s_EditorWindowsQueuedForDelete.push_back(s_EditorWindows[guid]);
            }
            else
            {
                LOG_WARN("{0} Could not close editor window with GUID {1}", __FUNCTION__, guid);
            }
        }

        void ResetEditorWindowReferences()
        {
            // s_EntityEditor->ResetReferences();
            // s_EditorWindows[guid]->ResetReferences();
        }

        void OnEntitySelected(const EntityHandle& entity)
        {
            for (auto& it : s_EditorWindows)
            {
                it.second->OnEntitySelected(entity);
            }
        }

		void local_Initialize()
		{
            Serialization::DeserializeObjectFromFile(Paths::Settings(s_EditorWindowDataFileName).c_str(), s_EditorWindows);
		}

		void local_Shutdown()
		{
            Serialization::SerializeObjectToFile(s_EditorWindows, Paths::Settings(s_EditorWindowDataFileName).c_str());

            auto it = s_EditorWindows.begin();
            while (it != s_EditorWindows.end())
            {
                it = s_EditorWindows.erase(it);
            }
            s_EditorWindows.clear();
		}

        void local_Update()
        {
            if (Input::FrameKeyAction(eKeys::eKeys_Escape, eKeyState::eKeyState_Press))
            {
                Stop();
            }

            if (Input::FrameKeyAction(eKeys::eKeys_U, eKeyState::eKeyState_Press) &&
                (Input::IsKeyDown(eKeys::eKeys_LCTRL) || Input::IsKeyDown(eKeys::eKeys_RCTRL)))
            {
                s_ShowingEditorUI = !s_ShowingEditorUI;
            }

            constexpr size_t numberOfHotkeyedScenes = eKeys::eKeys_F12 - eKeys::eKeys_F1 + 1;
            for (size_t i = 0; i < numberOfHotkeyedScenes; i++)
            {
                if (Input::FrameKeyAction((eKeys)(eKeys::eKeys_F1 + i), eKeyState::eKeyState_Press))
                {
                    Scenes::SetCurrentScene((int)i);
                    break;
                }
            }

            auto it = s_EditorWindows.begin();
            while (it != s_EditorWindows.end())
            {
                it->second->Draw();
                ++it;
            }

            for (size_t i = 0; i < s_EditorWindowsQueuedForDelete.size(); i++)
            {
                s_EditorWindows.erase(s_EditorWindowsQueuedForDelete[i]->Guid());
            }
            s_EditorWindowsQueuedForDelete.clear();
        }

        void local_EndFrame()
        {
            if (!Window::IsMinimized())
            {
                const bgfx::ViewId viewIdFbo1 = 2; // #TODO Fix hard coded value
                {   // Debug drawer calls
                    DebugDrawEncoder& debugDrawer = Renderer::DebugDrawer(); // #TESTING
                    debugDrawer.begin(viewIdFbo1, true);

                    const bx::Vec3 normal = { .0f,  1.f, .0f };
                    const bx::Vec3 pos = { .0f, .0f, .0f };

                    debugDrawer.drawSphere(0.f, 0.f, 0.f, 3.f, Axis::X);

                    bx::Plane plane(bx::InitNone);
                    bx::calcPlane(plane, normal, pos);

                    debugDrawer.drawGrid(Axis::Y, pos, 50, 1.0f);

                    debugDrawer.end();
                }

                // #TODO Replace draws by moving to EditorWindowSceneView class
                // Framework::RenderView(viewIdFbo1);
            }
            Framework::EndFrame();
        }

	}

}
