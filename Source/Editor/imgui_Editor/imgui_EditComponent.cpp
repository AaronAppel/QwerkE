#include "../EditComponent.h"

#include <string>
#include <vector>

#include "../QwerkE_Framework/Libraries/imgui/imgui.h"

#include "../QwerkE_Framework/Source/Headers/QwerkE_Enums.h"

#include "../QwerkE_Framework/Source/Utilities/StringHelpers.h"
#include "../QwerkE_Framework/Source/Core/Resources/Resources.h"
#include "../QwerkE_Framework/Source/Core/Scenes/Entities/Routines/RenderRoutine.h"
#include "../QwerkE_Framework/Source/Core/Scenes/Entities/GameObject.h"
#include "../QwerkE_Framework/Source/Core/Scenes/Entities/Components/LightComponent.h"
#include "../QwerkE_Framework/Source/Core/Scenes/Entities/Components/PhysicsComponent.h"
#include "../QwerkE_Framework/Source/Core/Scenes/Entities/Components/RenderComponent.h"
#include "../QwerkE_Framework/Source/Core/Graphics/DataTypes/Texture.h"
#include "../QwerkE_Framework/Source/Core/Graphics/DataTypes/Renderable.h"
#include "../QwerkE_Framework/Source/Core/Graphics/Shader/ShaderProgram.h"
#include "../QwerkE_Framework/Source/Core/Graphics/Mesh/Mesh.h"
#include "../QwerkE_Framework/Source/Core/Graphics/DataTypes/Material.h"

// TESTING:
#include "../QwerkE_Framework/Source/Core/Scenes/Entities/Components/Extended/Bullet3Component.h"

namespace QwerkE {

    EditComponent::EditComponent()
        : m_Materials(Resources::SeeMaterials()), m_Shaders(Resources::SeeShaderPrograms()), m_Meshes(Resources::SeeMeshes())
    {
    }

    EditComponent::~EditComponent()
    {
        CleanUpAssetStrings();
    }

