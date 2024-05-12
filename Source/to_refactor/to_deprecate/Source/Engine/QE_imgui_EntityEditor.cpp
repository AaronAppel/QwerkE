#include "QE_EntityEditor.h"

#include "Libraries/imgui/QC_imgui.h"
#include "Libraries/Mirror/Source/Mirror.h"
#include "Libraries/Bullet3/BulletDynamics/Dynamics/btRigidBody.h" // #TODO Remove Bullet3 dependency from here

#include "QF_Defines.h"
#include "QF_GameObject.h"
#include "QF_Log.h"
#include "QF_Mesh.h"
#include "QF_RenderComponent.h"
#include "QF_Assets.h"
#include "QF_Scene.h"
#include "QF_Scenes.h"
#include "QF_Settings.h"
#include "QF_Renderable.h"
#include "QF_RenderRoutine.h"

#include "QE_Editor.h"
#include "QE_EditorInspector.h"

namespace QwerkE {

	EntityEditor::EntityEditor()
	{
        m_EditComponent = std::make_unique<EditComponent>();

        if (Settings::GetEngineSettings().audioEnabled)
        {
            m_ListboxItemStrings.push_back("Sound Listener");
            m_ListboxItemStrings.push_back("Sound Player");
            m_ListboxItemTypes.push_back(eComponentTags::Component_SoundListener);
            m_ListboxItemTypes.push_back(eComponentTags::Component_SoundPlayer);
        }
	}

	void EntityEditor::Draw()
	{
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
	}

	void EntityEditor::DrawEntityEditor()
	{
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

            static float pos[3] = { 0.0f, 0.0f, 0.0f };
            pos[0] = m_CurrentEntity->GetPosition().x;
            pos[1] = m_CurrentEntity->GetPosition().y;
            pos[2] = m_CurrentEntity->GetPosition().z;

            static float rot[3] = { 0.0f, 0.0f, 0.0f };
            rot[0] = m_CurrentEntity->GetRotation().x;
            rot[1] = m_CurrentEntity->GetRotation().y;
            rot[2] = m_CurrentEntity->GetRotation().z;

            static float scale[3] = { 1.0f, 1.0f, 1.0f };
            scale[0] = m_CurrentEntity->GetScale().x;
            scale[1] = m_CurrentEntity->GetScale().y;
            scale[2] = m_CurrentEntity->GetScale().z;

            if (ImGui::InputFloat3("Pos: ", pos))
            {
                m_CurrentEntity->SetPosition(vec3(pos[0], pos[1], pos[2]));
            }
            if (ImGui::InputFloat3("Rot: ", rot))
            {
                m_CurrentEntity->SetRotation(vec3(rot[0], rot[1], rot[2]));
            }
            if (ImGui::InputFloat3("Scale: ", scale))
            {
                m_CurrentEntity->SetScale(vec3(scale[0], scale[1], scale[2]));
            }

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

            ImGui::SameLine();
            if (ImGui::Button("+Component"))
            {
                showComponentSelector = true;
            }

            if (showComponentSelector)
            {
                static int listbox_item_current = eComponentTags::Component_Null;
                const int itemsHeight = 4;

                if (ImGui::ListBox("+Component", &listbox_item_current, m_ListboxItemStrings.data(), m_ListboxItemStrings.size(), itemsHeight))
                {
                    switch ((int)m_ListboxItemTypes.at(listbox_item_current))
                    {
                    case eComponentTags::Component_SoundListener:
                        if (!Settings::GetEngineSettings().audioEnabled)
                            break;
                        break;

                    case eComponentTags::Component_SoundPlayer:
                        if (!Settings::GetEngineSettings().audioEnabled)
                            break;
                        break;

                    case eComponentTags::Component_Render:
                        if (m_CurrentEntity->GetComponent(Component_Render) == nullptr)
                        {
                            Renderable renderable;
                            renderable.SetMaterial(Assets::GetMaterial(null_material));
                            renderable.SetShader(Assets::GetShaderProgram(null_shader));
                            renderable.SetMesh(Assets::GetMesh(null_mesh));
                            static int uniqueId = 0; // #FEATURE F0004, F0003
                            renderable.SetRenderableName(std::to_string(uniqueId++));

                            RenderComponent* rComp = (RenderComponent*)m_CurrentEntity->GetComponent(Component_Render);

                            if (rComp == nullptr)
                            {
                                rComp = new RenderComponent();
                                m_CurrentEntity->AddComponent(rComp);

                                if (m_CurrentEntity->GetFirstDrawRoutineOfType(Routine_Render) == nullptr)
                                {
                                    m_CurrentEntity->AddDrawRoutine((Routine*) new RenderRoutine());
                                }

                                rComp->AddRenderable(renderable);
                                rComp->GetParent()->GetFirstDrawRoutineOfType(Routine_Render)->Initialize();
                            }
                        }
                        break;
                    }

                    showComponentSelector = false;
                    Scenes::SetCurrentSceneDirty();
                }
            }

            m_EditComponent->Draw(m_CurrentEntity);
        }

        ImGui::End();
	}

}
