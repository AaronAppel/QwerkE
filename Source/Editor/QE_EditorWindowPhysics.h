#pragma once

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowPhysics : public EditorWindow
		{
		public:
			EditorWindowPhysics(GUID guid) :
				EditorWindow("Physics",
					EditorWindowTypes::Physics,
					guid,
					(EditorWindowFlags)(0))
			{ }

		private:
			void DrawInternal() override
			{
				ImGui::Text("TODO");
				ImGui::Text("Physics collision layers grid");
				ImGui::Text("Physics global, per-project, and per-scene properties/settings like gravity");
				ImGui::Text("Physics global properties like gravity");

                // #NOTE From imgui_demo.cpp line 5383
                const char* column_names[] = { "Layer 0", "Layer 1", "Layer 2", "Layer 3", "Layer 4", "Layer 5", "Layer 6", "Layer 7", "Layer 8", "Layer 9", "Layer 10", "Layer 11", "Layer 12" };
                const int columns_count = IM_ARRAYSIZE(column_names);
                const int rows_count = 12;

                static ImGuiTableFlags table_flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_Hideable | ImGuiTableFlags_HighlightHoveredColumn;
                static ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_AngledHeader | ImGuiTableColumnFlags_WidthFixed;
                static bool bools[columns_count * rows_count] = {}; // Dummy storage selection storage
                static int frozen_cols = 1;
                static int frozen_rows = 2;
                ImGui::CheckboxFlags("_ScrollX", &table_flags, ImGuiTableFlags_ScrollX);
                ImGui::CheckboxFlags("_ScrollY", &table_flags, ImGuiTableFlags_ScrollY);
                ImGui::CheckboxFlags("_NoBordersInBody", &table_flags, ImGuiTableFlags_NoBordersInBody);
                ImGui::CheckboxFlags("_HighlightHoveredColumn", &table_flags, ImGuiTableFlags_HighlightHoveredColumn);
                ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
                ImGui::SliderInt("Frozen columns", &frozen_cols, 0, 2);
                ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
                ImGui::SliderInt("Frozen rows", &frozen_rows, 0, 2);
                ImGui::CheckboxFlags("Disable header contributing to column width", &column_flags, ImGuiTableColumnFlags_NoHeaderWidth);

                const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

                if (ImGui::BeginTable("table_angled_headers", columns_count, table_flags, ImVec2(0.0f, TEXT_BASE_HEIGHT * 1.5f * 12)))
                {
                    ImGui::TableSetupColumn(column_names[0], ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_NoReorder);
                    for (int n = 1; n < columns_count; n++)
                        ImGui::TableSetupColumn(column_names[n], column_flags);
                    ImGui::TableSetupScrollFreeze(frozen_cols, frozen_rows);

                    // #TODO Column header is actually the 1st column item instead.
                    // Need to set a proper header and fix the 1st column missing
                    ImGui::TableAngledHeadersRow(); // Draw angled headers for all columns with the ImGuiTableColumnFlags_AngledHeader flag.
                    ImGui::TableHeadersRow();       // Draw remaining headers and allow access to context-menu and other functions.
                    for (int row = 0; row < rows_count; row++)
                    {
                        ImGui::PushID(row);
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::AlignTextToFramePadding();
                        ImGui::Text("Layer %d", row);
                        for (int column = 1; column < columns_count; column++)
                            if (ImGui::TableSetColumnIndex(column))
                            {
                                ImGui::PushID(column);
                                if (column - 1 == row)
                                {
                                    ImGui::BeginDisabled();
                                }
                                ImGui::Checkbox("", &bools[row * columns_count + column]);
                                if (column - 1 == row)
                                {
                                    ImGui::EndDisabled();
                                }
                                ImGui::PopID();
                            }
                        ImGui::PopID();
                    }
                    ImGui::EndTable();
                }
			}
		};

	}

}