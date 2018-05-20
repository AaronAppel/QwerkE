#include "imgui_EditComponent.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/ResourceManager/ResourceManager.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/ServiceLocator.h"
#include "../QwerkE_Framework/QwerkE_Framework/Entities/GameObject.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Graphics/Mesh/Model.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Graphics/Mesh/Mesh.h"
#include "../QwerkE_Framework/QwerkE_Framework/Entities/Components/RenderComponent.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Graphics/Gfx_Classes/MaterialData.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Graphics/ShaderProgram/ShaderProgram.h"

imgui_EditComponent::imgui_EditComponent()
{
	m_ResourceManager = ((ResourceManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Resource_Manager));

	m_Materials = m_ResourceManager->LookAtMaterials();
	m_Textures = m_ResourceManager->LookAtTextures();
	m_Models = m_ResourceManager->LookAtModels();
	m_Shaders = m_ResourceManager->LookAtShaders();
}

imgui_EditComponent::~imgui_EditComponent()
{
}

void imgui_EditComponent::Draw(GameObject* entity)
{
	ImGuiCol idx = ImGuiCol_FrameBg; // TODO: Style imgui windows for editing
	ImVec4 col = ImVec4(0, 0, 0, 0);

	if (ImGui::Button("Refresh")) { m_RefreshFlag = 1; }
	if (ImGui::CollapsingHeader("RenderComponent"))
	{
		// TODO: Show values from the shader base on attributes and uniforms.
		// ImGui::PushStyleVar(); ImGui::PushStyleColor(idx, col);

		RenderComponent* rComp = (RenderComponent*)entity->GetComponent(Component_Render);

		// populate asset names
		if (m_RefreshFlag)
		{
			m_RefreshFlag = 0;
			for (const auto &p : *m_Materials)
			{
				m_MatStrings.push_back(p.second->name.c_str());
			}
			for (const auto &p : *m_Shaders)
			{
				m_ShaderStrings.push_back(p.second->GetName().c_str());
			}
			for (const auto &p : *m_Models)
			{
				m_ModelStrings.push_back(p.second->GetName().c_str());
			}
		}

		// draw data
		if (ImGui::ListBox("Shaders", &m_ShaderIndex, &m_ShaderStrings[0], m_ShaderStrings.size(), 4))
		{
			rComp->SetShader(m_ResourceManager->GetShader(m_ShaderStrings[m_ShaderIndex]));
		}
		ImGui::Separator();
		if (ImGui::ListBox("Materials", &m_MatIndex, &m_MatStrings[0], m_MatStrings.size(), 3))
		{
			rComp->SetMaterial(m_ResourceManager->GetMaterial(m_MatStrings[m_MatIndex]));
		}
		ImGui::Separator();
		if (ImGui::ListBox("Models", &m_ModelIndex, &m_ModelStrings[0], m_ModelStrings.size(), 3))
		{
			rComp->SetModel(m_ResourceManager->GetModel(m_ModelStrings[m_ModelIndex]));
		}
		ImGui::Separator();

		// render primitive/mode
		if (ImGui::Button("Tris"))
			// TODO: determine the proper mesh index
			rComp->GetModel()->m_Meshes[0]->SetPrimitiveType(GL_TRIANGLES);
		ImGui::SameLine();
		if (ImGui::Button("TriStrip"))
			rComp->GetModel()->m_Meshes[0]->SetPrimitiveType(GL_TRIANGLE_STRIP);
		ImGui::SameLine();
		if (ImGui::Button("TriFan"))
			rComp->GetModel()->m_Meshes[0]->SetPrimitiveType(GL_TRIANGLE_FAN);
		ImGui::SameLine();
		if (ImGui::Button("Points"))
			rComp->GetModel()->m_Meshes[0]->SetPrimitiveType(GL_POINTS);
		ImGui::SameLine();
		if (ImGui::Button("Lines"))
			rComp->GetModel()->m_Meshes[0]->SetPrimitiveType(GL_LINES);
		ImGui::SameLine();
		if (ImGui::Button("Strips"))
			rComp->GetModel()->m_Meshes[0]->SetPrimitiveType(GL_LINE_STRIP);

		float colors[4] = { rComp->GetColour().x, rComp->GetColour().y, rComp->GetColour().z, rComp->GetColour().w };
		ImGui::DragFloat4("Color", colors, 0.05f, 0.0f, 1.0f);
		rComp->SetColour(vec4(colors[0], colors[1], colors[2], colors[3]));
		// ImGui::PopStyleColor();
	}

	if (ImGui::CollapsingHeader("OtherComponent"))
	{
		ImGui::Text("TODO: Handle other components!");
	}
}