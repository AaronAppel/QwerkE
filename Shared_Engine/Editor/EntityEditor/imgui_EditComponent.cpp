#include "imgui_EditComponent.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/ResourceManager/ResourceManager.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/ServiceLocator.h"
#include "../QwerkE_Framework/QwerkE_Framework/Graphics/Mesh/Mesh.h"
#include "../QwerkE_Framework/QwerkE_Framework/Graphics/MaterialData.h"
#include "../QwerkE_Framework/QwerkE_Common/Utilities/StringHelpers.h"
#include "../QwerkE_Framework/QwerkE_Framework/Entities/Routines/RenderRoutine.h"
#include "../QwerkE_Framework/QwerkE_Framework/Entities/GameObject.h"
#include "../QwerkE_Framework/QwerkE_Framework/Entities/Components/RenderComponent.h"
#include "../QwerkE_Framework/QwerkE_Framework/Graphics/Texture.h"
#include "../QwerkE_Framework/QwerkE_Framework/Graphics/Renderable.h"
#include "../QwerkE_Framework/QwerkE_Framework/Graphics/Shader/ShaderProgram.h"

imgui_EditComponent::imgui_EditComponent()
{
	m_ResourceManager = ((ResourceManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Resource_Manager));

	m_Materials = m_ResourceManager->LookAtMaterials();
	m_Textures = m_ResourceManager->LookAtTextures();
	m_Shaders = m_ResourceManager->LookAtShaderProgram();
	m_Meshes = m_ResourceManager->LookAtMeshes();
}

imgui_EditComponent::~imgui_EditComponent()
{
}

