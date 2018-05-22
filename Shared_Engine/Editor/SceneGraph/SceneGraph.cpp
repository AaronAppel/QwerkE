#include "SceneGraph.h"
#include "../QwerkE_Framework/QwerkE_Common/Libraries/imgui/imgui.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/SceneManager.h"
#include "../QwerkE_Framework/QwerkE_Framework/Scenes/Scene.h"
#include "../QwerkE_Framework/QwerkE_Framework/Entities/GameObject.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/ServiceLocator.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/ResourceManager/ResourceManager.h"
#include "../Editor.h"
#include "../EntityEditor/EntityEditor.h"
#include "../imgui_Editor.h"

#include <map>
#include <string>

SceneGraph::SceneGraph(Editor* editor)
{
    m_Editor = (imgui_Editor*)editor;
	m_SceneManager = (SceneManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Scene_Manager);
	m_ResourceManager = (ResourceManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Resource_Manager);
}

SceneGraph::~SceneGraph()
{
}

void SceneGraph::Draw()
{
	if (ImGui::Begin("SceneGraph"))
	{
		if (ImGui::Button("Actors"))
		{
			m_CurrentList = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cams"))
		{
			m_CurrentList = 1;
		}
		if (ImGui::Button("Lights"))
		{
			m_CurrentList = 2;
		}
		ImGui::SameLine();
		if (ImGui::Button("Other"))
		{
			// switch to ???... particles, boundaries, etc
			m_CurrentList = 3;
		}
		ImGui::Separator();

		std::map<std::string, GameObject*> entities = m_SceneManager->GetCurrentScene()->GetObjectList();
		std::map<std::string, GameObject*>::iterator thing;

		switch (m_CurrentList)
		{
		case 0: // Actors
			for (thing = entities.begin(); thing != entities.end(); thing++)
			{
				if (ImGui::Button(thing->second->GetName().c_str()))
				{
					m_Editor->GetEntityEditor()->SetCurrentEntity(thing->second->GetName());
				}
			}
			break;
		case 1: // Cams
			break;
		case 2: // Lights
			break;
		case 3: // Other
			break;
		default:
			break;
		}
		ImGui::End();
	}
}