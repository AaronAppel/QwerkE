#include "QE_EditorWindowStylePicker.h"

#include "QF_Serialize.h"

#include "QE_EditorWindow.h"
#include "QE_Settings.h"

#include "QE_Mirror.h"

namespace QwerkE {

	namespace Editor {

        // void local_SetCodedStyle();
        void SetupImGuiStyle();

        void local_UpdateStyleFilesList(std::vector<const char*>& styleFiles, int& currentIndex)
        {
            for (size_t i = 0; i < styleFiles.size(); i++)
            {
                delete[] styleFiles[i];
            }
            styleFiles.clear();

            std::filesystem::directory_iterator dirIterator;
            dirIterator = Directory::ListDir(Paths::StylesDir().c_str());

            const char* currentStyleFileName = Settings::GetStyleFileName();
            for (const auto& directoryEntry : dirIterator)
            {
                const Path& path = directoryEntry.path();
                if (strcmp(path.extension().generic_string().c_str() + 1, style_file_ext))
                {
                    continue;
                }

                styleFiles.emplace_back(strdup(path.filename().u8string().c_str()));
                if (currentStyleFileName)
                {
                    if (strcmp(styleFiles[styleFiles.size() - 1], currentStyleFileName) == 0)
                    {
                        currentIndex = styleFiles.size() - 1;
                    }
                }
            }

            if (currentStyleFileName && styleFiles.empty())
            {
                styleFiles.emplace_back(currentStyleFileName);
            }
        }

        void EditorWindowStylePicker::DrawInternal()
        {
            // #TODO Add preset styles to choose and load from file

            ImGuiStyle& style = ImGui::GetStyle();

            if (ImGui::Button("Load Style From File"))
            {
                const std::string styleFileName = Files::ExplorerOpen("Style file (*.style)\0*.style\0", Paths::StylesDir().c_str());
                Serialize::FromFile(styleFileName.c_str(), style);
                m_Edited = false;
            }
            ImGui::SameLine();
            if (ImGui::Button("Load Coded Style"))
            {
                // local_SetCodedStyle();
                SetupImGuiStyle();
                m_Edited = false;
            }

            ImGui::SameLine();
            if (m_StyleFiles.empty())
            {
                local_UpdateStyleFilesList(m_StyleFiles, m_CurrentStyleFileIndex);
            }
            ImGui::PushItemWidth(250.f);
            if (ImGui::Combo("Styles##StylePickerCombo", &m_CurrentStyleFileIndex, m_StyleFiles.data(), (int)m_StyleFiles.size()))
            {
                Serialize::FromFile(Paths::Style(m_StyleFiles[m_CurrentStyleFileIndex]).c_str(), style);
                m_Edited = false;
            }
            ImGui::PopItemWidth();

            if (m_Edited)
            {
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.f, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1.f, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1.f, 0.6f, 0.6f));
            }

            if (ImGui::Button("Save Style Current Style"))
            {
                Serialize::ToFile(style, Paths::Style(Settings::GetStyleFileName()).c_str());
                if (m_Edited)
                {
                    m_Edited = false;
                    ImGui::PopStyleColor(3);
                }
            }

            ImGui::SameLine();
            if (ImGui::Button("Save Style To File"))
            {
                const std::string styleFilePath = Files::ExplorerSave("Style file (*.style)\0*.style\0", Paths::StylesDir().c_str());
                if (!styleFilePath.empty())
                {
                    if (!Files::Exists(styleFilePath.c_str()))
                    {
                        Files::CreateEmptyFile(styleFilePath.c_str());
                    }
                    Serialize::ToFile(style, styleFilePath.c_str());
                    if (m_Edited)
                    {
                        m_Edited = false;
                        ImGui::PopStyleColor(3);
                    }
                }
            }

