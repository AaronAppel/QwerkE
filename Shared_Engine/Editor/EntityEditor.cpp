#include "EntityEditor.h"
#include "../../../Shared_Generic/Libraries/imgui/imgui.h"
#include "../Scene/SceneManager.h"
#include "../Systems/ResourceManager.h"
#include "../Scene/Scene.h"
#include "../Systems/ServiceLocator.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Components/RenderComponent.h"
#include "Editor.h"

#include <map>
#include <string>

EntityEditor::EntityEditor(Editor* editor)
{
    m_Editor = editor;
    m_SceneManager = (SceneManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Scene_Manager);
    m_ResourceManager = (ResourceManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Resource_Manager);
}

EntityEditor::~EntityEditor()
{
}

void EntityEditor::Draw()
{
    static bool isOpen = true;
    ImGui::Begin("Entity Editor", &isOpen);

    GameObject* thing = m_SceneManager->GetCurrentScene()->GetGameObject(m_CurrentEntity.c_str());
    if (thing == nullptr)
    {
        ImGui::End();
        return; // no object selected
    }

    static int meshIndex = 0;
    static int shaderIndex = 0;
    static int matIndex = 0;
    const char* meshes[3] = { "Plane", "TutorialCube", "Box" };
    const char* shaders[3] = { "LitMaterial", "Basic3D", "TestShader" };
    const char* materials[3] = { "container", "UV_Map", "null" };

    RenderComponent* rComp = (RenderComponent*)thing->GetComponent(Component_Render);
    ImGui::Text(thing->GetName().c_str());
    if (ImGui::ListBox("Meshes", &meshIndex, meshes, 3, 3))
    {
        rComp->SetMesh(m_ResourceManager->GetMesh(meshes[meshIndex]));
    }
    if (ImGui::ListBox("Shaders", &shaderIndex, shaders, 3, 3))
    {
        rComp->SetShader(m_ResourceManager->GetShader(shaders[shaderIndex]));
    }
    if (ImGui::ListBox("Materials", &matIndex, materials, 3, 3))
    {
        rComp->SetMaterial(m_ResourceManager->GetMaterial(materials[matIndex]));
    }

    float colors[4] = { rComp->GetColour().x, rComp->GetColour().y, rComp->GetColour().z, rComp->GetColour().w };
    ImGui::DragFloat4("Color", colors, 0.05f, 0.0f, 1.0f);
    rComp->SetColour(vec4(colors[0], colors[1], colors[2], colors[3]));
    

    ImGui::End();
}