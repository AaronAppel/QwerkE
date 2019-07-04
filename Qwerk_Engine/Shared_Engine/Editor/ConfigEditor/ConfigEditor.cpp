#include "ConfigEditor.h"

void ConfigEditor::Draw()
{
    if (ImGui::BeginMenu("ConfigEditor"))
    {
        if (ImGui::CollapsingHeader("Framework", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_CollapsingHeader))
        {

        }
        ImGui::EndMenu();
    }
}