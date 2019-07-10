#include "imgui_ShaderEditor.h"
#include "../../../QwerkE_Framework/QwerkE_Common/Utilities/FileIO/FileUtilities.h"
#include "../../../QwerkE_Framework/QwerkE_Common/Libraries/imgui/imgui.h"
#include "../../../QwerkE_Framework/Systems/ServiceLocator.h"
#include "../../../QwerkE_Framework/Systems/Input/InputManager.h"
#include "../../../QwerkE_Framework/Systems/Renderer/Renderer.h"
#include "../../../QwerkE_Framework/Systems/SceneManager.h"
#include "../../../QwerkE_Framework/Modules/Time.h"
#include "../../../QwerkE_Framework/Graphics/FrameBufferObject.h"
#include "../../../QwerkE_Framework/QwerkE_Common/Utilities/StringHelpers.h"
#include "../../../QwerkE_Framework/Graphics/Shader/ShaderProgram.h"
#include "../../../QwerkE_Framework/Graphics/Shader/ShaderComponent.h"

imgui_ShaderEditor::imgui_ShaderEditor()
{
	m_Shader = ((ResourceManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Resource_Manager))->GetShaderProgram(null_shader);
	m_ShaderList = ((ResourceManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Resource_Manager))->SeeShaderPrograms();
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

void imgui_ShaderEditor::Draw(bool *isOpen)
{
	if (m_Shader == nullptr)
	{
        m_Shader = ((ResourceManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Resource_Manager))->GetShaderProgram(null_shader);
	}

	// TEST: temporary variables for testing
	const int bufferSize = 2000;
	static char vertBuffer[bufferSize] = { 0 };
	static char fragBuffer[bufferSize] = { 0 };
	static char geoBuffer[bufferSize] = { 0 };

	static GLuint currentShader = 0;

    static bool showShaderList = false;

    ImGui::Begin("Shader Editor", isOpen);
	ImGui::Checkbox("ShaderList", &showShaderList);
	if(showShaderList)
	for (auto p : *m_ShaderList)
	{
		if (ImGui::Button(p.second->GetName().c_str()))
		{
			m_Shader = p.second;
		}
	}

	if (currentShader != m_Shader->GetProgram())
	{
		// strcpy_s(buffer, (char*)shader->GetVertString());
		currentShader = m_Shader->GetProgram();
		// TODO: Handle null shader strings
		strcpy_s(vertBuffer, bufferSize, m_Shader->GetVertShader()->GetStringData());
		strcpy_s(fragBuffer, bufferSize, m_Shader->GetFragShader()->GetStringData());
		// strcpy_s(geoBuffer, bufferSize, (const char*)shader->GetGeoString());
	}

	//ImGui::ShowTestWindow();
    int windowHeight = 400;
	if (ImGui::CollapsingHeader("Vertex"))
	{
		if (ImGui::InputTextMultiline("", vertBuffer, bufferSize, ImVec2(ImGui::GetWindowWidth() - 20, windowHeight)))
		{
			// buffer was changed
		}
	}
	if (ImGui::CollapsingHeader("Fragment"))
	{
		if (ImGui::InputTextMultiline("", fragBuffer, bufferSize, ImVec2(ImGui::GetWindowWidth() - 20, windowHeight)))
		{
			// buffer was changed
		}
	}
	if (ImGui::CollapsingHeader("Geometry"))
	{
		if (ImGui::InputTextMultiline("", geoBuffer, bufferSize, ImVec2(ImGui::GetWindowWidth() - 20, windowHeight)))
		{
			// buffer was changed
		}
	}
	// TODO: Add recompile functionality back in. Although this should only be for certain shaders
	// like "modifiable" tagged ones. Or something...
	if (ImGui::Button("Recompile Vertex"))
	{
		// Good enough for now, but should remove allocation calls
        m_Shader->RecompileShaderType(GL_VERTEX_SHADER, DeepCopyString(fragBuffer));
	}

	// like "modifiable" tagged ones. Or something...
	if (ImGui::Button("Recompile Fragment"))
	{
		// Good enough for now, but should remove allocation calls
		m_Shader->RecompileShaderType(GL_FRAGMENT_SHADER, DeepCopyString(fragBuffer));
	}

	if (ImGui::Button("Recompile Geometry"))
	{
        // TODO: Handle geometry shader editing.
        // m_Shader->RecompileShaderType(GL_GEOMETRY_SHADER, DeepCopyString(fragBuffer));
	}

	// Reload from file
	if (ImGui::Button("Reload Vertex"))
	{
		if (FileExists(ShaderFolderPath(m_Shader->GetVertShader()->GetName().c_str())))
			m_Shader->RecompileShaderType(GL_VERTEX_SHADER, LoadCompleteFile(ShaderFolderPath(m_Shader->GetVertShader()->GetName().c_str()))); // RAM passed to shader
	}
	if (ImGui::Button("Reload Fragment"))
	{
		if (FileExists(ShaderFolderPath(m_Shader->GetFragShader()->GetName().c_str())))
			m_Shader->RecompileShaderType(GL_FRAGMENT_SHADER, LoadCompleteFile(ShaderFolderPath(m_Shader->GetFragShader()->GetName().c_str()))); // RAM passed to shader
	}
	if (ImGui::Button("Reload Geometry"))
	{
		// TODO: Handle geometry shader editing.
	}
	ImGui::End();

}