#include "Editor.h"
#include "../Systems/Graphics/ShaderProgram/ShaderProgram.h"
#include "../../Shared_Generic/Utilities/FileIO/FileUtilities.h"
#include "../../Shared_Generic/Libraries/imgui/imgui.h"
#include "../Systems/ServiceLocator.h"
#include "../Systems/Input/InputManager.h"
#include "../Scene/SceneManager.h"

#include <cstring>

Editor::Editor()
{
	m_SceneGraph = new SceneGraph();
	m_SceneManager = (SceneManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Scene_Manager);
	m_Input = (InputManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Input_Manager);
}

Editor::~Editor()
{
	delete m_SceneGraph;
}

void Editor::NewFrame()
{
}

void Editor::Update()
{

}

void Editor::Draw()
{
	m_SceneGraph->Draw();
	DrawSceneList();
}

void Editor::DrawSceneList()
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

int repl_callback(ImGuiTextEditCallbackData *data)
{
	if (data->EventKey == 21) // left click
	{
		int bp = 1;
	}
	if (data->EventChar != 0)
	{
		*(char*)data->UserData = data->EventChar;
	}
	if (data->EventKey != 21)
	{
		*(char*)data->UserData = 65;
	}

	if (false)
	{
		/*
		char re[2] = { '\0' };
		re[0] = (char)data->EventChar;
		data->InsertChars(data->CursorPos, re);
		*/
	}
	return 0;
}

void Editor::DrawShaderEditor(ShaderProgram* shader)
{
    const int bufferSize = 1000;
	static char buffer[bufferSize] = {0};
    static GLuint currentShader = 0;

    if (currentShader != shader->GetProgram())
    {
        // strcpy_s(buffer, (char*)shader->GetVertString());
		currentShader = shader->GetProgram();
        strcpy_s(buffer, bufferSize, (const char*)shader->GetFragString());
    }

	ImGui::Begin("Shader Editor");
    if (ImGui::InputTextMultiline("", buffer, bufferSize, ImVec2(400, 250)))
    {
		// buffer was changed
    }

    if (ImGui::Button("Recompile"))
    {
        int bp = 1;
        //shader->ReCompile1Shader(GL_VERTEX_SHADER, buffer);
        long length;
        // char* newShaderString = LoadCompleteFile("../Shared_Generic/Resources/Shaders/TestShader.frag", &length);
        shader->SetShaderStringData(GL_FRAGMENT_SHADER, buffer);
        shader->ReCompile1Shader(GL_FRAGMENT_SHADER, buffer);
    }

    ImGui::End();
}