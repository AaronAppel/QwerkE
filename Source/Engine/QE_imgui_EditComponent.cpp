#include "QE_EditComponent.h"

#include <string>
#include <vector>

#include "imgui/imgui.h"

#include "QC_StringHelpers.h"

#include "QF_Enums.h"

#include "QF_Assets.h"
#include "QF_RenderRoutine.h"
#include "QF_GameObject.h"
#include "QF_LightComponent.h"
#include "QF_PhysicsComponent.h"
#include "QF_RenderComponent.h"
#include "QF_Texture.h"
#include "QF_Renderable.h"
#include "QF_ShaderProgram.h"
#include "QF_Mesh.h"
#include "QF_Material.h"

#include "QF_Bullet3Component.h" // TESTING:

namespace QwerkE {

    void EditComponent::Draw(GameObject* entity)
    {
        if (m_LastEntity != entity)
        {
            m_SelectedRenderableIndex = 0;
        }

        m_LastEntity = entity;

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
                if (ImGui::CollapsingHeader("OtherComponent", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::Text("Unhandled component type");
                }
                break;
            }
        }
    }

    void EditComponent::ShowShaderList(RenderComponent* rComp)
    {
        if (ImGui::Begin("Shader Selector", &m_ShowShaderList))
        {
            auto shaders = Assets::SeeShaderPrograms();
            for (auto& it : shaders)
            {
                if (ImGui::Selectable(it.first.c_str()))
                {
                    rComp->SetShaderAtIndex(m_SelectedRenderableIndex, Assets::GetShaderProgram(it.first.c_str()));
                }

                if (ImGui::IsItemClicked(1))
                {
                    static bool shaderEditor = true; // #TODO:
                }
            }
        }
        ImGui::End();
    }

    void EditComponent::ShowMaterialList(RenderComponent* rComp)
    {
        if (ImGui::Begin("Material Selector", &m_ShowMaterialList))
        {
            auto materials = Assets::SeeMaterials();
            for (auto& it : materials)
            {
                if (ImGui::Selectable(it.first.c_str()))
                {
                    rComp->SetMaterialAtIndex(m_SelectedRenderableIndex, Assets::GetMaterial(it.first.c_str()));
                }

                if (ImGui::IsItemClicked(1))
                {
                    static bool materialEditor = true; // #TODO:
                }
            }
        }

        ImGui::End();
    }

    void EditComponent::ShowMeshList(RenderComponent* rComp)
    {
        if (ImGui::Begin("Mesh Selector", &m_ShowMeshList))
        {
            auto meshes = Assets::SeeMeshes();
            for (auto& it : meshes)
            {
                if (ImGui::Selectable(it.first.c_str()))
                {
                    rComp->SetMeshAtIndex(m_SelectedRenderableIndex, Assets::GetMesh(it.first.c_str()));
                }
            }
        }
        ImGui::End();
    }

    void EditComponent::ShowRenderComponent(RenderComponent* renderComponent)
    {
        if (renderComponent && ImGui::CollapsingHeader("RenderComponent", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (renderComponent->GetSchematicName().c_str())
            {
                ImGui::Text(renderComponent->GetSchematicName().c_str());
            }

            // TODO: Show values from the shader base on attributes and uniforms.
            // ImGui::PushStyleVar(); ImGui::PushStyleColor(idx, col);

            std::vector<Renderable>* renderables = (std::vector<Renderable>*)renderComponent->LookAtRenderableList();
            if (m_SelectedRenderableIndex > renderables->size()) m_SelectedRenderableIndex = 0;

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
                if (ImGui::Selectable(renderables->at(i).GetRenderableName().c_str(), m_SelectedRenderableIndex == i, ImGuiSelectableFlags_SpanAllColumns))
                {
                    m_SelectedRenderableIndex = i;
                }
                ImGui::NextColumn();

                // Shader
                if (ImGui::Selectable(renderables->at(i).GetShader()->GetName().c_str()))
                {
                    m_ShowShaderList = true;
                    m_SelectedRenderableIndex = i;
                }
                ImGui::NextColumn();

                // Material
                // if (i == m_Materialindex) ImGui::PushStyleColor(ImGuiCol(1), ImVec4(1,1,1,1));
                if (ImGui::Selectable(renderables->at(i).GetMaterial()->GetMaterialName().c_str()))
                {
                    m_ShowMaterialList = true;
                    m_SelectedRenderableIndex = i;
                }
                ImGui::NextColumn();

                // Mesh
                if (ImGui::Selectable(renderables->at(i).GetMesh()->GetName().c_str()))
                {
                    m_ShowMeshList = true;
                    m_SelectedRenderableIndex = i;
                }
            }
            ImGui::Columns(1);
            ImGui::Separator();

            if (m_ShowShaderList) ShowShaderList(renderComponent);
            if (m_ShowMaterialList) ShowMaterialList(renderComponent);
            if (m_ShowMeshList) ShowMeshList(renderComponent);

            // Shader uniforms and attributes

            if (renderables->empty() && renderComponent->GetParent() && ImGui::Button("+Renderable"))
            {
                Renderable renderable;
                static int uniqueId = 0; // #FEATURE F0004, F0003
                renderable.SetRenderableName(std::to_string(uniqueId++));
                renderComponent->AddRenderable(renderable);
                renderComponent->Setup(null_shader, null_material, null_mesh);
                // renderComponent->Activate();
            }

            if (renderables->size() <= m_SelectedRenderableIndex)
                return;

            Renderable& renderable = renderables->at(m_SelectedRenderableIndex);
            const std::vector<std::string>* attributes = renderable.GetShader()->SeeAttributes();
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
            const std::vector<std::string>* uniforms = renderable.GetShader()->SeeUniforms();
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
                    renderable.GetMesh()->SetPrimitiveType(GL_TRIANGLES);
                    break;
                case 1:
                    renderable.GetMesh()->SetPrimitiveType(GL_TRIANGLE_STRIP);
                    break;
                case 2:
                    renderable.GetMesh()->SetPrimitiveType(GL_TRIANGLE_FAN);
                    break;
                case 3:
                    renderable.GetMesh()->SetPrimitiveType(GL_POINTS);
                    break;
                case 4:
                    renderable.GetMesh()->SetPrimitiveType(GL_LINES);
                    break;
                case 5:
                    renderable.GetMesh()->SetPrimitiveType(GL_LINE_STRIP);
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
