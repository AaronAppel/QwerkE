#include "EntityEditor.h"
#include "../QwerkE_Framework/QwerkE_Common/Libraries/imgui/imgui.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/SceneManager.h"
#include "../QwerkE_Framework/QwerkE_Framework/Scenes/Scene.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/ResourceManager/ResourceManager.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/ServiceLocator.h"
#include "../QwerkE_Framework/QwerkE_Framework/Entities/GameObject.h"
#include "../QwerkE_Framework/QwerkE_Framework/Graphics/Mesh/Mesh.h"
#include "imgui_EditComponent.h"
#include "../Editor.h"

#include <map>
#include <string>

EntityEditor::EntityEditor(Editor* editor)
{
    m_Editor = editor;
    m_SceneManager = (SceneManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Scene_Manager);
    m_ResourceManager = (ResourceManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Resource_Manager);

	m_EditComponent = new imgui_EditComponent();
}

EntityEditor::~EntityEditor()
{
	delete m_EditComponent;
}
// imgui styling: https://www.unknowncheats.me/forum/direct3d/189635-imgui-style-settings.html
void EntityEditor::Draw()
{
	if (m_CurrentEntity == nullptr)
	{
		// TODO: What if the scene is empty? What does map return? nullptr I hope
		m_CurrentEntity = m_SceneManager->GetCurrentScene()->GetObjectList().begin()->second;
	}
	if(m_CurrentEntity)
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

		// Draw properties for object components like render, character, etc
		m_EditComponent->Draw(m_CurrentEntity);

		ImGui::End();
	}
	else
		ImGui::End();
}