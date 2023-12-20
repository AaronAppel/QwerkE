#include "imgui_Editor.h"

#include "../QwerkE_Framework/Libraries/imgui/imgui.h"

#include "../QwerkE_Framework/Source/FileSystem/FileIO/FileUtilities.h"
#include "../QwerkE_Framework/Source/Core/Input/Input.h"
#include "../QwerkE_Framework/Source/Core/Audio/Audio.h"
#include "../QwerkE_Framework/Source/Core/Graphics/Renderer.h"
#include "../QwerkE_Framework/Source/Core/Scenes/Scenes.h"
#include "../QwerkE_Framework/Source/Core/Time/Time.h"
#include "../QwerkE_Framework/Source/Core/Graphics/DataTypes/FrameBufferObject.h"
#include "../QwerkE_Framework/Source/Debug/Profiler/Profiler.h"
#include "../QwerkE_Framework/Source/Core/Network/Network.h"

#include "../EntityEditor.h"
#include "../ResourceViewer.h"
#include "../ShaderEditor.h"
#include "../SceneViewer.h"
#include "../SceneGraph.h"
#include "../EditComponent.h"

namespace QwerkE {

    imgui_Editor::imgui_Editor()
    {
        m_EntityEditor = new EntityEditor();
        m_SceneGraph = new SceneGraph(m_EntityEditor);
        m_ResourceViewer = new ResourceViewer();
        m_ShaderEditor = new ShaderEditor();
        m_SceneViewer = new SceneViewer();
    }

    imgui_Editor::~imgui_Editor()
    {
        delete m_EntityEditor;
        delete m_SceneGraph;
        delete m_ResourceViewer;
        delete m_ShaderEditor;
        delete m_SceneViewer;
    }

    void imgui_Editor::NewFrame()
    {
    }

    void imgui_Editor::Update()
    {
    }

    void imgui_Editor::Draw()
    {
        PROFILE_SCOPE("Editor Render");
        if (m_ShowingEditorGUI == false)
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
                        if (index == 0) m_ShowingExampleWindow = !m_ShowingExampleWindow;
                    }
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Tools"))
                {
                    const int size = 1;
                    static const char* toolsList[size] = { "Shader Editor" };
                    static bool* toolsStates[size] = { &m_ShowingShaderEditor };

                    for (int i = 0; i < size; i++)
                    {
                        if (ImGui::Checkbox(toolsList[i], toolsStates[i]))
                        {
                            m_ShowingShaderEditor = *toolsStates[i];
                        }
                    }
                    ImGui::EndMenu();
                }

                if (ImGui::Checkbox("GUI", &m_ShowingEditorGUI)) {}

                static bool showFPS = true;
                if (ImGui::Button("FPS"))
                {
                    showFPS = !showFPS;
                }

                ImGui::SameLine();
                if (showFPS)
                {
                    ImGui::Text("%4.2f", 1.0 / Time::Delta());
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

                ImGui::EndMainMenuBar();
            }

            // Scenes::GetCurrentScene()->Draw();
            return;
        }

        RenderDockingContext();

        if (m_ShowingExampleWindow)
        {
            ImGui::ShowDemoWindow();
        }

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("Menu"))
            {
                static int index = 0;
                static const int size = 5;
                const char* d[size] = { "ExampleWindow", "two", "three", "four", "five" };
                if (ImGui::ListBox("", &index, d, size, 3))
                {
                    if (index == 0) m_ShowingExampleWindow = !m_ShowingExampleWindow;
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Tools"))
            {
                const int size = 1;
                static const char* toolsList[size] = { "Shader Editor" };
                static bool* toolsStates[size] = { &m_ShowingShaderEditor };

                for (int i = 0; i < size; i++)
                {
                    if (ImGui::Checkbox(toolsList[i], toolsStates[i]))
                    {
                        m_ShowingShaderEditor = *toolsStates[i];
                    }
                }
                ImGui::EndMenu();
            }

            if (ImGui::Checkbox("GUI", &m_ShowingEditorGUI)) {}

            static bool showFPS = true;
            if (ImGui::Button("FPS"))
                showFPS = !showFPS;
            ImGui::SameLine();
            if (showFPS) ImGui::Text("%4.2f", 1.0 / Time::Delta());

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

            ImGui::EndMainMenuBar();
        }

        if (m_ShowingShaderEditor)
            m_ShaderEditor->Draw(&m_ShowingShaderEditor);

        m_ResourceViewer->Draw();
        m_SceneViewer->Draw();
        m_SceneGraph->Draw();
        m_EntityEditor->Draw();
    }

    void imgui_Editor::RenderDockingContext()
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

}
