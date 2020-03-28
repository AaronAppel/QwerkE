#include "imgui_MaterialEditor.h"
#include "../QwerkE_Framework/Source/Headers/QwerkE_Enums.h"
#include "../QwerkE_Framework/Source/Core/Graphics/DataTypes/Material.h"
#include "../QwerkE_Framework/Source/Core/Graphics/DataTypes/Texture.h"
#include "../QwerkE_Framework/Source/Core/Resources/Resources.h"

#include <map>

namespace QwerkE {

    MaterialEditor::MaterialEditor()
    {
        m_TextureList = Resources::SeeTextures();
    }

    MaterialEditor::~MaterialEditor()
    {
    }

    void MaterialEditor::Draw(Material* material, bool* isOpen)
    {
        // NOTE: For now just replace existing texture.
        // In the future think of a way to preserve old data.
        if (ImGui::Begin("Material Editor", isOpen))
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
            counter = 0;
            for (auto p : *m_TextureList)
            {
                if (counter % 6)
                    ImGui::SameLine();

                ImGui::ImageButton((ImTextureID)p.second->s_Handle, ImVec2(64, 64), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), 1);
                if (ImGui::IsItemHovered())
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
                    LOG_INFO("Set {0}({1}) mat to map index {2}", p.second->s_Name.c_str(), (unsigned int)p.second->s_Handle, (int)m_CurrentMap);
                }
                counter++;
            }

            ImGui::End();
        }
    }

}
