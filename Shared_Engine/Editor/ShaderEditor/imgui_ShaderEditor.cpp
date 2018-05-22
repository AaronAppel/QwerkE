#include "imgui_ShaderEditor.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/ServiceLocator.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Graphics/ShaderProgram/ShaderProgram.h"
#include "../QwerkE_Framework/QwerkE_Common/Utilities/FileIO/FileUtilities.h"
#include "../QwerkE_Framework/QwerkE_Common/Libraries/imgui/imgui.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Input/InputManager.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Renderer/Renderer.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/SceneManager.h"
#include "../QwerkE_Framework/QwerkE_Framework/Components/Time.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Graphics/Gfx_Classes/FrameBufferObject.h"
#include "../../QwerkE_Framework/QwerkE_Common/Utilities/StringHelpers.h"

imgui_ShaderEditor::imgui_ShaderEditor()
{
	m_Shader = ((ResourceManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Resource_Manager))->GetShader("TestShader");
}

imgui_ShaderEditor::~imgui_ShaderEditor()
{
}

void imgui_ShaderEditor::NewFrame()
{
}

void imgui_ShaderEditor::Update()
{

}

void imgui_ShaderEditor::Draw()
{
	assert(m_Shader != nullptr); // DEBUG

	// TEST: temporary variables for testing
	const int bufferSize = 2000;
	static char vertBuffer[bufferSize] = { 0 };
	static char fragBuffer[bufferSize] = { 0 };
	static char geoBuffer[bufferSize] = { 0 };

	static GLuint currentShader = 0;

	if (currentShader != m_Shader->GetProgram())
	{
		// strcpy_s(buffer, (char*)shader->GetVertString());
		currentShader = m_Shader->GetProgram();
		// TODO: Handle null shader strings
		strcpy_s(vertBuffer, bufferSize, (const char*)m_Shader->GetVertString());
		strcpy_s(fragBuffer, bufferSize, (const char*)m_Shader->GetFragString());
		// strcpy_s(geoBuffer, bufferSize, (const char*)shader->GetGeoString());
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
	if (ImGui::CollapsingHeader("Geometry"))
	{
		if (ImGui::InputTextMultiline("", geoBuffer, bufferSize, ImVec2(400, 250)))
		{
			// buffer was changed
		}
	}

	if (ImGui::Button("Recompile Vert"))
	{
		// Good enough for now, but should remove allocation calls
		m_Shader->ReCompile1Shader(GL_VERTEX_SHADER, DeepCopyString(vertBuffer));
	}

	if (ImGui::Button("Recompile Frag"))
	{
		// Good enough for now, but should remove allocation calls
		m_Shader->ReCompile1Shader(GL_FRAGMENT_SHADER, DeepCopyString(fragBuffer));
	}

	if (ImGui::Button("Recompile Geo"))
	{
		// TODO: Handle geometry shader editing.
	}
	ImGui::End();

}