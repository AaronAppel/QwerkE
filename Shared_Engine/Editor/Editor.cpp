#include "Editor.h"
#include "../QwerkE_Framework/QwerkE_Common/Utilities/StringHelpers.h"
#include "../QwerkE_Framework/QwerkE_Common/Utilities/FileIO/FileUtilities.h"
#include "../QwerkE_Framework/QwerkE_Common/Libraries/imgui/imgui.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Graphics/ShaderProgram/ShaderProgram.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/ServiceLocator.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Input/InputManager.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Renderer.h"
#include "../QwerkE_Framework/QwerkE_Framework/Scene/SceneManager.h"
#include "EntityEditor/EntityEditor.h"

Editor::Editor()
{
	m_SceneGraph = new SceneGraph(this);
    m_EntityEditor = new EntityEditor(this);
	m_SceneManager = (SceneManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Scene_Manager);
	m_Input = (InputManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Input_Manager);
}

Editor::~Editor()
{
	delete m_SceneGraph;
    delete m_EntityEditor;
}

EntityEditor* Editor::GetEntityEditor()
{
    return m_EntityEditor;
}

void Editor::DrawShaderEditor(ShaderProgram* shader)
{
	assert(shader != nullptr); // DEBUG

	// TEST: temporary variables for testing
    const int bufferSize = 2000;
    static char vertBuffer[bufferSize] = { 0 };
    static char fragBuffer[bufferSize] = { 0 };
	static char geoBuffer[bufferSize] = { 0 };

    static GLuint currentShader = 0;

    if (currentShader != shader->GetProgram())
    {
        // strcpy_s(buffer, (char*)shader->GetVertString());
        currentShader = shader->GetProgram();
		// TODO: Handle null shader strings
        strcpy_s(vertBuffer, bufferSize, (const char*)shader->GetVertString());
        strcpy_s(fragBuffer, bufferSize, (const char*)shader->GetFragString());
		// strcpy_s(geoBuffer, bufferSize, (const char*)shader->GetGeoString());
    }

    // TODO: boolean to open/close window
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
	if (ImGui::CollapsingHeader("Geometry"))
	{
		if (ImGui::InputTextMultiline("", geoBuffer, bufferSize, ImVec2(400, 250)))
		{
			// buffer was changed
		}
	}

    if (ImGui::Button("Recompile Vert"))
    {
		shader->ReCompile1Shader(GL_VERTEX_SHADER, DeepCopyString(vertBuffer)); // Good enough for now, but should remove allocation calls

		// can also load shader from file
		// long length;
		// char* newShaderString = LoadCompleteFile("../Shared_Generic/Resources/Shaders/TestShader.vert", &length);
    }

    if (ImGui::Button("Recompile Frag"))
    {
		shader->ReCompile1Shader(GL_FRAGMENT_SHADER, DeepCopyString(fragBuffer)); // Good enough for now, but should remove allocation calls

		// can also load shader from file
		// long length;
		// char* newShaderString = LoadCompleteFile("../Shared_Generic/Resources/Shaders/TestShader.frag", &length);
    }

	if (ImGui::Button("Recompile Geo"))
	{
		// TODO: Handle geometry shader editing.
	}
    ImGui::End();
}