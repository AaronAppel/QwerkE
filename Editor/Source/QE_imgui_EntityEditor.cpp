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

#include "QF_ComponentCamera.h"
#include "QF_ComponentInfo.h"
#include "QF_ComponentLight.h"
#include "QF_ComponentMesh.h"
#include "QF_ComponentScript.h"
#include "QF_ComponentTransform.h"

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
        DrawEntityEditor();

        if (!m_CurrentEntity)
            return;

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

        if (ImGui::Begin("Entity Editor"))
        {
            if (!currentScene || !m_CurrentEntity)
            {
                ImGui::Text("Null current Scene or entity");
                ImGui::End();
                return;
            }

            // #TODO Nicer looking '+/-' buttons : https://youtu.be/oESRecjuLNY?t=1787

            //// Begin drawing entity data...
            // Draw generic GameObject data like transform and name
            // std::string name = m_CurrentEntity->GetName().c_str() + ' '; // extra space for editing
            char* entityName = (char*)m_CurrentEntity.EntityName();
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x / 1.6f);
            if (ImGui::InputText("##EntityName", entityName, strlen(entityName)))
            {
            }
            ImGui::SameLine();
            ImGui::Text(std::to_string(m_CurrentEntity.EntityGuid()).c_str());
            ImGui::PopItemWidth();

            ImGui::Separator();

            static bool showComponentSelector = false;
            const char* enabledState = m_CurrentEntity.IsEnabled() ? "Deactivate" : "Activate";
            if (ImGui::Button(enabledState))
            {
                m_CurrentEntity.SetIsEnabled(!m_CurrentEntity.IsEnabled());
            }
            ImGui::SameLine();
            if (ImGui::Button("Add Component"))
            {
                ImGui::OpenPopup("ComponentList");
            }

            if (ImGui::BeginPopup("ComponentList"))
            {
                if (!m_CurrentEntity.HasComponent<ComponentScript>() && ImGui::MenuItem("ComponentScript"))
                {
                    ComponentScript& script = m_CurrentEntity.AddComponent<ComponentScript>();
                    script.m_ScriptInstances.insert({ eScriptTypes::Testing, nullptr});
                    script.Bind(m_CurrentEntity);
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }

            // #TOOD Deprecate m_EditComponent if it stays small
            m_EditComponent->Draw(m_CurrentEntity);
        }

        ImGui::End();
#endif
	}

}
