#include "QE_EntityEditor.h"

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QC_imgui.h"
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/Mirror.h"
#endif

#include "QF_GameObject.h"
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
        m_EditComponent = std::make_unique<EditComponent>();
#endif
	}

	void EntityEditor::Draw()
	{
#ifdef _QDEARIMGUI
        GameObject* currentEntity = nullptr;
        if (m_CurrentEntity)
        {
            currentEntity = m_CurrentEntity;
        }
        else if (Scene* currentScene = Scenes::GetCurrentScene())
        {
            const std::map<std::string, GameObject*>& list = currentScene->GetObjectList();
            auto begin = list.begin(); // #TODO Review logic
            if (begin != list.end())
            {
                m_CurrentEntity = begin->second;
            }
        }

		if (currentEntity != nullptr)
        {
            DrawEntityEditor();
            if (m_CurrentEntity)
            {
                if (const bool entityChanged = Inspector::InspectObject(*m_CurrentEntity, "Inspect Scene Object"))
                {
                    Scenes::SetCurrentSceneDirty();
                }
            }
            else
            {
                // #TODO Draw null error text
            }
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
            //// Begin drawing entity data...
            // Draw generic GameObject data like transform and name
            // std::string name = m_CurrentEntity->GetName().c_str() + ' '; // extra space for editing
            if (ImGui::InputText("Name: ", (char*)m_CurrentEntity->GetName().c_str(), m_CurrentEntity->GetName().size()))
            {

            }
            // m_CurrentEntity->SetName(); // #TODO Scene map probably needs to handle name changes

            ImGui::Separator();

            static bool showComponentSelector = false;
            const char* enabledState = m_CurrentEntity->Enabled() ? "Deactivate" : "Activate";
            if (ImGui::Button(enabledState))
            {
                if (m_CurrentEntity->Enabled())
                {
                    m_CurrentEntity->Deactivate();
                }
                else
                {
                    m_CurrentEntity->Activate();
                }
            }

            m_EditComponent->Draw(m_CurrentEntity);
        }

        ImGui::End();
#endif
	}

}
