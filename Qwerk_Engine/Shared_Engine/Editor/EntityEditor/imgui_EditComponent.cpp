#include "imgui_EditComponent.h"
#include "../QwerkE_Framework/Systems/ResourceManager/ResourceManager.h"
#include "../QwerkE_Framework/Systems/Services.h"
#include "../QwerkE_Framework/QwerkE_Common/Utilities/StringHelpers.h"
#include "../QwerkE_Framework/Entities/Routines/RenderRoutine.h"
#include "../QwerkE_Framework/Entities/GameObject.h"
#include "../QwerkE_Framework/Entities/Components/RenderComponent.h"
#include "../QwerkE_Framework/Graphics/Texture.h"
#include "../QwerkE_Framework/Graphics/Renderable.h"
#include "../QwerkE_Framework/Graphics/Shader/ShaderProgram.h"
#include "../QwerkE_Framework/Graphics/Mesh/Mesh.h"
#include "../QwerkE_Framework/Graphics/Material.h"

#include <vector>
#include <string>

namespace QwerkE {

    imgui_EditComponent::imgui_EditComponent()
        : m_Materials(Services::Resources.SeeMaterials()), m_Shaders(Services::Resources.SeeShaderPrograms()), m_Meshes(Services::Resources.SeeMeshes())
    {
    }

    imgui_EditComponent::~imgui_EditComponent()
    {
    }

