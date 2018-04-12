#include "imgui_Editor.h"
#include "../Systems/ServiceLocator.h"
#include "../Systems/Graphics/ShaderProgram/ShaderProgram.h"
#include "../../Shared_Generic/Utilities/FileIO/FileUtilities.h"
#include "../../Shared_Generic/Libraries/imgui/imgui.h"
#include "../Systems/Input/InputManager.h"
#include "../Systems/Renderer.h"
#include "../Scene/SceneManager.h"

extern float g_FrameRate;

imgui_Editor::imgui_Editor()
{
	m_SceneGraph = new SceneGraph();
	m_SceneManager = (SceneManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Scene_Manager);
	m_Input = (InputManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Input_Manager);
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
	((Renderer*)QwerkE::ServiceLocator::GetService(eEngineServices::Renderer))->DrawFont(std::to_string(g_FrameRate).c_str()); // TEST:
	m_SceneGraph->Draw();
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

void imgui_Editor::DrawShaderEditor(ShaderProgram* shader)
{
	const int bufferSize = 2000;
	static char vertBuffer[bufferSize] = { 0 };
	static char fragBuffer[bufferSize] = { 0 };
	static GLuint currentShader = 0;

	if (currentShader != shader->GetProgram())
	{
		// strcpy_s(buffer, (char*)shader->GetVertString());
		currentShader = shader->GetProgram();
		strcpy_s(vertBuffer, bufferSize, (const char*)shader->GetVertString());
		strcpy_s(fragBuffer, bufferSize, (const char*)shader->GetFragString());
	}

	ImGui::Begin("Shader Editor");
	//ImGui::ShowTestWindow();
	if (ImGui::CollapsingHeader("Vertex"))
	{
		if (ImGui::InputTextMultiline("", vertBuffer, bufferSize, ImVec2(400, 250)))
		{
			// buffer was changed
		}
	}
	if (ImGui::CollapsingHeader("Fragment"))
	{
		if (ImGui::InputTextMultiline("", fragBuffer, bufferSize, ImVec2(400, 250)))
		{
			// buffer was changed
		}
	}

	if (ImGui::Button("Recompile Vert"))
	{
		int bp = 1;
		//shader->ReCompile1Shader(GL_VERTEX_SHADER, buffer);
		long length;
		// char* newShaderString = LoadCompleteFile("../Shared_Generic/Resources/Shaders/TestShader.frag", &length);
		shader->SetShaderStringData(GL_VERTEX_SHADER, vertBuffer);
		shader->ReCompile1Shader(GL_VERTEX_SHADER, vertBuffer);
	}

	if (ImGui::Button("Recompile Frag"))
	{
		int bp = 1;
		//shader->ReCompile1Shader(GL_VERTEX_SHADER, buffer);
		long length;
		// char* newShaderString = LoadCompleteFile("../Shared_Generic/Resources/Shaders/TestShader.frag", &length);
		shader->SetShaderStringData(GL_FRAGMENT_SHADER, fragBuffer);
		shader->ReCompile1Shader(GL_FRAGMENT_SHADER, fragBuffer);
	}

	ImGui::End();
}