#include "EntityEditor.h"
#include "../QwerkE_Framework/QwerkE_Common/Libraries/imgui/imgui.h"
#include "../QwerkE_Framework/QwerkE_Framework/Scenes/SceneManager.h"
#include "../QwerkE_Framework/QwerkE_Framework/Scenes/Scene.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/ResourceManager.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/ServiceLocator.h"
#include "../QwerkE_Framework/QwerkE_Framework/Entities/GameObject.h"
#include "../QwerkE_Framework/QwerkE_Framework/Entities/Components/RenderComponent.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Graphics/Model/Mesh/Mesh.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Graphics/Model/Model.h"
#include "../Editor.h"

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

	if (ImGui::Button("Tris"))
	{
		rComp->GetModel()->m_Meshes[1]->SetPrimitiveType(GL_TRIANGLES);
	}
	ImGui::SameLine();
	if (ImGui::Button("Points"))
	{
		rComp->GetModel()->m_Meshes[1]->SetPrimitiveType(GL_POINTS);
	}
	ImGui::SameLine();
	if (ImGui::Button("Lines"))
	{
		rComp->GetModel()->m_Meshes[1]->SetPrimitiveType(GL_LINES);
	}
	ImGui::SameLine();
	if (ImGui::Button("Strips"))
	{
		rComp->GetModel()->m_Meshes[1]->SetPrimitiveType(GL_LINE_STRIP);
	}

    float colors[4] = { rComp->GetColour().x, rComp->GetColour().y, rComp->GetColour().z, rComp->GetColour().w };
    ImGui::DragFloat4("Color", colors, 0.05f, 0.0f, 1.0f);
    rComp->SetColour(vec4(colors[0], colors[1], colors[2], colors[3]));


    ImGui::End();
}