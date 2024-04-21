#include "QE_EditorWindowStylePicker.h"

#include "QF_Serialization.h"
#include "QF_Settings.h"

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

        void EditorWindowStylePicker::DrawInternal()
        {
            // #TODO Add preset styles to choose and load from file

            ImGuiStyle& style = ImGui::GetStyle();

            if (m_Edited)
            {
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.f, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1.f, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1.f, 0.6f, 0.6f));
            }

            if (ImGui::Button("Save Style"))
            {
                Serialization::SerializeObjectToFile(style, Settings::GetStyleFileName());
            }

            if (m_Edited)
            {
                ImGui::PopStyleColor(3);
            }

            const Mirror::TypeInfo* styleTypeInfo = Mirror::InfoForType<ImGuiStyle>();

            for (size_t i = 0; i < styleTypeInfo->fields.size(); i++)
            {
                // #TODO Look at using the inspector to render widgets for ImGuiStyle members

                const Mirror::Field& field = styleTypeInfo->fields[i];

                if (field.typeInfo->enumType == MirrorTypes::m_float &&
                    strcmp(styleTypeInfo->fields[i].name.c_str(), "Alpha") == 0)
                {
                    ImGui::Text("Alpha");
                    ImGui::SameLine();
                    m_Edited |= ImGui::DragFloat("##Alpha", &style.Alpha, .01f, .01f, 1.f); // #NOTE .01f minimum or ImGui will say End() wasn't called
                    continue;
                }

                // #TODO Edit save if () { edited = true; }

                switch (field.typeInfo->enumType)
                {
                case MirrorTypes::m_float:
                    ImGui::Text(field.name.c_str());
                    // ImGui::SameLine();
                    // ImGui::DragFloat(field.name.c_str(), (float*)((char*)&style + field.offset), 0.1f, 0.f, 1.f);
                    break;

                case MirrorTypes::ImVec2:
                    ImGui::Text(field.name.c_str());
                    // ImGui::SameLine();
                    // ImGui::DragFloat("##WindowPaddingX", &style.WindowPadding.x, 0.1f, 0.f, 1000.f);
                    // ImGui::SameLine();
                    // ImGui::DragFloat("##WindowPaddingY", &style.WindowPadding.y, 0.1f, 0.f, 1000.f);
                    break;

                case MirrorTypes::m_int:
                    ImGui::Text(field.name.c_str());
                    break;

                case MirrorTypes::m_imvec4_array:
                    if (ImGui::CollapsingHeader(styleTypeInfo->fields[i].name.c_str(), ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_SpanAvailWidth))
                    {
                        ImGui::Text("More Info:");

                        ImGui::SameLine();
                        static bool uiOptionsEnabled = false;
                        ImGui::Checkbox("ColourPickerOptionsCheck", &uiOptionsEnabled);

                        ImGui::SameLine();
                        static float uiScalar = .8f;
                        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                        m_Edited |= ImGui::SliderFloat("ColourPickerSlider", &uiScalar, .1f, 1.2f);
                        ImGui::PopItemWidth();

                        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * uiScalar);

                        ImGuiColorEditFlags_ flags = static_cast<ImGuiColorEditFlags_>(
                            ImGuiColorEditFlags_NoSidePreview |
                            ImGuiColorEditFlags_PickerHueWheel |
                            ImGuiColorEditFlags_NoLabel
                            );
                        flags = static_cast<ImGuiColorEditFlags_>(flags | (uiOptionsEnabled ? ImGuiColorEditFlags_None : ImGuiColorEditFlags_NoInputs));

                        const size_t range = ImGuiCol_qw::_size_constant - 1;
                        for (int i = 0; i < range; i++)
                        {
                            ImGui::Text(ENUM_TO_STR(ImGuiCol_qw::_from_index(i)));
                            if (ImGui::ColorPicker4(ENUM_TO_STR(ImGuiCol_qw::_from_index(i)), (float*)&style.Colors[i], flags)) { m_Edited = true; }
                        }

                        ImGui::PopItemWidth();
                    }
                    break;

                default:
                    break;
                }

                if (strcmp(styleTypeInfo->fields[i].name.c_str(), "DisabledAlpha") == 0)
                {
                    ImGui::Text("DisabledAlpha");
                    ImGui::SameLine();
                    m_Edited |= ImGui::DragFloat("##DisabledAlpha", &style.DisabledAlpha, 0.1f, 0.f, 1.f);
                }
            }
        }

	}

}