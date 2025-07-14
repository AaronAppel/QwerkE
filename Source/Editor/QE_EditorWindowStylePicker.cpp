#include "QE_EditorWindowStylePicker.h"

#include "QF_Serialize.h"

#include "QE_EditorWindow.h"
#include "QE_Settings.h"

#include "QE_Mirror.h"

namespace QwerkE {

	namespace Editor {

        void local_LoadCodedImGuiStyle(); // #NOTE Kept for future convenience

        static void local_CleanUpStyleFiles(std::vector<const char*>& styleFiles)
        {
            for (size_t i = 0; i < styleFiles.size(); i++)
            {
                delete[] styleFiles[i];
            }
            styleFiles.clear();
        }

        static void local_UpdateStyleFilesList(std::vector<const char*>& styleFiles, int& currentIndex)
        {
            local_CleanUpStyleFiles(styleFiles);

            styleFiles = Directory::ListFiles(Paths::StylesDir().c_str(), style_file_ext, true);

            if (const char* currentStyleFileName = Settings::GetStyleFileName())
            {
                for (size_t i = 0; i < styleFiles.size(); i++)
                {
                    if (strcmp(styleFiles[i], currentStyleFileName) == 0)
                    {
                        currentIndex = i;
                    }
                }

                if (currentStyleFileName && styleFiles.empty())
                {
                    styleFiles.emplace_back(currentStyleFileName);
                }
            }
        }

        EditorWindowStylePicker::~EditorWindowStylePicker()
        {
            local_CleanUpStyleFiles(m_StyleFiles);
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
            if (ImGui::Button("Load Style From Code"))
            {
                local_LoadCodedImGuiStyle();
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

                    if (drawSettings && field.typeInfo->id == Mirror::IdForType<float>() &&
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
                    case Mirror::IdForType<float>():
                        if (!drawSettings) continue;
                        if (i % 2)
                        {
                            ImGui::SameLine();
                        }
                        ImGui::Text(field.name.c_str());
                        ImGui::SameLine();
                        ImGui::DragFloat(("##" + field.name).c_str(), (float*)((char*)&style + field.offset), 0.1f);
                        break;

                    case Mirror::IdForType<ImVec2>():
                        if (!drawSettings) continue;
                        if (i % 2)
                        {
                            ImGui::SameLine();
                        }
                        ImGui::Text(field.name.c_str());
                        ImGui::SameLine();
                        ImGui::DragFloat2(("##" + field.name).c_str(), (float*)((char*)&style + field.offset), 0.1f);
                        break;

                    case Mirror::IdForType<int>():
                        if (!drawSettings) continue;
                        if (i % 2)
                        {
                            ImGui::SameLine();
                        }
                        ImGui::Text(field.name.c_str());
                        break;

                    case Mirror::IdForType<ImVec4[ImGuiCol_COUNT]>():
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

                            static int popUpSelectedItemIndex = 0;
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
                                ImGui::PopItemWidth();
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
                                if (ImGui::Button("Copy"))
                                {
                                    std::string numberAsString;
                                    numberAsString.reserve(32);
                                    // #TODO Add header to clipboard for copy/paste targets? Need sanitization logic
                                    numberAsString.append(std::to_string(style.Colors[popUpSelectedItemIndex].x) + ","); // #NOTE ',' delimiter
                                    numberAsString.append(std::to_string(style.Colors[popUpSelectedItemIndex].y) + ",");
                                    numberAsString.append(std::to_string(style.Colors[popUpSelectedItemIndex].z) + ",");
                                    numberAsString.append(std::to_string(style.Colors[popUpSelectedItemIndex].w));
                                    ImGui::SetClipboardText(numberAsString.c_str());
                                    LOG_INFO("{0} Copied index {1} to clipboard: {2}", __FUNCTION__, popUpSelectedItemIndex, numberAsString.c_str());
                                }
                                if (ImGui::Button("Paste"))
                                {
                                    std::string clipBoardText = ImGui::GetClipboardText();
                                    if (clipBoardText.size() >= 16 && clipBoardText.size() < 90) // #TODO Find maximum valid size ~80
                                    {
                                        LOG_INFO("{0} Pasting index {1} from clipboard: {2}", __FUNCTION__, popUpSelectedItemIndex, clipBoardText.c_str());

                                        const float x = std::stof(clipBoardText);

                                        u8 stride = clipBoardText.find_first_of(",") + 1;
                                        clipBoardText = clipBoardText.substr(stride);
                                        const float y = std::stof(clipBoardText);

                                        stride = clipBoardText.find_first_of(",") + 1;
                                        clipBoardText = clipBoardText.substr(stride);
                                        const float z = std::stof(clipBoardText);

                                        stride = clipBoardText.find_first_of(",") + 1;
                                        clipBoardText = clipBoardText.substr(stride);
                                        const float w = std::stof(clipBoardText);

                                        style.Colors[popUpSelectedItemIndex].x = x;
                                        style.Colors[popUpSelectedItemIndex].y = y;
                                        style.Colors[popUpSelectedItemIndex].z = z;
                                        style.Colors[popUpSelectedItemIndex].w = w;

                                        m_Edited = true;
                                    }
                                }
                                ImGui::EndPopup();
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

        void local_LoadCodedImGuiStyle() // #NOTE Kept for future convenience
        {
            ImGuiStyle& style = ImGui::GetStyle();

            style.WindowRounding = 0;
            style.FrameRounding = 2.5f;
            style.ScrollbarRounding = 0;

            ImVec4* colors = style.Colors;
            // colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
            // ... Adjust other values here
        }

	}

}