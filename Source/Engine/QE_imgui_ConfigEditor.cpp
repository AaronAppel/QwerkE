#include "QE_ConfigEditor.h"

#include "../../../QwerkE_Framework/Libraries/imgui/imgui.h"

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
