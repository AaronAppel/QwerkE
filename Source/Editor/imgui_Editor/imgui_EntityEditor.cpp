#include "../EntityEditor.h"
#include "../EditComponent.h"
#include "../Editor.h"

#include "../QwerkE_Framework/Libraries/imgui/imgui.h"
#include "../QwerkE_Framework/Source/Core/Scenes/Scene.h"
#include "../QwerkE_Framework/Source/Core/Resources/Resources.h"
#include "../QwerkE_Framework/Source/Core/Scenes/Entities/GameObject.h"
#include "../QwerkE_Framework/Source/Core/Scenes/Entities/Components/RenderComponent.h"
#include "../QwerkE_Framework/Source/Core/Scenes/Entities/Components/Extended/Bullet3Component.h"
#include "../QwerkE_Framework/Source/Core/Scenes/Entities/Routines/RenderRoutine.h"
#include "../QwerkE_Framework/Source/Core/Scenes/Entities/Routines/Extended/Bullet3Routine.h"
#include "../QwerkE_Framework/Source/Core/Graphics/Mesh/Mesh.h"
#include "../QwerkE_Framework/Source/Core/Graphics/DataTypes/Renderable.h"
#include "../QwerkE_Framework/Source/Extended/Bullet3/b3_PhysicsFactory.h"

#include <map>
#include <string>

namespace QwerkE {

	EntityEditor::EntityEditor()
	{
        m_EditComponent = new EditComponent();
	}

	EntityEditor::~EntityEditor()
	{
        delete m_EditComponent;
	}

	// ImGui styling: https://www.unknowncheats.me/forum/direct3d/189635-imgui-style-settings.html
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
            ImGui::InputText("Name: ", (char*)m_CurrentEntity->GetName().c_str(), m_CurrentEntity->GetName().size());
            // m_CurrentEntity->SetName(); // TODO: Scene map probably needs to handle name changes

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
                const char* listbox_items[] = { "Renderable", "Physics" };
                static int listbox_item_current = 1;

                if (ImGui::ListBox("+Component", &listbox_item_current, listbox_items, IM_ARRAYSIZE(listbox_items), 4))
                {
                    switch (listbox_item_current)
                    {
                    case 0:
                        if (m_CurrentEntity->GetComponent(Component_Render) == nullptr)
                        {
                            Renderable renderable;
                            renderable.SetMaterial(Resources::GetMaterial(null_material));
                            renderable.SetShader(Resources::GetShaderProgram(null_shader));
                            renderable.SetMesh(Resources::GetMesh(null_mesh));
                            static int uniqueId = 0; // TODO: Implement GUIDs
                            // NOTE: Currently if 2 or more renderables have the same name, you will not be able to select any one except the first in editor
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
                    case 1:
                        if (m_CurrentEntity->GetComponent(Component_Physics) == nullptr)
                        {
                            vec3 position = m_CurrentEntity->GetPosition();
                            vec3 scale = m_CurrentEntity->GetScale();
                            btRigidBody* rigidBody = PhysicsFactory::CreateRigidCube(btVector3(position.x, position.y, position.z), scale.x, scale.y, scale.z, 1.0f);
                            PhysicsComponent* component = new Bullet3Component(rigidBody);
                            m_CurrentEntity->AddComponent(component);
                            Physics::RegisterObject(rigidBody);

                            Bullet3Routine* bulletRoutine = new Bullet3Routine();
                            m_CurrentEntity->AddUpdateRoutine(bulletRoutine);
                            bulletRoutine->Initialize();

                            // TODO: Free bulletRoutine RAM
                        }
                        break;
                    }

                    showComponentSelector = false;
                }
            }

            // Draw properties for object components like render, character, etc
            m_EditComponent->Draw(m_CurrentEntity);

            ImGui::End();
        }
        else
        {
            ImGui::End();
        }
	}

}
