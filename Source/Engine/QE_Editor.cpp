#include "QE_Editor.h"

#include "Libraries/imgui/QC_imgui.h"
#include "Libraries/enum/QC_enum.h"

#include "QF_Debug.h"
#include "QF_Defines.h"
#include "QF_FileSystem.h"
#include "QF_FileUtilities.h"
#include "QF_Profile.h"
#include "QF_Network.h"
#include "QF_Time.h"
#include "QF_Settings.h"
#include "QF_Window.h"

#include "QE_EditorInspector.h"
#include "QE_EntityEditor.h"
#include "QE_ResourceViewer.h"
#include "QE_ShaderEditor.h"
#include "QE_SceneGraph.h"
#include "QE_SceneViewer.h"

namespace QwerkE {

    namespace Editor {

        static EntityEditor* s_EntityEditor = nullptr;
        static SceneGraph* s_SceneGraph = nullptr;
        static ShaderEditor* s_ShaderEditor = nullptr;
        static ResourceViewer* s_ResourceViewer = nullptr;
        static SceneViewer* s_SceneViewer = nullptr;

        static EngineSettings* s_EngineSettings = nullptr;

        QC_ENUM(eSettingsOptions, int, Null, Engine, Project, User);
        static eSettingsOptions s_SettingsEditorOption = eSettingsOptions::Null;

        static bool* s_showingFPS = nullptr; // #TODO Review individual value pointers

#ifdef dearimgui
        static bool s_ShowingImGuiExampleWindow = false;

        static void priv_DrawMainMenuBar();
        static void RenderDockingContext();

#endif

        void Editor::Initialize()
        {
            s_EntityEditor = new EntityEditor();
            s_SceneGraph = new SceneGraph(s_EntityEditor);
            s_ResourceViewer = new ResourceViewer();
            s_ShaderEditor = new ShaderEditor();
            s_SceneViewer = new SceneViewer();

            s_EngineSettings = &Settings::GetEngineSettings();
            ASSERT(s_EngineSettings, "Null engine settings!");
            static bool* s_showingFPS = &s_EngineSettings->showingFPS;
        }

        void Editor::Shutdown()
        {
            delete s_EntityEditor;
            delete s_SceneGraph;
            delete s_ResourceViewer;
            delete s_ShaderEditor;
            delete s_SceneViewer;
        }

        void Editor::Draw()
        {
            PROFILE_SCOPE("Editor Render");

            if (Window::IsMinimized())
                return;

#ifdef dearimgui

            priv_DrawMainMenuBar();
            RenderDockingContext();

            if (s_ShowingImGuiExampleWindow)
            {
                ImGui::ShowDemoWindow(&s_ShowingImGuiExampleWindow);
            }

            if (s_EngineSettings->showingShaderEditor)
            {
                s_ShaderEditor->Draw(s_EngineSettings->showingShaderEditor);
            }

            const bool before = s_EngineSettings->showingSettingsEditor;

            if (s_EngineSettings->showingSettingsEditor &&
                ImGui::Begin("Settings Inspector", &s_EngineSettings->showingSettingsEditor))
            {
                if (ImGui::IsWindowDocked() && !s_EngineSettings->showingSettingsEditor)
                {
                    int bp = 0;
                }

                for (size_t i = 1; i < eSettingsOptions::_size_constant; i++)
                {
                    if (i > 1) ImGui::SameLine();

                    if (ImGui::Button(ENUM_TO_STR(eSettingsOptions::_from_index(i))))
                    {
                        s_SettingsEditorOption = eSettingsOptions::_from_index(i);
                    }
                }

                if (s_SettingsEditorOption != (eSettingsOptions)eSettingsOptions::Null)
                {
                    std::string buffer = "";
                    buffer.reserve(200);
                    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.3f);

                    switch (s_SettingsEditorOption)
                    {
                    case eSettingsOptions::Engine:
                        Inspector::InspectFieldRecursive(Mirror::InfoForType<EngineSettings>(), &Settings::GetEngineSettings(), buffer);
                        break;

                    case eSettingsOptions::Project:
                        Inspector::InspectFieldRecursive(Mirror::InfoForType<ProjectSettings>(), &Settings::GetProjectSettings(), buffer);
                        break;

                    case eSettingsOptions::User:
                        Inspector::InspectFieldRecursive(Mirror::InfoForType<UserSettings>(), &Settings::GetUserSettings(), buffer);
                        break;
                    }

                    ImGui::PopItemWidth();
                }

                ImGui::End();
            }
            else
            {
                if (before != s_EngineSettings->showingSettingsEditor)
                {
                    ImGui::End();
                }
            }

            if (s_EngineSettings->showingSchematicsEditor && ImGui::Begin("Schematics Inspector", &s_EngineSettings->showingSchematicsEditor))
            {
                // #TODO Cache result to avoid constant directory info fetching
                auto dirFileNamesUPtr = ReadDir(SchematicsFolderPath(""));
                auto fileNames = dirFileNamesUPtr.get();

                for (size_t i = 0; i < fileNames->size(); i++)
                {
                    ImGui::Button(fileNames->at(i).c_str());
                }

                ImGui::End();
            }

            s_ResourceViewer->Draw();
            s_SceneViewer->Draw();
            s_SceneGraph->Draw();
            s_EntityEditor->Draw();
#endif
        }

        EntityEditor* Editor::GetEntityEditor()
        {
            return s_EntityEditor;
        }

#ifdef dearimgui
        void priv_DrawMainMenuBar()
        {
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

                if (ImGui::BeginMenu("Tools"))
                {
                    const int size = 1;
                    static const char* toolsList[size] = { "Shader Editor" };
                    static bool* toolsStates[size] = { &s_EngineSettings->showingShaderEditor };

                    for (int i = 0; i < size; i++)
                    {
                        if (ImGui::Checkbox(toolsList[i], toolsStates[i]))
                        {
                            s_EngineSettings->showingShaderEditor = *toolsStates[i];
                        }
                    }
                    ImGui::EndMenu();
                }

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

                if (ImGui::BeginMenu("Testing"))
                {
                    static bool clientServerEnabled = false;
                    if (ImGui::Checkbox("Client/Server", &clientServerEnabled))
                    {
                        if (clientServerEnabled)
                        {
                            Network::Initialize();
                        }
                        else
                        {
                            Network::TearDown();
                        }
                    }

                    if (clientServerEnabled)
                    {
                        Network::TestUpdate();
                    }

                    ImGui::EndMenu();
                }

                ImGui::SameLine(ImGui::GetWindowWidth() - 36); // 4 characters * 9 pixels each
                if (s_showingFPS)
                {
                    char a[] = { '0', '0', '0', '\0' };

                    const float& deltaTime = Time::FrameDelta();
                    if (deltaTime == .0f)
                    {
                        snprintf(a, sizeof(a) / sizeof(char), "%1.1f", deltaTime);
                    }
                    else
                    {
                        snprintf(a, sizeof(a) / sizeof(char), "%1.1f", 1.f / deltaTime);
                    }

                    ImGui::PushItemWidth(200.f);
                    if (ImGui::Button(a))
                    {
                    }
                    ImGui::PopItemWidth();

                    if (ImGui::IsItemClicked())
                    {
                        *s_showingFPS = !s_showingFPS;
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
        }

        void RenderDockingContext()
        {
            // Copied from imgui docking example
            static bool opt_fullscreen_persistant = true;
            bool opt_fullscreen = opt_fullscreen_persistant;

            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
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
                        dockspaceOpen = false;
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }

            ImGui::End();
        }
#endif

    }

}
