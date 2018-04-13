#include "SceneGraph.h"
#include "../../../Shared_Generic/Libraries/imgui/imgui.h"
#include "../../Scene/SceneManager.h"
#include "../../Systems/ResourceManager.h"
#include "../../Scene/Scene.h"
#include "../../Systems/ServiceLocator.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/Components/RenderComponent.h"
#include "../Editor.h"
#include "../EntityEditor/EntityEditor.h"

#include <map>
#include <string>

SceneGraph::SceneGraph(Editor* editor)
{
    m_Editor = editor;
	m_SceneManager = (SceneManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Scene_Manager);
	m_ResourceManager = (ResourceManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Resource_Manager);
}

SceneGraph::~SceneGraph()
{
}

void SceneGraph::Draw()
{
	static bool isOpen = true;
	ImGui::Begin("SceneGraph", &isOpen);

	std::map<std::string, GameObject*> entities = m_SceneManager->GetCurrentScene()->GetObjectList();
	std::map<std::string, GameObject*>::iterator thing;

	for (thing = entities.begin(); thing != entities.end(); thing++)
	{
		if (ImGui::Button(thing->second->GetName().c_str()))
		{
            m_Editor->GetEntityEditor()->SetCurrentEntity(thing->second->GetName());
        }
	}

	ImGui::End();
}