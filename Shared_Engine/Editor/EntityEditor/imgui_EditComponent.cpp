#include "imgui_EditComponent.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/ResourceManager.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/ServiceLocator.h"
#include "../QwerkE_Framework/QwerkE_Framework/Entities/GameObject.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Graphics/Mesh/Model.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Graphics/Mesh/Mesh.h"
#include "../QwerkE_Framework/QwerkE_Framework/Entities/Components/RenderComponent.h"

imgui_EditComponent::imgui_EditComponent()
{
	m_ResourceManager = ((ResourceManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Resource_Manager));
}

imgui_EditComponent::~imgui_EditComponent()
{
}

void imgui_EditComponent::Draw(GameObject* entity)
{
	static int meshIndex = 0;
	static int shaderIndex = 0;
	static int matIndex = 0;
	static int modelIndex = 0;
	static const char* meshes[3] = { "Plane", "TutorialCube", "Box" };
	static const char* shaders[4] = { "LitMaterial", "Basic3D", "TestShader", "Basic2D" };
	static const char* materials[4] = { "container.mat", "UV_Map.mat", "null_material.mat", "green_canvas.png" };
	static const char* models[3] = { "nanosuit/nanosuit.obj", "Alexstrasza/Alexstrasza.obj", "scifi_assault_rifle/scifi_assault_rifle.obj" };

	ImGuiCol idx = ImGuiCol_FrameBg; // TODO: Style imgui windows for editing
	ImVec4 col = ImVec4(0, 0, 0, 0);

	if (ImGui::CollapsingHeader("RenderComponent"))
	{
		// TODO: Show values from the shader base on attributes and uniforms.

		ImGui::PushStyleColor(idx, col);
		// ImGui::PushStyleVar();
		RenderComponent* rComp = (RenderComponent*)entity->GetComponent(Component_Render);
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

	if (ImGui::CollapsingHeader("OtherComponent"))
	{
		ImGui::Text("TODO: Handle other components!");
	}
}