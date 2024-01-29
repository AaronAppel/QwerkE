#include "QE_EntityEditor.h"

#include "imgui/imgui.h"

#include "QF_ConfigHelper.h"
#include "QF_Mesh.h"
#include "QF_Renderable.h"

#include "QF_Defines.h"
#include "QF_Physics.h"
#include "QF_Resources.h"
#include "QF_GameObject.h"
#include "QF_RenderComponent.h"
#include "QF_Bullet3Component.h"
#include "QF_RenderRoutine.h"
#include "QF_Bullet3Routine.h"
#include "QF_Scene.h"
#include "QF_Scenes.h"
#include "QF_b3_PhysicsFactory.h"

#include "QE_EditComponent.h"
#include "QE_Editor.h"

namespace QwerkE {

	EntityEditor::EntityEditor()
	{
        m_EditComponent = new EditComponent();

        if (ConfigHelper::GetConfigData().systems.AudioEnabled)
        {
            m_ListboxItemStrings.push_back("Sound Listener");
            m_ListboxItemStrings.push_back("Sound Player");
            m_ListboxItemTypes.push_back(eComponentTags::Component_SoundListener);
            m_ListboxItemTypes.push_back(eComponentTags::Component_SoundPlayer);
        }
        if (ConfigHelper::GetConfigData().systems.PhysicsEnabled)
        {
            m_ListboxItemStrings.push_back("Physics");
            m_ListboxItemTypes.push_back(eComponentTags::Component_Physics);
        }
	}

	EntityEditor::~EntityEditor()
	{
        delete m_EditComponent;
	}

	void EntityEditor::Draw()
	{
		if (m_CurrentEntity == nullptr)
		{
            auto list = Scenes::GetCurrentScene()->GetObjectList();
            auto begin = list.begin();
            if (begin != list.end())
            {
                m_CurrentEntity = begin->second;
			}
		}

		if (m_CurrentEntity != nullptr)
        {
            DrawEntityEditor();
		}
	}

	void EntityEditor::DrawEntityEditor()
	{
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
                        if (!ConfigHelper::GetConfigData().systems.AudioEnabled)
                            break;
                        break;

                    case eComponentTags::Component_SoundPlayer:
                        if (!ConfigHelper::GetConfigData().systems.AudioEnabled)
                            break;
                        break;

                    case eComponentTags::Component_Render:
                        if (m_CurrentEntity->GetComponent(Component_Render) == nullptr)
                        {
                            Renderable renderable;
                            renderable.SetMaterial(Resources::GetMaterial(null_material));
                            renderable.SetShader(Resources::GetShaderProgram(null_shader));
                            renderable.SetMesh(Resources::GetMesh(null_mesh));
                            static int uniqueId = 0; // #FEATURE F0004 obsidian://open?vault=Documentation&file=Development%2FFeatures%2FFeature%20Tickets%2FF0004
                            // #FEATURE F0003 obsidian://open?vault=Documentation&file=Development%2FFeatures%2FFeature%20Tickets%2FF0003
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

                    case eComponentTags::Component_Physics:
                        if (m_CurrentEntity->GetComponent(Component_Physics) == nullptr)
                        {
                            if (!ConfigHelper::GetConfigData().systems.PhysicsEnabled)
                                break;

                            vec3 position = m_CurrentEntity->GetPosition();
                            vec3 scale = m_CurrentEntity->GetScale();
                            btRigidBody* rigidBody = PhysicsFactory::CreateRigidCube(btVector3(position.x, position.y, position.z), scale.x, scale.y, scale.z, 1.0f);
                            PhysicsComponent* component = new Bullet3Component(rigidBody);

                            if (m_CurrentEntity->AddComponent(component))
                            {
                                Physics::RegisterObject(rigidBody);
                            }
                            else
                            {
                                delete component;
                                delete rigidBody;
                                break;
                            }

                            Bullet3Routine* bulletRoutine = new Bullet3Routine();
                            if (m_CurrentEntity->AddUpdateRoutine(bulletRoutine))
                            {
                                bulletRoutine->Initialize();
                            }
                            else
                            {
                                delete bulletRoutine;
                            }
                        }
                        break;
                    }

                    showComponentSelector = false;
                }
            }

            m_EditComponent->Draw(m_CurrentEntity);
        }

        ImGui::End();
	}

}