    void EditComponent::Draw(GameObject* entity)
    {
        if (m_LastEntity != entity)
        {
            m_RenderableIndex = 0;
        }

        m_LastEntity = entity;

        if (ImGui::Button("Refresh")) { m_RefreshAssetStrings = true; } // #TODO Bug review

        for (auto it = entity->SeeComponents()->begin(); it != entity->SeeComponents()->end(); ++it)
        {
            switch (it->first)
            {
            case eComponentTags::Component_Render:
                ShowRenderComponent((RenderComponent*)it->second);
                break;

            case eComponentTags::Component_Physics:
                ShowPhysicsComponent((PhysicsComponent*)it->second);
                break;

            case eComponentTags::Component_Light:
                ShowLightComponent((LightComponent*)it->second);
                break;

            case eComponentTags::Component_SoundListener:
                ShowAudioComponent((AudioComponent*)it->second);
                break;

            default:

                // LOG_ERROR("Unknown component type found!");

                if (ImGui::CollapsingHeader("OtherComponent", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::Text("#TODO Unhandled component type!");
                }
                break;
            }
        }
    }

    void EditComponent::CleanUpAssetStrings()
    {
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
    }

    void EditComponent::ShowShaderMenu(RenderComponent* rComp)
    {
        if (ImGui::Begin("Shader Selector", &m_ShowShaderList))
        {
            for (size_t i = 0; i < m_ShaderStrings.size(); i++)
            {
                if (ImGui::Selectable(m_ShaderStrings[i]))
                {
                    rComp->SetShaderAtIndex(m_RenderableIndex, Resources::GetShaderProgram(m_ShaderStrings[i]));
                }
            }

            if (ImGui::IsItemClicked(1))
            {
                static bool shaderEditor = true;
            }
        }
        ImGui::End();
    }

    void EditComponent::ShowMaterialMenu(RenderComponent* rComp)
    {
        if (ImGui::Begin("Material Selector", &m_ShowMaterialList))
        {
            for (size_t i = 0; i < m_MatStrings.size(); i++)
            {
                if (ImGui::Selectable(m_MatStrings[i]))
                {
                    rComp->SetMaterialAtIndex(m_RenderableIndex, Resources::GetMaterial(m_MatStrings[i]));
                }
                if (ImGui::IsItemClicked(1))
                {
                    static bool materialEditor = true; // #TODO:
                }
            }
        }

        ImGui::End();
    }

    void EditComponent::ShowMeshMenu(RenderComponent* rComp)
    {
        if (ImGui::Begin("Mesh Selector", &m_ShowMeshList))
        {
            for (size_t i = 0; i < m_MeshStrings.size(); i++)
            {
                if (ImGui::Selectable(m_MeshStrings[i]))
                {
                    rComp->SetMeshAtIndex(m_RenderableIndex, Resources::GetMesh(m_MeshStrings[i]));
                }
            }
        }
        ImGui::End();
    }

    void EditComponent::ShowRenderComponent(RenderComponent* renderComponent)
    {
        if (ImGui::CollapsingHeader("RenderComponent", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (renderComponent->GetSchematicName().c_str())
            {
                ImGui::Text(renderComponent->GetSchematicName().c_str());
            }

            // TODO: Show values from the shader base on attributes and uniforms.
            // ImGui::PushStyleVar(); ImGui::PushStyleColor(idx, col);

            std::vector<Renderable>* renderables = (std::vector<Renderable>*)renderComponent->LookAtRenderableList();
            if (m_RenderableIndex > renderables->size()) m_RenderableIndex = 0;

            if (m_RefreshAssetStrings)
            {
                m_RefreshAssetStrings = false;

                CleanUpAssetStrings();

                for (const auto& p : *m_Materials)
                {
                    m_MatStrings.push_back(DeepCopyString(p.second->GetMaterialName().c_str()));
                }
                for (const auto& p : *m_Shaders)
                {
                    m_ShaderStrings.push_back(DeepCopyString(p.second->GetName().c_str()));
                }
                for (const auto& p : *m_Meshes)
                {
                    m_MeshStrings.push_back(DeepCopyString(p.second->GetName().c_str()));
                }

                // #TODO Also show Textures + Meshes
            }

            ImGui::Columns(4, "RenderablesHeader", false);
            {
                ImGui::Text("Renderable");
                ImGui::NextColumn();

                ImGui::Text("Shader");
                ImGui::NextColumn();

                ImGui::Text("Material");
                ImGui::NextColumn();

                ImGui::Text("Mesh");
            }
            ImGui::Columns(1);
            ImGui::Separator();

            ImGui::Columns(4, "mycolumns3", true);  // 3-ways, no border
            for (unsigned int i = 0; i < renderables->size(); i++)
            {
                if (i > 0)
                    ImGui::NextColumn();

                // Renderable
                if (ImGui::Selectable(renderables->at(i).GetRenderableName().c_str(), m_RenderableIndex == i, ImGuiSelectableFlags_SpanAllColumns))
                {
                    m_RenderableIndex = i;
                }
                ImGui::NextColumn();

                // Shader
                if (ImGui::Selectable(renderables->at(i).GetShaderSchematic()->GetName().c_str()))
                {
                    m_ShowShaderList = true;
                    m_RenderableIndex = i;
                }
                ImGui::NextColumn();

                // Material
                // if (i == m_Materialindex) ImGui::PushStyleColor(ImGuiCol(1), ImVec4(1,1,1,1));
                if (ImGui::Selectable(renderables->at(i).GetMaterialSchematic()->GetMaterialName().c_str()))
                {
                    m_ShowMaterialList = true;
                    m_RenderableIndex = i;
                }
                ImGui::NextColumn();

                // Mesh
                if (ImGui::Selectable(renderables->at(i).GetMesh()->GetName().c_str()))
                {
                    m_ShowMeshList = true;
                    m_RenderableIndex = i;
                }
            }
            ImGui::Columns(1);
            ImGui::Separator();

            if (m_ShowShaderList) ShowShaderMenu(renderComponent);
            if (m_ShowMaterialList) ShowMaterialMenu(renderComponent);
            if (m_ShowMeshList) ShowMeshMenu(renderComponent);

            // Shader uniforms and attributes
            const std::vector<std::string>* attributes = renderComponent->GetRenderableList()->at(m_RenderableIndex).GetShaderSchematic()->SeeAttributes(); // TODO: Fix out of bounds when m_RenderableIndex is too high
            ImGui::Button("Attr");
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                for (unsigned int i = 0; i < attributes->size(); i++)
                {
                    ImGui::Text(attributes->at(i).c_str());
                }
                ImGui::EndTooltip();
            }
            ImGui::SameLine();
            const std::vector<std::string>* uniforms = renderComponent->GetRenderableList()->at(m_RenderableIndex).GetShaderSchematic()->SeeUniforms();
            ImGui::Button("Unif");
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                for (unsigned int i = 0; i < uniforms->size(); i++)
                {
                    ImGui::Text(uniforms->at(i).c_str());
                }
                ImGui::EndTooltip();
            }
            ImGui::SameLine();

            // Render primitive/mode
            static int selection = 0;
            const char* primitives[] = { "Tris", "TriStrip", "TriFan", "Points" ,"Lines", "LineStrip" };
            ImGui::PushItemWidth(90);
            if (ImGui::Combo("Prim", &selection, primitives, 5))
            {
                switch (selection)
                {
                case 0:
                    renderComponent->GetRenderableList()->at(m_RenderableIndex).GetMesh()->SetPrimitiveType(GL_TRIANGLES);
                    break;
                case 1:
                    renderComponent->GetRenderableList()->at(m_RenderableIndex).GetMesh()->SetPrimitiveType(GL_TRIANGLE_STRIP);
                    break;
                case 2:
                    renderComponent->GetRenderableList()->at(m_RenderableIndex).GetMesh()->SetPrimitiveType(GL_TRIANGLE_FAN);
                    break;
                case 3:
                    renderComponent->GetRenderableList()->at(m_RenderableIndex).GetMesh()->SetPrimitiveType(GL_POINTS);
                    break;
                case 4:
                    renderComponent->GetRenderableList()->at(m_RenderableIndex).GetMesh()->SetPrimitiveType(GL_LINES);
                    break;
                case 5:
                    renderComponent->GetRenderableList()->at(m_RenderableIndex).GetMesh()->SetPrimitiveType(GL_LINE_STRIP);
                    break;
                }
            }
            ImGui::PopItemWidth();

            // float colors[4] = { rComp->GetColour().x, rComp->GetColour().y, rComp->GetColour().z, rComp->GetColour().w };
            // ImGui::DragFloat4("Color", colors, 0.05f, 0.0f, 1.0f);
            // rComp->SetColour(vec4(colors[0], colors[1], colors[2], colors[3]));
            // ImGui::PopStyleColor();
        }
    }

    void EditComponent::ShowPhysicsComponent(PhysicsComponent* pComp)
    {
        if (ImGui::CollapsingHeader("PhysicsComponent", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
        {
            // TODO: Remove Bullet3 testing code
            Bullet3Component* bulletComponent = (Bullet3Component*)pComp;
            static float amount[3] = { 0.0f, 10.0f, 0.0f };

            if (ImGui::InputFloat3("Force: ", amount))
            {
            }

            if (ImGui::Button("ApplyForce"))
            {
                bulletComponent->ApplyForce(vec3(amount[0], amount[1], amount[2]));
            }

            if (ImGui::Button("ApplyTorque"))
            {
                bulletComponent->ApplyForce(vec3(amount[0], amount[1], amount[2]));
            }
        }
    }

    void EditComponent::ShowLightComponent(LightComponent* pComp)
    {
        if (ImGui::CollapsingHeader("LightComponent", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
        {
        }
    }

    void EditComponent::ShowAudioComponent(AudioComponent* pComp)
    {
        if (ImGui::CollapsingHeader("AudioComponent", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
        {
        }
    }
}
