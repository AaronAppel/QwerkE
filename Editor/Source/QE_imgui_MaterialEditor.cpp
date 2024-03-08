#include "QE_MaterialEditor.h"

#include <map>

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QC_imgui.h"
#endif

#include "QF_Log.h"

#include "QF_Enums.h"
#include "QF_Material.h"
#include "QF_Texture.h"
#include "QF_Assets.h"

namespace QwerkE {

    MaterialEditor::MaterialEditor()
    {
        m_TextureList = &Assets::SeeTextures();
    }

    const int s_PixelsPerChar = 8;
#ifdef _QDEARIMGUI
    const ImVec2 s_buttonSize = ImVec2(64, 64);
#else
    const vec2 s_buttonSize = vec2(64, 64);
#endif

    void MaterialEditor::Draw(Material* material, bool* isOpen)
    {
#ifdef _QDEARIMGUI
        // NOTE: For now just replace existing texture. In the future, think of a way to preserve old data.
        if (ImGui::Begin("Material Editor", isOpen))
        {
            const std::map<eMaterialMaps, Texture*>* textures = material->SeeMaterials();
            ImGui::Text(material->GetMaterialName().c_str());

            const char* res = ENUM_TO_STR(eMaterialMaps::_from_index(m_CurrentMap));
            ImGui::SameLine(ImGui::GetWindowWidth() - (strlen(res) * s_PixelsPerChar));
            ImGui::Text(res);

            ImGui::Separator();

            int counter = 0;
            const int imagesPerRow = 4;

            for (auto p : *textures)
            {
                if (counter > 0 && counter % imagesPerRow) // #TODO Review counter comparison
                {
                    ImGui::SameLine();
                }

                ImGui::Text(std::to_string((int)p.first).c_str());

                if (p.second)
                {
                    ImGui::SameLine();
                    float scalar = 1.f;
                    if (m_CurrentMap == p.first)
                    {
                        scalar = 1.1f;
                    }

                    ImGui::ImageButton("ID0", (ImTextureID)p.second->s_Handle, s_buttonSize * scalar, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f)); // #TODO Had to remove deprecated padding value of 1

                    // #TODO Add the ability to add or remove textures from a material.

                    if (ImGui::IsItemHovered())
                    {
                        ImGui::BeginTooltip();
                        ImGui::Text(p.second->s_FileName.c_str());
                        ImGui::Text(std::to_string(p.second->s_Handle).c_str());
                        //ImGui::Text("TagName");
                        ImGui::EndTooltip();
                    }

                    if (ImGui::IsItemClicked())
                    {
                        m_CurrentMap = p.first;
                    }
                }
                counter++;
            }

            ImGui::Separator();

            counter = 0;
            for (auto p : *m_TextureList)
            {
                if (counter > 0 && counter % imagesPerRow) // #TODO Review counter comparison
                {
                    ImGui::SameLine();
                }

                if (ImGui::ImageButton("ID1", (ImTextureID)p.second->s_Handle, s_buttonSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f))) // #TODO Had to remove deprecated padding value of 1
                {
                }

                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    ImGui::Text(p.second->s_FileName.c_str());
                    ImGui::Text(std::to_string(p.second->s_Handle).c_str());
                    ImGui::EndTooltip();
                }

                if (ImGui::IsItemClicked())
                {
                    // #TODO It is possible to add a map to a material without one?
                    material->SetTexture(p.second, m_CurrentMap);
                    LOG_INFO("Set {0}({1}) mat to map index {2}", p.second->s_FileName.c_str(), (unsigned int)p.second->s_Handle, (int)m_CurrentMap);
                }
                counter++;
            }

            ImGui::End();
        }
#endif
    }

}