            if (ImGui::CollapsingHeader("Edit Current Style", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_SpanAvailWidth))
            {
                if (m_Edited)
                {
                    ImGui::PopStyleColor(3);
                }

                ImGui::Text("More Info:");
                ImGui::SameLine();
                ImGui::Checkbox("##MoreInfoCheckbox", &m_ShowMoreInfo);

                ImGui::SameLine();
                ImGui::PushItemWidth(150.f);
                ImGui::Text("UI Scale:");
                ImGui::SameLine();
                ImGui::SliderFloat("##UIScaleSlider", &m_UiScaling, .1f, 1.2f);
                ImGui::Text("Font:");
                ImGui::SliderFloat("##FontScaleSlider", &m_FontScale, 0.1f, 3.f);
                ImGui::PopItemWidth();
                ImGui::Separator();

                ImGui::Text("Settings:");

                ImGui::PushItemWidth((ImGui::GetContentRegionAvail().x / (5 * m_UiScaling)));
                ImGui::SetWindowFontScale(m_FontScale);

                const Mirror::TypeInfo* styleTypeInfo = Mirror::InfoForType<ImGuiStyle>();

                bool drawSettings = false;

                if (ImGui::CollapsingHeader("Settings", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_SpanAvailWidth))
                {
                    drawSettings = true;
                }

                for (size_t i = 0; i < styleTypeInfo->fields.size(); i++)
                {
                    // #TODO Look at adding widgets for ImGuiStyle members
                    // #TODO Add columns to align text with value better.
                    // Can standardize column width
                    // In QF_EditorWindowFolderViewer.h
                    // ImGui::Columns(columnCount, 0, false);
                    // ...
                    // ImGui::NextColumn();

                    const Mirror::Field& field = styleTypeInfo->fields[i];

                    if (drawSettings && field.typeInfo->id == Mirror::TypeId<float>() &&
                        strcmp(styleTypeInfo->fields[i].name.c_str(), "Alpha") == 0)
                    {
                        ImGui::Text("Alpha");
                        ImGui::SameLine();
                        m_Edited |= ImGui::DragFloat("##Alpha", &style.Alpha, .01f, .01f, 1.f); // #NOTE .01f minimum or ImGui will say End() wasn't called
                        continue;
                    }

                    // #TODO Edit save if () { edited = true; }

                    switch (field.typeInfo->id)
                    {
                    case Mirror::TypeId<float>():
                        if (!drawSettings) continue;
                        if (i % 2)
                        {
                            ImGui::SameLine();
                        }
                        ImGui::Text(field.name.c_str());
                        ImGui::SameLine();
                        ImGui::DragFloat(("##" + field.name).c_str(), (float*)((char*)&style + field.offset), 0.1f);
                        break;

                    case Mirror::TypeId<ImVec2>():
                        if (!drawSettings) continue;
                        if (i % 2)
                        {
                            ImGui::SameLine();
                        }
                        ImGui::Text(field.name.c_str());
                        ImGui::SameLine();
                        ImGui::DragFloat2(("##" + field.name).c_str(), (float*)((char*)&style + field.offset), 0.1f);
                        break;

                    case Mirror::TypeId<int>():
                        if (!drawSettings) continue;
                        if (i % 2)
                        {
                            ImGui::SameLine();
                        }
                        ImGui::Text(field.name.c_str());
                        break;

                    case Mirror::TypeId<ImVec4[ImGuiCol_COUNT]>():
                        if (ImGui::CollapsingHeader("Colors", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_SpanAvailWidth))
                        {
                            ImGuiColorEditFlags_ colourPickerFlags = static_cast<ImGuiColorEditFlags_>(
                                ImGuiColorEditFlags_NoAlpha |
                                ImGuiColorEditFlags_NoSidePreview |
                                ImGuiColorEditFlags_PickerHueWheel |
                                ImGuiColorEditFlags_NoLabel
                                );
                            colourPickerFlags = static_cast<ImGuiColorEditFlags_>(colourPickerFlags | (m_ShowMoreInfo ? ImGuiColorEditFlags_None : ImGuiColorEditFlags_NoInputs));

                            const float availableWindowWidth = ImGui::GetContentRegionAvail().x;
                            const size_t range = ImGuiCol_qw::_size_constant - 1;

                            constexpr u8 pickersPerRow = 7;
                            const float scalar = 1.02 * m_UiScaling; // #NOTE Slightly shrink to fit

                            int popUpSelectedItemIndex = 0;
                            for (int j = 0; j < range; j++)
                            {
                                if (j % pickersPerRow)
                                {
                                    ImGui::SameLine();
                                }

                                ImGui::PushItemWidth(availableWindowWidth / (pickersPerRow * scalar));
                                if (ImGui::ColorPicker4(ENUM_TO_STR(ImGuiCol_qw::_from_index(j)), (float*)&style.Colors[j], colourPickerFlags))
                                {
                                    m_Edited = true;
                                }
                                if (ImGui::IsItemHovered())
                                {
                                    if (ImGui::BeginItemTooltip())
                                    {
                                        ImGui::Text(ENUM_TO_STR(ImGuiCol_qw::_from_index(j)));
                                        ImGui::End();
                                    }
                                }
                                if (ImGui::IsItemClicked(ImGui::MouseRight))
                                {
                                    ImGui::OpenPopup("ColorPickerCopyPaste");
                                    popUpSelectedItemIndex = j;
                                }
                            }

                            if (ImGui::BeginPopup("ColorPickerCopyPaste"))
                            {
                                // #TODO Implement when less tired
                                // if (ImGui::Button("Copy"))
                                // {
                                //     std::string numberAsString;
                                //     numberAsString.reserve(32);
                                //     numberAsString.append(std::to_string(style.Colors[popUpSelectedItemIndex].x));
                                //     numberAsString.append(std::to_string(style.Colors[popUpSelectedItemIndex].y));
                                //     numberAsString.append(std::to_string(style.Colors[popUpSelectedItemIndex].z));
                                //     numberAsString.append(std::to_string(style.Colors[popUpSelectedItemIndex].w));
                                //     ImGui::SetClipboardText(numberAsString.c_str());
                                //     LOG_INFO("{0} Copied to clipboard: {1}", __FUNCTION__, numberAsString.c_str());
                                // }
                                // if (ImGui::Button("Paste"))
                                // {
                                //     std::string clipBoardText = ImGui::GetClipboardText();
                                //     LOG_INFO("{0} Pasting from clipboard: {1}", __FUNCTION__, clipBoardText.c_str());
                                //     memcpy(&style.Colors[popUpSelectedItemIndex], clipBoardText.data(), sizeof(float) * 4);
                                //     m_Edited = true;
                                // }
                                // ImGui::EndPopup();
                            }
                        }
                        break;

                    default:
                        break;
                    }
                }

                ImGui::SetWindowFontScale(1.f);
                ImGui::PopItemWidth();
            }
        }

        // void local_SetCodedStyle
        void SetupImGuiStyle()
        {
            // BlackDevil style by Naeemullah1 from ImThemes
            ImGuiStyle& style = ImGui::GetStyle();

            style.Alpha = 1.0f;
            style.DisabledAlpha = 0.5f;
            style.WindowPadding = ImVec2(10.0f, 10.0f);
            style.WindowRounding = 5.0f;
            style.WindowBorderSize = 1.0f;
            style.WindowMinSize = ImVec2(20.0f, 20.0f);
            style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
            style.WindowMenuButtonPosition = ImGuiDir_Left;
            style.ChildRounding = 5.0f;
            style.ChildBorderSize = 1.0f;
            style.PopupRounding = 5.0f;
            style.PopupBorderSize = 1.0f;
            style.FramePadding = ImVec2(5.0f, 5.0f);
            style.FrameRounding = 5.0f;
            style.FrameBorderSize = 0.0f;
            style.ItemSpacing = ImVec2(8.0f, 5.0f);
            style.ItemInnerSpacing = ImVec2(5.0f, 5.0f);
            style.CellPadding = ImVec2(5.0f, 5.0f);
            style.IndentSpacing = 10.0f;
            style.ColumnsMinSpacing = 5.0f;
            style.ScrollbarSize = 10.0f;
            style.ScrollbarRounding = 5.0f;
            style.GrabMinSize = 10.0f;
            style.GrabRounding = 0.0f;
            style.TabRounding = 10.0f;
            style.TabBorderSize = 0.0f;
            style.TabMinWidthForCloseButton = 0.0f;
            style.ColorButtonPosition = ImGuiDir_Right;
            style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
            style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

            style.Colors[ImGuiCol_Text] = ImVec4(0.7843137383460999f, 0.7843137383460999f, 0.7843137383460999f, 1.0f);
            style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.43529412150383f, 0.407843142747879f, 0.3137255012989044f, 1.0f);
            style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05882352963089943f, 0.1176470592617989f, 0.05882352963089943f, 0.3921568691730499f);
            style.Colors[ImGuiCol_ChildBg] = ImVec4(0.05882352963089943f, 0.1176470592617989f, 0.1568627506494522f, 0.7843137383460999f);
            style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.7843137383460999f);
            style.Colors[ImGuiCol_Border] = ImVec4(0.3921568691730499f, 0.0f, 0.0f, 0.7843137383460999f);
            style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
            style.Colors[ImGuiCol_FrameBg] = ImVec4(0.05882352963089943f, 0.1176470592617989f, 0.1568627506494522f, 0.7843137383460999f);
            style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1176470592617989f, 0.2352941185235977f, 0.3529411852359772f, 0.7843137383460999f);
            style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.3529411852359772f, 0.3529411852359772f, 0.1176470592617989f, 0.7843137383460999f);
            style.Colors[ImGuiCol_TitleBg] = ImVec4(0.05882352963089943f, 0.1176470592617989f, 0.1568627506494522f, 0.7843137383460999f);
            style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.05882352963089943f, 0.1176470592617989f, 0.1568627506494522f, 0.7843137383460999f);
            style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.05882352963089943f, 0.1176470592617989f, 0.1568627506494522f, 0.196078434586525f);
            style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.7843137383460999f);
            style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05882352963089943f, 0.1176470592617989f, 0.1568627506494522f, 0.7843137383460999f);
            style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1176470592617989f, 0.3529411852359772f, 0.2352941185235977f, 0.7843137383460999f);
            style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1176470592617989f, 0.3529411852359772f, 0.3529411852359772f, 0.7843137383460999f);
            style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.1176470592617989f, 0.5882353186607361f, 0.2352941185235977f, 0.7843137383460999f);
            style.Colors[ImGuiCol_CheckMark] = ImVec4(0.1176470592617989f, 0.5882353186607361f, 0.2352941185235977f, 0.7843137383460999f);
            style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.1176470592617989f, 0.3529411852359772f, 0.2352941185235977f, 0.7843137383460999f);
            style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.1176470592617989f, 0.5882353186607361f, 0.2352941185235977f, 0.7843137383460999f);
            style.Colors[ImGuiCol_Button] = ImVec4(0.3529411852359772f, 0.3529411852359772f, 0.1176470592617989f, 0.7843137383460999f);
            style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3529411852359772f, 0.4705882370471954f, 0.2352941185235977f, 0.7843137383460999f);
            style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.5882353186607361f, 0.3529411852359772f, 0.2352941185235977f, 0.7843137383460999f);
            style.Colors[ImGuiCol_Header] = ImVec4(0.05882352963089943f, 0.1176470592617989f, 0.1568627506494522f, 0.7843137383460999f);
            style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.1176470592617989f, 0.3529411852359772f, 0.3529411852359772f, 0.7843137383460999f);
            style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.1176470592617989f, 0.5882353186607361f, 0.2352941185235977f, 0.7843137383460999f);
            style.Colors[ImGuiCol_Separator] = ImVec4(0.3529411852359772f, 0.3529411852359772f, 0.2352941185235977f, 0.7843137383460999f);
            style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1176470592617989f, 0.3529411852359772f, 0.3529411852359772f, 0.7843137383460999f);
            style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.5882353186607361f, 0.3529411852359772f, 0.2352941185235977f, 0.7843137383460999f);
            style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.05882352963089943f, 0.1176470592617989f, 0.1568627506494522f, 0.7843137383460999f);
            style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.5882353186607361f, 0.3529411852359772f, 0.3529411852359772f, 0.7843137383460999f);
            style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.5882353186607361f, 0.2352941185235977f, 0.2352941185235977f, 0.7843137383460999f);
            style.Colors[ImGuiCol_Tab] = ImVec4(0.3529411852359772f, 0.3529411852359772f, 0.1176470592617989f, 0.7843137383460999f);
            style.Colors[ImGuiCol_TabHovered] = ImVec4(0.3529411852359772f, 0.4705882370471954f, 0.2352941185235977f, 0.7843137383460999f);
            style.Colors[ImGuiCol_TabActive] = ImVec4(0.5882353186607361f, 0.3529411852359772f, 0.2352941185235977f, 0.7843137383460999f);
            style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.05882352963089943f, 0.1176470592617989f, 0.1568627506494522f, 0.7843137383460999f);
            style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.5882353186607361f, 0.3529411852359772f, 0.3529411852359772f, 0.7843137383460999f);
            style.Colors[ImGuiCol_PlotLines] = ImVec4(0.3921568691730499f, 0.7843137383460999f, 0.3921568691730499f, 0.7843137383460999f);
            style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.4274509847164154f, 0.3490196168422699f, 0.7843137383460999f);
            style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.0f, 0.3529411852359772f, 0.3921568691730499f, 0.7843137383460999f);
            style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.196078434586525f, 0.5882353186607361f, 0.5882353186607361f, 0.7843137383460999f);
            style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 0.7843137383460999f);
            style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 0.7843137383460999f);
            style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 0.7843137383460999f);
            style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.7843137383460999f);
            style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
            style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.3921568691730499f, 0.3529411852359772f, 0.3921568691730499f, 0.3921568691730499f);
            style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
            style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
            style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
            style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
            style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
        }

	}

}