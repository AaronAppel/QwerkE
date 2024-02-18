#include "QE_ConfigEditor.h"

#include "Libraries/imgui/QC_imgui.h"
#include "Libraries/Mirror/Source/Mirror.h"

#include "QF_Settings.h"

#include "QE_EditorInspector.h"

namespace QwerkE {

    void ConfigEditor::Draw()
    {
        if (!ImGui::Begin("Inspect Project Settings"))
            return;

        std::string buffer = "";
        buffer.reserve(200);

        ImGui::PushItemWidth(ImGui::GetWindowWidth() / 2.5f);
        Inspector::InspectFieldRecursive(Mirror::InfoForClass<ProjectSettings>(), &Settings::GetProjectSettings(), buffer);
        ImGui::PopItemWidth();

        ImGui::End();
    }

}
