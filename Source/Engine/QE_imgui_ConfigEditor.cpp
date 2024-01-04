#include "QE_ConfigEditor.h"

#include "imgui/imgui.h"

namespace QwerkE {

    void ConfigEditor::Draw()
    {
        if (ImGui::BeginMenu("ConfigEditor"))
        {
            if (ImGui::CollapsingHeader("Framework", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_CollapsingHeader))
            {
                // #TODO
            }
            ImGui::EndMenu();
        }
    }

}
