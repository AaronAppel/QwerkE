#include "QE_ShaderEditor.h"

#include "imgui/imgui.h"

#include "QC_StringHelpers.h"

#include "QF_FileUtilities.h"
#include "QF_Input.h"
#include "QF_Renderer.h"
#include "QF_Scenes.h"
#include "QF_Time.h"
#include "QF_FrameBufferObject.h"
#include "QF_ShaderProgram.h"
#include "QF_ShaderComponent.h"
#include "QF_Resources.h"

namespace QwerkE {

    const char* s_ShadersComboText = "Shader Schematic";
    const int s_CharacterPixelSize = 10;

    void ShaderEditor::Draw(bool* isOpen)
    {
        if (m_CurrentShader == nullptr)
        {
            m_CurrentShader = Resources::GetShaderProgram(null_shader);
            if (m_CurrentShader)
            {
                m_CurrentShaderComponent = m_CurrentShader->GetVertShader();
                UpdateTextBuffer();
            }
        }

        ImGui::Begin("Shader Editor", isOpen);

        auto shaders = Resources::SeeShaderPrograms();
        std::vector<const char*> shaderNames;
        shaderNames.reserve(3);

        static int currentShaderIndex = 0;
        int sceneFileNameWidth = 0;
        int counter = 0;
        for (auto& it : shaders)
        {
            const char* shaderName = it.second->GetName().c_str();
            if (currentShaderIndex == counter)
            {
                sceneFileNameWidth = strlen(shaderName) * s_CharacterPixelSize;
            }
            shaderNames.push_back(shaderName);
            counter++;
        }

        ImGui::PushItemWidth((float)sceneFileNameWidth);
        // ImGui::SameLine(ImGui::GetWindowWidth() - sceneFileNameWidth - (strlen(s_ShadersComboText) * s_CharacterPixelSize));
        if (ImGui::Combo(s_ShadersComboText, &currentShaderIndex, shaderNames.data(), shaders.size()))
        {
            m_CurrentShader = Resources::GetShaderProgram(shaderNames[currentShaderIndex]);
            if (m_CurrentShader)
            {
                m_CurrentShaderComponent = m_CurrentShader->GetVertShader();
                UpdateTextBuffer();
            }
        }
        ImGui::PopItemWidth();

        if (!m_CurrentShader)
            return;

        const char* shaderTypes[] = { "Vertex", "Fragment", "Geometry"};
        const int size = sizeof(shaderTypes) / sizeof(char*);
        for (size_t i = 0; i < size; i++)
        {
            if (i > 0) ImGui::SameLine();

            bool popStyleColor = false;
            if (i == m_CurrentShaderTypeIndex)
            {
                popStyleColor = true;

                if (strcmp("", m_TextBuffer) == 0)
                {
                    ImGui::PushStyleColor(ImGuiCol_Button,          (ImVec4)ImColor::HSV(1.f, 0.6f, 0.6f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)ImColor::HSV(1.f, 0.6f, 0.6f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)ImColor::HSV(1.f, 0.6f, 0.6f));
                }
                else
                {
                    ImGui::PushStyleColor(ImGuiCol_Button,          (ImVec4)ImColor::HSV(0.4f, 1.f, 0.4f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   (ImVec4)ImColor::HSV(0.4f, 1.f, 0.4f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive,    (ImVec4)ImColor::HSV(0.4f, 1.f, 0.4f));
                }
            }

            if (ImGui::Button(shaderTypes[i]))
            {
                switch (i)
                {
                case 0:
                    m_CurrentShaderComponent = m_CurrentShader->GetVertShader();
                    break;

                case 1:
                    m_CurrentShaderComponent = m_CurrentShader->GetFragShader();
                    break;

                case 2:
                    m_CurrentShaderComponent = m_CurrentShader->GetGeoShader();
                    break;
                }
                UpdateTextBuffer();
                m_CurrentShaderTypeIndex = i;
            }

            if (popStyleColor)
            {
                ImGui::PopStyleColor(3);
            }
        }

        ImVec2 textWindowSize = ImVec2(ImGui::GetWindowWidth(), 300.0f);
        static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;

        ImGui::InputTextMultiline("", m_TextBuffer, m_TextBufferSize, textWindowSize, flags);

        if (m_CurrentShader && ImGui::Button("Recompile"))
        {
            m_CurrentShader->RecompileShaderType(m_CurrentShaderComponent->GetGLShaderType(), DeepCopyString(m_TextBuffer)); // #TODO Review deep copy
        }

        ImGui::End();
    }

    void ShaderEditor::UpdateTextBuffer()
    {
        if (m_CurrentShaderComponent)
        {
            strcpy_s(m_TextBuffer, m_TextBufferSize, m_CurrentShaderComponent->GetStringData());
        }
        else
        {
            strcpy_s(m_TextBuffer, m_TextBufferSize, "");
        }
    }

}