void imgui_EditComponent::Draw(GameObject* entity)
{
	ImGuiCol idx = ImGuiCol_FrameBg; // TODO: Style imgui windows for editing
	ImVec4 col = ImVec4(0, 0, 0, 0);

	if (ImGui::Button("Refresh")) { m_RefreshFlag = 1; }

	RenderComponent* rComp = (RenderComponent*)entity->GetComponent(Component_Model);

	if (rComp)
		if (ImGui::CollapsingHeader("RenderComponent", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
		{
			// TODO: Show values from the shader base on attributes and uniforms.
			// ImGui::PushStyleVar(); ImGui::PushStyleColor(idx, col);

			std::vector<Renderable>* renderables = (std::vector<Renderable>*)rComp->LookAtRenderableList();

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
				for (size_t i = 0; i < m_TextureStrings.size(); i++)
				{
					delete m_TextureStrings[i];
				}
				for (size_t i = 0; i < m_MeshStrings.size(); i++)
				{
					delete m_MeshStrings[i];
				}

				m_MatStrings.clear();
				m_ShaderStrings.clear();
				m_TextureStrings.clear();
				m_MeshStrings.clear();

				int counter = 0; // TODO: Remove
				for (const auto &p : *m_Materials)
				{
					m_MatStrings.push_back(p.second->s_Name.c_str());
					if (p.second->s_Name.c_str()) m_MatIndex = counter;
					counter++;
				}
				counter = 0;
				for (const auto &p : *m_Shaders)
				{
					m_ShaderStrings.push_back(DeepCopyString(p.second->GetName().c_str())); //RAM:
					if (p.second->GetName().c_str()) m_ShaderIndex = counter;
					counter++;
				}
				for (const auto &p : *m_Textures)
				{
					m_TextureStrings.push_back(DeepCopyString(std::to_string(p.second).c_str())); //RAM:
				}
				for (const auto &p : *m_Meshes)
				{
					m_MeshStrings.push_back(DeepCopyString(p.second->GetName().c_str())); //RAM:
				}
				// TODO: Textures + Meshes
				counter = 0;
			}

			ImGui::Columns(3, "RenderablesHeader", false);  // 3-ways, no border
			{
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

			ImGui::Columns(3, "mycolumns3", true);  // 3-ways, no border
			for (int i = 0; i < renderables->size(); i++)
			{
				if (i > 0)
					ImGui::NextColumn();

				// shader
				if (ImGui::Selectable(renderables->at(i).GetShaderSchematic()->GetName().c_str()))
				{
					m_ShowShaderList = true;
					m_Shaderindex = i;
				}
				ImGui::NextColumn();

				// material
				if (ImGui::Selectable(renderables->at(i).GetMaterialSchematic()->s_Name.c_str()))
				{
					m_ShowMaterialList = true;
					m_Materialindex = i;
				}
				ImGui::NextColumn();

				// mesh
				if (ImGui::Selectable(renderables->at(i).GetMesh()->GetName().c_str()))
				{
					m_ShowMeshList = true;
					m_Meshindex = i;
				}
			}
			ImGui::Columns(1);
			ImGui::Separator();

			if (m_ShowShaderList) ShowShaderMenu(rComp);
			if (m_ShowMaterialList) ShowMaterialMenu(rComp);
			if (m_ShowMeshList) ShowMeshMenu(rComp);

			// draw data

			// render primitive/mode
			// TODO: Find a good way to set renderable mesh primitive
			if (ImGui::Button("Tris"))
				// TODO: determine the proper mesh index
				;// rComp->GetModel()->m_Meshes[0]->SetPrimitiveType(GL_TRIANGLES);
			ImGui::SameLine();
			if (ImGui::Button("TriStrip"))
				;// rComp->GetModel()->m_Meshes[0]->SetPrimitiveType(GL_TRIANGLE_STRIP);
			ImGui::SameLine();
			if (ImGui::Button("TriFan"))
				;// rComp->GetModel()->m_Meshes[0]->SetPrimitiveType(GL_TRIANGLE_FAN);
			if (ImGui::Button("Points"))
				;// rComp->GetModel()->m_Meshes[0]->SetPrimitiveType(GL_POINTS);
			ImGui::SameLine();
			if (ImGui::Button("Lines"))
				;// rComp->GetModel()->m_Meshes[0]->SetPrimitiveType(GL_LINES);
			ImGui::SameLine();
			if (ImGui::Button("Strips"))
				;// rComp->GetModel()->m_Meshes[0]->SetPrimitiveType(GL_LINE_STRIP);

			;// float colors[4] = { rComp->GetColour().x, rComp->GetColour().y, rComp->GetColour().z, rComp->GetColour().w };
			;// ImGui::DragFloat4("Color", colors, 0.05f, 0.0f, 1.0f);
			;// rComp->SetColour(vec4(colors[0], colors[1], colors[2], colors[3]));
			// ImGui::PopStyleColor();
		}

	if (ImGui::CollapsingHeader("OtherComponent", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("TODO: Handle other components!");
	}
}

void imgui_EditComponent::ShowShaderMenu(RenderComponent* rComp)
{
	if (ImGui::Begin("Shader Selector", &m_ShowShaderList))
	{
		ImGui::SetWindowFocus();

		for (size_t i = 0; i < m_ShaderStrings.size(); i++)
		{
			if (ImGui::Selectable(m_ShaderStrings[i]))
			{
				rComp->SetShaderAtIndex(m_Shaderindex, m_ResourceManager->GetShaderProgram(m_ShaderStrings[i]));
			}
		}

		ImGui::End();
	}
}

void imgui_EditComponent::ShowMaterialMenu(RenderComponent* rComp)
{
	if (ImGui::Begin("Material Selector", &m_ShowMaterialList))
	{
		ImGui::SetWindowFocus();

		for (size_t i = 0; i < m_MatStrings.size(); i++)
		{
			if (ImGui::Selectable(m_MatStrings[i]))
			{
				rComp->SetMaterialAtIndex(m_Materialindex, m_ResourceManager->GetMaterial(m_MatStrings[i]));
			}
		}

		ImGui::End();
	}
}

void imgui_EditComponent::ShowMeshMenu(RenderComponent* rComp)
{
	if (ImGui::Begin("Mesh Selector", &m_ShowMeshList))
	{
		ImGui::SetWindowFocus();

		for (size_t i = 0; i < m_MeshStrings.size(); i++)
		{
			if (ImGui::Selectable(m_MeshStrings[i]))
			{
				rComp->SetMeshAtIndex(m_Meshindex, m_ResourceManager->GetMesh(m_MeshStrings[i]));
			}
		}

		ImGui::End();
	}
}