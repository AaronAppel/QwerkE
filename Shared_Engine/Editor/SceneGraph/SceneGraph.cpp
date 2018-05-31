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
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Factory/Factory.h"
#include "../QwerkE_Framework/QwerkE_Common/Utilities/StringHelpers.h"
#include "../QwerkE_Framework/QwerkE_Common/Utilities/GUID.h"

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
		ImGui::SameLine();
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
		ImGui::SameLine();
		// object creation
		static bool creator = false;
		if (ImGui::Button("Add+"))
		{
			creator = !creator;
		}
		if (creator)
		{
			if (ImGui::BeginPopup("Create Object"))
			{


				if (ImGui::Button("Create"))
				{
					GameObject* result = ((Factory*)QwerkE::ServiceLocator::GetService(eEngineServices::Factory_Entity))->CreateTestModel(
						m_SceneManager->GetCurrentScene(), vec3(0,0,0));
					result->SetName(StringAppend("Object", std::to_string(UniqueID()).c_str()));
					m_SceneManager->GetCurrentScene()->AddObjectToScene(result);
				}
				ImGui::End();
			}
		}
		ImGui::Separator();

		std::map<std::string, GameObject*> entities = m_SceneManager->GetCurrentScene()->GetObjectList();
		std::vector<GameObject*> cameras = m_SceneManager->GetCurrentScene()->GetCameraList();
		std::vector<GameObject*> lights = m_SceneManager->GetCurrentScene()->GetLightList();
		std::map<std::string, GameObject*>::iterator thing;

		int itemWidth = 100;
		int itemsPerRow = ImGui::GetWindowWidth() / itemWidth + 1;

		int counter = 0;
		ImGui::PushItemWidth(50);
		switch (m_CurrentList)
		{
		case 0: // Actors
			for (thing = entities.begin(); thing != entities.end(); thing++)
			{

				if (counter % itemsPerRow)
					ImGui::SameLine();

				if (ImGui::Button(thing->second->GetName().c_str()))
				{
					m_Editor->GetEntityEditor()->SetCurrentEntity(thing->second);
				}
				counter++;
			}
			break;
		case 1: // Cams
			for (int i = 0; i < cameras.size(); i++)
			{
				if (counter % itemsPerRow)
					ImGui::SameLine();

				if (ImGui::Button(cameras[i]->GetName().c_str()))
				{
					m_Editor->GetEntityEditor()->SetCurrentEntity(cameras[i]);
				}
				counter++;
			}
			break;
		case 2: // Lights
			for (int i = 0; i < lights.size(); i++)
			{
				if (counter % itemsPerRow)
					ImGui::SameLine();

				if (ImGui::Button(lights[i]->GetName().c_str()))
				{
					m_Editor->GetEntityEditor()->SetCurrentEntity(lights[i]);
				}
				counter++;
			}
			break;
		case 3: // Other
			if (ImGui::Button("What else is there?"))
			{
			}
			break;
		}
		ImGui::PopItemWidth();
		ImGui::End();
	}
}