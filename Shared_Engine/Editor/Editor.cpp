#include "Editor.h"
#include "../Systems/Graphics/ShaderProgram/ShaderProgram.h"
#include "../../Shared_Generic/Utilities/FileIO/FileUtilities.h"
#include "../../Shared_Generic/Libraries/imgui/imgui.h"
#include "../Systems/ServiceLocator.h"
#include "../Systems/Input/InputManager.h"
#include "../Systems/Renderer.h"
#include "../Scene/SceneManager.h"
#include "EntityEditor.h"

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