    void imgui_EditComponent::Draw(GameObject* entity)
    {
        ImGuiCol idx = ImGuiCol_FrameBg; // TODO: Style imgui windows for editing
        ImVec4 col = ImVec4(0, 0, 0, 0);

        if (ImGui::Button("Refresh")) { m_RefreshFlag = 1; } // broken

        RenderComponent* rComp = (RenderComponent*)entity->GetComponent(Component_Render);

        if (rComp)
        {
            ImGui::SameLine();
            ImGui::Text(rComp->GetSchematicName().c_str());
            if (ImGui::CollapsingHeader("RenderComponent", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
            {
                // TODO: Show values from the shader base on attributes and uniforms.
                // ImGui::PushStyleVar(); ImGui::PushStyleColor(idx, col);

                std::vector<Renderable>* renderables = (std::vector<Renderable>*)rComp->LookAtRenderableList();
                if (m_RenderableIndex > renderables->size()) m_RenderableIndex = 0;
                // populate asset names
                if (m_RefreshFlag)
                {
                    // Check to see what the current assets are
                    m_RefreshFlag = 0;

                    // cleanup RAM
                    for (size_t i = 0; i < m_MatStrings.size(); i++)
                    {
                        delete m_MatStrings[i];
                    }
                    for (size_t i = 0; i < m_ShaderStrings.size(); i++)
                    {
                        delete m_ShaderStrings[i];
                    }
                    for (size_t i = 0; i < m_MeshStrings.size(); i++)
                    {
                        delete m_MeshStrings[i];
                    }

                    m_MatStrings.clear();
                    m_ShaderStrings.clear();
                    m_MeshStrings.clear();

                    for (const auto& p : *m_Materials)
                    {
                        m_MatStrings.push_back(DeepCopyString(p.second->GetMaterialName().c_str()));
                    }
                    for (const auto& p : *m_Shaders)
                    {
                        m_ShaderStrings.push_back(DeepCopyString(p.second->GetName().c_str())); //RAM:
                    }
                    for (const auto& p : *m_Meshes)
                    {
                        m_MeshStrings.push_back(DeepCopyString(p.second->GetName().c_str())); //RAM:
                    }
                    // TODO: Textures + Meshes
                }

                ImGui::Columns(4, "RenderablesHeader", false);  // 3-ways, no border
                {
                    // name
                    ImGui::Text("Renderable");
                    ImGui::NextColumn();

                    // shader
                    ImGui::Text("Shader");
                    ImGui::NextColumn();

                    // material
                    ImGui::Text("Material");
                    ImGui::NextColumn();

                    // mesh
                    ImGui::Text("Mesh");
                }
                ImGui::Columns(1);
                ImGui::Separator();

                ImGui::Columns(4, "mycolumns3", true);  // 3-ways, no border
                for (int i = 0; i < renderables->size(); i++)
                {
                    if (i > 0)
                        ImGui::NextColumn();

                    // shader
                    if (ImGui::Selectable(renderables->at(i).GetRenderableName().c_str(), m_RenderableIndex == i, ImGuiSelectableFlags_SpanAllColumns))
                    {
                        m_RenderableIndex = i;
                    }
                    ImGui::NextColumn();

                    // shader
                    if (ImGui::Selectable(renderables->at(i).GetShaderSchematic()->GetName().c_str()))
                    {
                        m_ShowShaderList = true;
                        m_RenderableIndex = i;
                    }
                    ImGui::NextColumn();

                    // material
                    // if (i == m_Materialindex) ImGui::PushStyleColor(ImGuiCol(1), ImVec4(1,1,1,1));
                    if (ImGui::Selectable(renderables->at(i).GetMaterialSchematic()->GetMaterialName().c_str()))
                    {
                        m_ShowMaterialList = true;
                        m_RenderableIndex = i;
                    }
                    ImGui::NextColumn();

                    // mesh
                    if (ImGui::Selectable(renderables->at(i).GetMesh()->GetName().c_str()))
                    {
                        m_ShowMeshList = true;
                        m_RenderableIndex = i;
                    }
                }
                ImGui::Columns(1);
                ImGui::Separator();

                if (m_ShowShaderList) ShowShaderMenu(rComp);
                if (m_ShowMaterialList) ShowMaterialMenu(rComp);
                if (m_ShowMeshList) ShowMeshMenu(rComp);

                // shader uniforms and attributes
                const std::vector<std::string>* attributes = rComp->GetRenderableList()->at(m_RenderableIndex).GetShaderSchematic()->SeeAttributes(); // TODO: Fix out of bounds when m_RenderableIndex is too high
                ImGui::Button("Attr");
                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    for (int i = 0; i < attributes->size(); i++)
                    {
                        ImGui::Text(attributes->at(i).c_str());
                    }
                    ImGui::EndTooltip();
                }
                ImGui::SameLine();
                const std::vector<std::string>* uniforms = rComp->GetRenderableList()->at(m_RenderableIndex).GetShaderSchematic()->SeeUniforms();
                ImGui::Button("Unif");
                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    for (int i = 0; i < uniforms->size(); i++)
                    {
                        ImGui::Text(uniforms->at(i).c_str());
                    }
                    ImGui::EndTooltip();
                }
                ImGui::SameLine();

                // render primitive/mode
                static int selection = 0;
                const char* primitives[] = { "Tris", "TriStrip", "TriFan", "Points" ,"Lines", "LineStrip" };
                ImGui::PushItemWidth(90);
                if (ImGui::Combo("Prim", &selection, primitives, 5))
                {
                    switch (selection)
                    {
                    case 0:
                        rComp->GetRenderableList()->at(m_RenderableIndex).GetMesh()->SetPrimitiveType(GL_TRIANGLES);
                        break;
                    case 1:
                        rComp->GetRenderableList()->at(m_RenderableIndex).GetMesh()->SetPrimitiveType(GL_TRIANGLE_STRIP);
                        break;
                    case 2:
                        rComp->GetRenderableList()->at(m_RenderableIndex).GetMesh()->SetPrimitiveType(GL_TRIANGLE_FAN);
                        break;
                    case 3:
                        rComp->GetRenderableList()->at(m_RenderableIndex).GetMesh()->SetPrimitiveType(GL_POINTS);
                        break;
                    case 4:
                        rComp->GetRenderableList()->at(m_RenderableIndex).GetMesh()->SetPrimitiveType(GL_LINES);
                        break;
                    case 5:
                        rComp->GetRenderableList()->at(m_RenderableIndex).GetMesh()->SetPrimitiveType(GL_LINE_STRIP);
                        break;
                    }
                }
                ImGui::PopItemWidth();

                // float colors[4] = { rComp->GetColour().x, rComp->GetColour().y, rComp->GetColour().z, rComp->GetColour().w };
                // ImGui::DragFloat4("Color", colors, 0.05f, 0.0f, 1.0f);
                // rComp->SetColour(vec4(colors[0], colors[1], colors[2], colors[3]));
                // ImGui::PopStyleColor();
            }

            if (ImGui::CollapsingHeader("OtherComponent", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Text("TODO: Handle other components!");
            }
        }
    }

    void imgui_EditComponent::ShowShaderMenu(RenderComponent* rComp)
    {
        if (ImGui::Begin("Shader Selector", &m_ShowShaderList))
        {
            for (size_t i = 0; i < m_ShaderStrings.size(); i++)
            {
                if (ImGui::Selectable(m_ShaderStrings[i]))
                {
                    rComp->SetShaderAtIndex(m_RenderableIndex, Services::Resources.GetShaderProgram(m_ShaderStrings[i]));
                }
            }
            if (ImGui::IsItemClicked(1))
            {
                static bool shaderEditor = true; // TODO:
            }

            ImGui::End();
        }
        else
            ImGui::End();
    }

    void imgui_EditComponent::ShowMaterialMenu(RenderComponent* rComp)
    {
        if (ImGui::Begin("Material Selector", &m_ShowMaterialList))
        {
            for (size_t i = 0; i < m_MatStrings.size(); i++)
            {
                if (ImGui::Selectable(m_MatStrings[i]))
                {
                    rComp->SetMaterialAtIndex(m_RenderableIndex, Services::Resources.GetMaterial(m_MatStrings[i]));
                }
                if (ImGui::IsItemClicked(1))
                {
                    static bool materialEditor = true; // TODO:
                }
            }

            ImGui::End();
        }
        else
            ImGui::End();
    }

    void imgui_EditComponent::ShowMeshMenu(RenderComponent* rComp)
    {
        if (ImGui::Begin("Mesh Selector", &m_ShowMeshList))
        {
            for (size_t i = 0; i < m_MeshStrings.size(); i++)
            {
                if (ImGui::Selectable(m_MeshStrings[i]))
                {
                    rComp->SetMeshAtIndex(m_RenderableIndex, Services::Resources.GetMesh(m_MeshStrings[i]));
                }
            }

            ImGui::End();
        }
        else
            ImGui::End();
    }

}
