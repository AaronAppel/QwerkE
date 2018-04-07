#include "Editor.h"
#include "../Systems/Graphics/ShaderProgram/ShaderProgram.h"
#include "../../Shared_Generic/Utilities/FileIO/FileUtilities.h"
#include "../../Shared_Generic/Libraries/imgui/imgui.h"

#include <cstring>

Editor::Editor()
{
	m_SceneGraph = new SceneGraph();
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
}

int repl_callback(ImGuiTextEditCallbackData *data)
{
    if (data->EventChar != 0)
    {
        char re[2] = { '\0' };
        re[0] = (char)data->EventChar;
        data->InsertChars(data->CursorPos, re);

    }

    //log("callback called\n");
    return 0;
}

void Editor::DrawShaderEditor(ShaderProgram* shader)
{
    const int bufferSize = 10000;
    static char buffer[bufferSize];
    static GLuint currentShader = 0;

    // TEST: Text field to recompile shader
    ImGui::Begin("Shader Editor");

    if (currentShader != shader->GetProgram())
    {
        // strcpy_s(buffer, (char*)shader->GetVertString());
        strcpy_s(buffer, (char*)shader->GetFragString());
    }

    if (ImGui::InputTextMultiline("Shader Body", buffer, bufferSize, ImVec2(300, 200), ImGuiInputTextFlags_CallbackAlways, repl_callback))
    {
        int bp = 1;
    }

    if (ImGui::Button("Recompile"))
    {
        int bp = 1;
        //shader->ReCompile1Shader(GL_VERTEX_SHADER, buffer);
        long length;
        char* newShaderString = LoadCompleteFile("../Shared_Generic/Resources/Shaders/TestShader.frag", &length);
        shader->SetShaderStringData(GL_FRAGMENT_SHADER, newShaderString);
        shader->ReCompile1Shader(GL_FRAGMENT_SHADER, newShaderString);
    }

    ImGui::End();
}