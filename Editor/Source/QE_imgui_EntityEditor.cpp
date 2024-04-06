#include "QE_EntityEditor.h"

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QC_imgui.h"
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/Mirror.h"
#endif

#include "QF_Log.h"
#include "QF_PathDefines.h"
#include "QF_Scene.h"
#include "QF_Scenes.h"
#include "QF_Settings.h"

#include "QE_Editor.h"
#include "QE_EditorInspector.h"

namespace QwerkE {

	EntityEditor::EntityEditor()
	{
#ifdef _QDEARIMGUI
        m_CurrentEntity = EntityHandle::InvalidHandle();
        m_EditComponent = std::make_unique<EditComponent>();
#endif
	}

	void EntityEditor::Draw()
	{
#ifdef _QDEARIMGUI
        if (!m_CurrentEntity)
            return;

        DrawEntityEditor();

        if (const bool entityChanged = Inspector::InspectObject(m_CurrentEntity, "Inspect Scene Object"))
        {
            Scenes::SetCurrentSceneDirty();
        }
#endif
	}

	void EntityEditor::DrawEntityEditor()
	{
#ifdef _QDEARIMGUI
        Scene* currentScene = Scenes::GetCurrentScene();
        if (!currentScene)
            return;

        if (ImGui::Begin("Entity Editor"))
        {
            // #TODO Nicer looking '+/-' buttons : https://youtu.be/oESRecjuLNY?t=1787

            //// Begin drawing entity data...
            // Draw generic GameObject data like transform and name
            // std::string name = m_CurrentEntity->GetName().c_str() + ' '; // extra space for editing
            char* entityName = (char*)m_CurrentEntity.EntityName();
            if (ImGui::InputText("Name: ", entityName, strlen(entityName)))
            {
            }

            ImGui::Separator();

            static bool showComponentSelector = false;
            const char* enabledState = m_CurrentEntity.IsEnabled() ? "Deactivate" : "Activate";
            if (ImGui::Button(enabledState))
            {
                m_CurrentEntity.SetIsEnabled(!m_CurrentEntity.IsEnabled());
            }

            m_EditComponent->Draw(m_CurrentEntity);
        }

        ImGui::End();
#endif
	}

}
