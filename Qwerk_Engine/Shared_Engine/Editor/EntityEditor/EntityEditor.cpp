#include "EntityEditor.h"
#include "../QwerkE_Framework/QwerkE_Common/Libraries/imgui/imgui.h"
#include "../QwerkE_Framework/Scenes/Scene.h"
#include "../QwerkE_Framework/Systems/Resources/Resources.h"
#include "../QwerkE_Framework/Systems/Services.h"
#include "../QwerkE_Framework/Entities/GameObject.h"
#include "../QwerkE_Framework/Entities/Components/RenderComponent.h"
#include "../QwerkE_Framework/Entities/Routines/RenderRoutine.h"
#include "../QwerkE_Framework/Graphics/Mesh/Mesh.h"
#include "../QwerkE_Framework/Graphics/Renderable.h"
#include "imgui_EditComponent.h"
#include "../Editor.h"

#include <map>
#include <string>

namespace QwerkE {

	EntityEditor::EntityEditor(Editor* editor)
	{
		m_Editor = editor;

#ifdef dearimgui
		m_EditComponent = new imgui_EditComponent();
#else
#pragma error "Define gui library!"
#endif // !dearimgui
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
			// TODO: What if the scene is empty? What does map return? nullptr I hope
			m_CurrentEntity = m_Scenes->GetCurrentScene()->GetObjectList().begin()->second;
		}
		if (m_CurrentEntity)
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
				if (ImGui::Button("+Renderable"))
				{
					// TODO:  : renderable.SetMaterial(m_Resources->GetMaterial(null_material));

					/*Renderable renderable;
					renderable.SetMaterial(m_Resources->GetMaterial(null_material));
					renderable.SetShader(m_Resources->GetShaderProgram(null_shader));
					renderable.SetMesh(m_Resources->GetMesh(null_mesh));

					RenderComponent* rComp = (RenderComponent*)m_CurrentEntity->GetComponent(Component_Render);
					rComp->AddRenderable(renderable);
					rComp->GetParent()->GetFirstDrawRoutineOfType(Routine_Render)->Initialize();*/
				}

				// Draw properties for object components like render, character, etc
				m_EditComponent->Draw(m_CurrentEntity);

				ImGui::End();
			}
			else
				ImGui::End();
	}

}
