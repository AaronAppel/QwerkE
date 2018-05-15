#include "../QwerkE_Framework/QwerkE_Framework/Systems/ServiceLocator.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Graphics/ShaderProgram/ShaderProgram.h"
#include "../QwerkE_Framework/QwerkE_Common/Utilities/FileIO/FileUtilities.h"
#include "../QwerkE_Framework/QwerkE_Common/Libraries/imgui/imgui.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Input/InputManager.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Renderer.h"
#include "../QwerkE_Framework/QwerkE_Framework/Scene/SceneManager.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Time.h"
#include "EntityEditor/EntityEditor.h"
#include "imgui_Editor.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Graphics/FBO/FrameBufferObject.h"

FrameBufferObject* g_FBO = new FrameBufferObject();

imgui_Editor::imgui_Editor()
{
	m_SceneManager = (SceneManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Scene_Manager);
	m_Input = (InputManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Input_Manager);
	g_FBO->Init();
}

imgui_Editor::~imgui_Editor()
{
}

void imgui_Editor::NewFrame()
{
}

void imgui_Editor::Update()
{

}

void imgui_Editor::Draw()
{
    // windows
    static bool shaderEditor = false;

	// scene view
	// TEMP: Render scene to texture
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	g_FBO->Bind();
	m_SceneManager->Draw();
	g_FBO->UnBind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// TEMP: End

	ImGui::Begin("Scene Window - Framework.cpp");
	ImGui::Image(ImTextureID(g_FBO->GetTextureID()), ImVec2(1600 / 3, 900 / 3), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();

	// menu
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			static int index = 0;
			static const int size = 5;
			const char* d[size] = { "one", "two", "three", "four", "five"};
			ImGui::ListBox("", &index, d, size, 3);
			ImGui::EndMenu();
		}

        if (ImGui::BeginMenu("Tools"))
        {
            const int size = 1;
            static const char* toolsList[size] = {"Shader Editor"};
            static bool* toolsStates[size] = { &shaderEditor };

            for (int i = 0; i < size; i++)
            {
                ImGui::Checkbox(toolsList[i], toolsStates[i]);
            }
            ImGui::EndMenu();
        }

		// FPS display
		static bool showFPS = true;
		if (ImGui::Button("FPS"))
			showFPS = !showFPS;
		ImGui::SameLine();
		if (showFPS) ImGui::Text("%4.2f", QwerkE::Time::GetFrameRate());


		ImGui::EndMainMenuBar();
	}

    if (shaderEditor)
    {
        DrawShaderEditor((ShaderProgram*)((ResourceManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Resource_Manager))->GetShader("TestShader"));
    }

	m_SceneGraph->Draw();
    m_EntityEditor->Draw();
	DrawSceneList();
}

void imgui_Editor::DrawSceneList()
{
	ImGui::Begin("Scene List");

	// TODO: Think of adding multi window support for viewing more than 1
	// enabled scene at a time. Going to have to look at how input would
	// work for that.
	m_SceneManager; // size
	for (int i = 0; i < 1; i++)
	{
		if (ImGui::Button("TestScene") || m_Input->GetIsKeyDown(eKeys::eKeys_1))
		{
			m_SceneManager->SetCurrentScene(eSceneTypes::Scene_TestScene);
		}
		if (ImGui::Button("GameScene") || m_Input->GetIsKeyDown(eKeys::eKeys_2))
		{
			// m_SceneManager->SetCurrentScene(eSceneTypes::Scene_GameScene);
		}
	}

	ImGui::End();
}