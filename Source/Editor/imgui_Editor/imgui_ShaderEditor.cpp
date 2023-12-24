#include "../ShaderEditor.h"

#include "../../../QwerkE_Framework/Libraries/imgui/imgui.h"

#include "../../../QwerkE_Framework/Source/Utilities/StringHelpers.h"
#include "../../../QwerkE_Framework/Source/FileSystem/FileIO/FileUtilities.h"
#include "../../../QwerkE_Framework/Source/Core/Input/Input.h"
#include "../../../QwerkE_Framework/Source/Core/Graphics/Renderer.h"
#include "../../../QwerkE_Framework/Source/Core/Scenes/Scenes.h"
#include "../../../QwerkE_Framework/Source/Core/Time/Time.h"
#include "../../../QwerkE_Framework/Source/Core/Graphics/DataTypes/FrameBufferObject.h"
#include "../../../QwerkE_Framework/Source/Core/Graphics/Shader/ShaderProgram.h"
#include "../../../QwerkE_Framework/Source/Core/Graphics/Shader/ShaderComponent.h"
#include "../../../QwerkE_Framework/Source/Core//Resources/Resources.h"

namespace QwerkE {

    ShaderEditor::ShaderEditor()
    {
        m_Shader = Resources::GetShaderProgram(null_shader);
        m_ShaderList = Resources::SeeShaderPrograms();
    }

    ShaderEditor::~ShaderEditor()
    {
    }

    void ShaderEditor::NewFrame()
    {
    }

    void ShaderEditor::Update()
    {
    }

    void ShaderEditor::Draw(bool* isOpen)
    {
        if (m_Shader == nullptr)
        {
            m_Shader = Resources::GetShaderProgram(null_shader);
        }

        // TEST: temporary variables for testing
        const int bufferSize = 2000;
        static char vertBuffer[bufferSize] = { 0 };
        static char fragBuffer[bufferSize] = { 0 };
        static char geoBuffer[bufferSize] = { 0 };

        static GLuint currentShaderProgram = 0;
        static bool showShaderList = false;

        ImGui::Begin("Shader Editor", isOpen);
        ImGui::Checkbox("ShaderList", &showShaderList);

        if (showShaderList)
        {
            for (auto p : *m_ShaderList)
            {
                if (ImGui::Button(p.second->GetName().c_str()))
                {
                    m_Shader = p.second;
                }
            }
        }

        if (currentShaderProgram != m_Shader->GetProgram())
        {
            // strcpy_s(buffer, (char*)shader->GetVertString());
            currentShaderProgram = m_Shader->GetProgram();
            // #TODO Handle null shader strings
            strcpy_s(vertBuffer, bufferSize, m_Shader->GetVertShader()->GetStringData());
            strcpy_s(fragBuffer, bufferSize, m_Shader->GetFragShader()->GetStringData());
            // strcpy_s(geoBuffer, bufferSize, (const char*)shader->GetGeoString());
        }

        // ImGui::ShowTestWindow();

        const float windowHeight = 400.0f; // #TODO Get window height dynamically or use a ratio of the current Window width
        const float magicNumber = 20.0f; // #TODO Width offset?

        if (ImGui::CollapsingHeader("Vertex"))
        {
            if (ImGui::InputTextMultiline("", vertBuffer, bufferSize, ImVec2(ImGui::GetWindowWidth() - magicNumber, windowHeight)))
            {
                // #TODO On buffer changed
            }
        }
        if (ImGui::CollapsingHeader("Fragment"))
        {
            if (ImGui::InputTextMultiline("", fragBuffer, bufferSize, ImVec2(ImGui::GetWindowWidth() - magicNumber, windowHeight)))
            {
                // #TODO On buffer changed
            }
        }
        if (ImGui::CollapsingHeader("Geometry"))
        {
            if (ImGui::InputTextMultiline("", geoBuffer, bufferSize, ImVec2(ImGui::GetWindowWidth() - magicNumber, windowHeight)))
            {
                // #TODO On buffer changed
            }
        }
        // #TODO Add recompile functionality back in. Although this should only be for certain shaders
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
            // #TODO Handle geometry shader editing.
            // m_Shader->RecompileShaderType(GL_GEOMETRY_SHADER, DeepCopyString(fragBuffer));
        }

        // Reload from file
        if (ImGui::Button("Reload Vertex"))
        {
            if (FileExists(ShadersFolderPath(m_Shader->GetVertShader()->GetName().c_str())))
            {
                // #TODO Use smart pointers to allocated object
                m_Shader->RecompileShaderType(GL_VERTEX_SHADER, LoadCompleteFile(ShadersFolderPath(m_Shader->GetVertShader()->GetName().c_str()), nullptr)); // RAM passed to shader
            }
        }
        if (ImGui::Button("Reload Fragment"))
        {
            if (FileExists(ShadersFolderPath(m_Shader->GetFragShader()->GetName().c_str())))
            {
                // #TODO Use smart pointers to allocated object
                m_Shader->RecompileShaderType(GL_FRAGMENT_SHADER, LoadCompleteFile(ShadersFolderPath(m_Shader->GetFragShader()->GetName().c_str()), nullptr));
            }
        }
        if (ImGui::Button("Reload Geometry"))
        {
            // #TODO Handle geometry shader editing.
        }

        ImGui::End();
    }

}
