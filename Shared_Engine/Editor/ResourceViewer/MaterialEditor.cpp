#include "MaterialEditor.h"
#include "../QwerkE_Framework/QwerkE_Framework/Graphics/Material.h"
#include "../QwerkE_Framework/QwerkE_Framework/Graphics/Texture.h"
#include "../QwerkE_Framework/QwerkE_Framework/QwerkE_Enums.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/ServiceLocator.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/ResourceManager/ResourceManager.h"

#include <map>

MaterialEditor::MaterialEditor()
{
	m_TextureList = ((ResourceManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Resource_Manager))->LookAtTextures();
}

MaterialEditor::~MaterialEditor()
{
}

void MaterialEditor::Draw(Material* material)
{
	// NOTE: For now just replace existing texture.
	// In the future think of a way to preserve old data.
	static bool isOpen = true;
	if (ImGui::Begin("Material Editor"), &isOpen)
	{
		const std::map<eMaterialMaps, Texture*>* textures = material->SeeMaterials();
		// display material textures
		ImGui::Text(material->GetMaterialName().c_str());
		ImGui::Separator();
		int counter = 0;
		for (auto p : *textures)
		{
			if (counter > 0)
				ImGui::SameLine();

			ImGui::SameLine();
			ImGui::Text(std::to_string((int)p.first).c_str());
			ImGui::SameLine();
			if (m_CurrentMap == p.first)
			{
				ImGui::ImageButton((ImTextureID)p.second->s_Handle, ImVec2(70, 70), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), 1);
			}
			else
			{
				ImGui::ImageButton((ImTextureID)p.second->s_Handle, ImVec2(64, 64), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), 1);
			}

			// TODO: Add the ability to add or remove textures from a material.

			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text(p.second->s_Name.c_str());
				ImGui::Text(std::to_string(p.second->s_Handle).c_str());
				//ImGui::Text("TagName");
				ImGui::EndTooltip();
			}
			if (ImGui::IsItemClicked())
				m_CurrentMap = p.first;
			counter++;
		}

		ImGui::Separator();
		counter = textures->size() + 1;
		for (auto p : *m_TextureList)
		{
			if (counter % (textures->size() + 1))
				ImGui::SameLine();

			ImGui::ImageButton((ImTextureID)p.second->s_Handle, ImVec2(64, 64), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), 1);
			if(ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text(p.second->s_Name.c_str());
				ImGui::Text(std::to_string(p.second->s_Handle).c_str());
				//ImGui::Text("TagName");
				ImGui::EndTooltip();
			}
			if (ImGui::IsItemClicked())
			{
				// TODO: It is possible to add a map to a material without one.
				// Handle that properly.
				material->SetTexture(p.second, m_CurrentMap);
			}
			counter++;
		}

		ImGui::End();
	}
}