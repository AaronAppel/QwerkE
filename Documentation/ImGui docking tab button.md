The ImGui docking tab tray has 2 buttons on the right side to collapse and close tab windows.
The code to handle that is in:

imgui.cpp line 17365
 // Docking/Collapse button
 if (has_window_menu_button)
 
and

imgui_widgets.cpp line 850
bool ImGui::CollapseButton(ImGuiID id, const ImVec2& pos, ImGuiDockNode* dock_node)

Just copy and add more entries there for custom functionality.