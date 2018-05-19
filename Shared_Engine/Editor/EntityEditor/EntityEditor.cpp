#include "EntityEditor.h"
#include "../QwerkE_Framework/QwerkE_Common/Libraries/imgui/imgui.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/SceneManager.h"
#include "../QwerkE_Framework/QwerkE_Framework/Scenes/Scene.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/ResourceManager.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/ServiceLocator.h"
#include "../QwerkE_Framework/QwerkE_Framework/Entities/GameObject.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Graphics/Mesh/Mesh.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Graphics/Mesh/Model.h"
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
    static bool isOpen = true;
    ImGui::Begin("Entity Editor", &isOpen);

    GameObject* thing = m_SceneManager->GetCurrentScene()->GetGameObject(m_CurrentEntity.c_str());
    if (thing == nullptr)
    {
        ImGui::End();
        return; // no object selected
    }
	//// Begine drawing entity data...
	// Draw generic GameObject data like transform and name
	static std::string name = thing->GetName().c_str() + std::to_string(' '); // extra space for editing
	ImGui::InputText("Name: ", (char*)name.data(), name.size());
	// thing->SetName(); // TODO: Scene map probably needs to handle name changes

	static float pos[3] = {0.0f, 0.0f, 0.0f};
	pos[0] = thing->GetPosition().x;
	pos[1] = thing->GetPosition().y;
	pos[2] = thing->GetPosition().z;
	static float rot[3] = {0.0f, 0.0f, 0.0f};
	rot[0] = thing->GetRotation().x;
	rot[1] = thing->GetRotation().y;
	rot[2] = thing->GetRotation().z;
	static float scale[3] = {1.0f, 1.0f, 1.0f};
	scale[0] = thing->GetScale().x;
	scale[1] = thing->GetScale().y;
	scale[2] = thing->GetScale().z;

	if(ImGui::InputFloat3("Pos: ", pos))
	{
		thing->SetPosition(vec3(pos[0], pos[1], pos[2]));
	}
	if(ImGui::InputFloat3("Rot: ", rot))
	{
		thing->SetRotation(vec3(rot[0], rot[1], rot[2]));
	}
	if(ImGui::InputFloat3("Scale: ", scale))
	{
		thing->SetScale(vec3(scale[0], scale[1], scale[2]));
	}
	ImGui::Separator();

	// Draw properties for object components like render, character, etc
	m_EditComponent->Draw(thing);

    ImGui::End();
}