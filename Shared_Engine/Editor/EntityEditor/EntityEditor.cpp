#include "EntityEditor.h"
#include "../QwerkE_Framework/QwerkE_Common/Libraries/imgui/imgui.h"
#include "../QwerkE_Framework/QwerkE_Framework/Scenes/SceneManager.h"
#include "../QwerkE_Framework/QwerkE_Framework/Scenes/Scene.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/ResourceManager.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/ServiceLocator.h"
#include "../QwerkE_Framework/QwerkE_Framework/Entities/GameObject.h"
#include "../QwerkE_Framework/QwerkE_Framework/Entities/Components/RenderComponent.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Graphics/Mesh/Mesh.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Graphics/Mesh/Model.h"
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
// imgui styling: https://www.unknowncheats.me/forum/direct3d/189635-imgui-style-settings.html
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
	//// Begine drawing entity data...
	// Draw generic GameObject data like transform and name
	static std::string name = thing->GetName().c_str() + std::to_string(' '); // extra space for editing
	ImGui::InputText("Name: ", (char*)name.data(), name.size());
	// thing->SetName(); // TODO: Scene map probably needs to handle name changes

	static float pos[3] = {0.0f, 0.0f, 0.0f};
	pos[0] = thing->GetPosition().x;
	pos[1] = thing->GetPosition().y;
	pos[2] = thing->GetPosition().z;
	static float rot[3] = {0.0f, 0.0f, 0.0f};
	rot[0] = thing->GetRotation().x;
	rot[1] = thing->GetRotation().y;
	rot[2] = thing->GetRotation().z;
	static float scale[3] = {1.0f, 1.0f, 1.0f};
	scale[0] = thing->GetScale().x;
	scale[1] = thing->GetScale().y;
	scale[2] = thing->GetScale().z;

	if(ImGui::InputFloat3("Pos: ", pos))
	{
		thing->SetPosition(vec3(pos[0], pos[1], pos[2]));
	}
	if(ImGui::InputFloat3("Rot: ", rot))
	{
		thing->SetRotation(vec3(rot[0], rot[1], rot[2]));
	}
	if(ImGui::InputFloat3("Scale: ", scale))
	{
		thing->SetScale(vec3(scale[0], scale[1], scale[2]));
	}
	ImGui::Separator();

	// Draw properties for object components like render, character, etc

    static int meshIndex = 0;
    static int shaderIndex = 0;
    static int matIndex = 0;
	static int modelIndex = 0;
    static const char* meshes[3] = { "Plane", "TutorialCube", "Box" };
    static const char* shaders[4] = { "LitMaterial", "Basic3D", "TestShader", "Basic2D" };
    static const char* materials[4] = { "container.mat", "UV_Map.mat", "null_material.mat", "green_canvas.png" };
	static const char* models[3] = { "nanosuit/nanosuit.obj", "Alexstrasza/Alexstrasza.obj", "sci-fi-assault-rifle/All.fbx.fbx" };

	ImGuiCol idx = ImGuiCol_FrameBg; // TODO: Style imgui windows for editing
	ImVec4 col = ImVec4(0,0,0,0);

	if(ImGui::CollapsingHeader("RenderComponent"))
	{
		ImGui::PushStyleColor(idx, col);
		// ImGui::PushStyleVar();
		RenderComponent* rComp = (RenderComponent*)thing->GetComponent(Component_Render);
		// draw data
		if (ImGui::ListBox("Meshes", &meshIndex, meshes, 3, 3))
		{
			rComp->SetMesh(m_ResourceManager->GetMesh(meshes[meshIndex]));
		}
		if (ImGui::ListBox("Shaders", &shaderIndex, shaders, 4, 4))
		{
			rComp->SetShader(m_ResourceManager->GetShader(shaders[shaderIndex]));
		}
		if (ImGui::ListBox("Materials", &matIndex, materials, 4, 4))
		{
			rComp->SetMaterial(m_ResourceManager->GetMaterial(materials[matIndex]));
		}
		if (ImGui::ListBox("Models", &modelIndex, models, 3, 3))
		{
			rComp->SetModel(m_ResourceManager->GetModel(models[modelIndex]));
		}
		// render primitive/mode
		if (ImGui::Button("Tris"))
		{
			rComp->GetModel()->m_Meshes[1]->SetPrimitiveType(GL_TRIANGLES);
		}
		ImGui::SameLine();
		if (ImGui::Button("TriStrip"))
		{
			rComp->GetModel()->m_Meshes[1]->SetPrimitiveType(GL_TRIANGLE_STRIP);
		}
		ImGui::SameLine();
		if (ImGui::Button("TriFan"))
		{
			rComp->GetModel()->m_Meshes[1]->SetPrimitiveType(GL_TRIANGLE_FAN);
		}
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
		ImGui::PopStyleColor();
	}

    ImGui::End();
}