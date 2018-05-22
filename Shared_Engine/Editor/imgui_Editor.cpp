#include "../QwerkE_Framework/QwerkE_Framework/Systems/ServiceLocator.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Graphics/ShaderProgram/ShaderProgram.h"
#include "../QwerkE_Framework/QwerkE_Common/Utilities/FileIO/FileUtilities.h"
#include "../QwerkE_Framework/QwerkE_Common/Libraries/imgui/imgui.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Input/InputManager.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Audio/AudioManager.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Renderer/Renderer.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/SceneManager.h"
#include "../QwerkE_Framework/QwerkE_Framework/Components/Time.h"
#include "EntityEditor/EntityEditor.h"
#include "imgui_Editor.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Graphics/Gfx_Classes/FrameBufferObject.h"
#include "ResourceViewer/ResourceViewer.h"
#include "ShaderEditor/imgui_ShaderEditor.h"
#include "SceneViewer/imgui_SceneViewer.h"

imgui_Editor::imgui_Editor()
{
	m_SceneManager = (SceneManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Scene_Manager);
	m_Input = (InputManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Input_Manager);
	m_EntityEditor = new EntityEditor(this);

	m_ResourceViewer = new ResourceViewer();
	m_ShaderEditor = new imgui_ShaderEditor();
	m_SceneViewer = new imgui_SceneViewer();
}

imgui_Editor::~imgui_Editor()
{
	delete m_EntityEditor;
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
	// imgui reference
	if (m_ShowingExampleWindow)
	{
		ImGui::ShowDemoWindow();
	}

	// menu
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			static int index = 0;
			static const int size = 5;
			const char* d[size] = { "ExampleWindow", "two", "three", "four", "five"};
			if (ImGui::ListBox("", &index, d, size, 3))
			{
				if (index == 0) m_ShowingExampleWindow = !m_ShowingExampleWindow;
			}
			ImGui::EndMenu();
		}

        if (ImGui::BeginMenu("Tools"))
        {
            const int size = 1;
            static const char* toolsList[size] = {"Shader Editor"};
            static bool* toolsStates[size] = { &m_ShowingShaderEditor };

            for (int i = 0; i < size; i++)
            {
                ImGui::Checkbox(toolsList[i], toolsStates[i]);
            }
            ImGui::EndMenu();
        }

		if (ImGui::Checkbox("GUI", &m_ShowingGUI));

		// FPS display
		static bool showFPS = true;
		if (ImGui::Button("FPS"))
			showFPS = !showFPS;
		ImGui::SameLine();
		if (showFPS) ImGui::Text("%4.2f", QwerkE::Time::GetFrameRate());

		ImGui::EndMainMenuBar();
	}

	if (m_ShowingGUI)
	{
		if (m_ShowingShaderEditor)
		{
			m_ShaderEditor->Draw();
		}
		if (ImGui::Begin("Audio"))
		{
			if (ImGui::Button("Play Sound"))
			{
				((AudioManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Audio_Manager))->PlaySound("bounce.wav");
			}
			ImGui::End();
		}
		m_ResourceViewer->Draw();
		m_SceneViewer->Draw();
		m_SceneGraph->Draw();
		m_EntityEditor->Draw();
	}
	else
	{
		m_SceneManager->Draw();
	}
}