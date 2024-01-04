#include "QE_ShaderEditor.h"

#include "imgui/imgui.h"

#include "QC_StringHelpers.h"

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

    void ShaderEditor::Draw(bool* isOpen)
    {
        if (m_Shader == nullptr)
        {
            m_Shader = Resources::GetShaderProgram(null_shader);
        }

        ImGui::Begin("Shader Editor", isOpen);
        ImGui::Checkbox("ShaderList", &m_ShowShaderList);

        if (m_ShowShaderList)
        {
            for (auto p : *m_ShaderList)
            {
                if (ImGui::Button(p.second->GetName().c_str()))
                {
                    m_Shader = p.second;
                }
            }
        }

        if (m_CurrentShaderProgram != (int)m_Shader->GetProgram())
        {
            // strcpy_s(buffer, (char*)shader->GetVertString());
            m_CurrentShaderProgram = (int)m_Shader->GetProgram();
            // #TODO Handle null shader strings
            strcpy_s(m_VertBuffer, m_BufferSize, m_Shader->GetVertShader()->GetStringData());
            strcpy_s(m_FragBuffer, m_BufferSize, m_Shader->GetFragShader()->GetStringData());
            // strcpy_s(m_GeoBuffer, bufferSize, (const char*)shader->GetGeoString());
        }

        const float windowHeight = 400.0f; // #TODO Get window height dynamically or use a ratio of the current Window width
        const float magicNumber = 20.0f; // #TODO Is this value a width offset?

        if (ImGui::CollapsingHeader("Vertex"))
        {
            if (ImGui::InputTextMultiline("", m_VertBuffer, m_BufferSize, ImVec2(ImGui::GetWindowWidth() - magicNumber, windowHeight)))
            {
                // #TODO On buffer changed
            }
        }
        if (ImGui::CollapsingHeader("Fragment"))
        {
            if (ImGui::InputTextMultiline("", m_FragBuffer, m_BufferSize, ImVec2(ImGui::GetWindowWidth() - magicNumber, windowHeight)))
            {
                // #TODO On buffer changed
            }
        }
        if (ImGui::CollapsingHeader("Geometry"))
        {
            if (ImGui::InputTextMultiline("", m_GeoBuffer, m_BufferSize, ImVec2(ImGui::GetWindowWidth() - magicNumber, windowHeight)))
            {
                // #TODO On buffer changed
            }
        }
        // #TODO Add recompile functionality back in. Although this should only be for certain shaders
        // like "modifiable" tagged ones. Or something...
        if (ImGui::Button("Recompile Vertex"))
        {
            // Good enough for now, but should remove allocation calls
            m_Shader->RecompileShaderType(GL_VERTEX_SHADER, DeepCopyString(m_VertBuffer));
        }

        // like "modifiable" tagged ones. Or something...
        if (ImGui::Button("Recompile Fragment"))
        {
            // Good enough for now, but should remove allocation calls
            m_Shader->RecompileShaderType(GL_FRAGMENT_SHADER, DeepCopyString(m_FragBuffer));
        }

        if (ImGui::Button("Recompile Geometry"))
        {
            // #TODO Handle geometry shader editing.
            // m_Shader->RecompileShaderType(GL_GEOMETRY_SHADER, DeepCopyString(m_GeoBuffer));
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
