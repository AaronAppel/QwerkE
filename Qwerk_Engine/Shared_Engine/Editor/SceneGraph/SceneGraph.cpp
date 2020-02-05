#include "SceneGraph.h"
#include "../QwerkE_Framework/QwerkE_Common/Libraries/imgui/imgui.h"
#include "../QwerkE_Framework/Scenes/Scene.h"
#include "../QwerkE_Framework/Entities/GameObject.h"
#include "../Editor.h"
#include "../EntityEditor/EntityEditor.h"
#include "../imgui_Editor.h"
#include "../QwerkE_Framework/Systems/SceneManager.h"
#include "../QwerkE_Framework/Systems/Services.h"
#include "../QwerkE_Framework/Systems/ResourceManager/ResourceManager.h"
#include "../QwerkE_Framework/Systems/Factory/Factory.h"
#include "../QwerkE_Framework/QwerkE_Common/Utilities/StringHelpers.h"
#include "../QwerkE_Framework/QwerkE_Common/Utilities/GUID.h"

#include <map>
#include <string>

namespace QwerkE {

	SceneGraph::SceneGraph(Editor* editor)
		: m_Editor((imgui_Editor*)editor),
	      m_SceneManager((SceneManager*)QwerkE::Services::GetService(eEngineServices::Scene_Manager))
	{
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
			// object creation
			if (ImGui::Button("Add+"))
			{
				ImGui::OpenPopup("Create Object");
			}

			if (ImGui::BeginPopup("Create Object"))
			{
				ImGui::Text("Object Types");
				ImGui::Separator();

				const char* names[] = { "Empty", "Light", "Camera", "Sphere" };
				for (int i = 0; i < IM_ARRAYSIZE(names); i++)
					if (ImGui::Selectable(names[i]))
					{
						Scene* scene = m_SceneManager->GetCurrentScene();
						Factory* factory = (Factory*)QwerkE::Services::GetService(eEngineServices::Factory_Entity);

						switch (i)
						{
						case 0:
							scene->AddObjectToScene(factory->CreateEmptyGameObject(scene, vec3(0, 0, 0)));
							break;
						case 1:
							scene->AddLight(factory->CreateLight(scene, vec3(0, 0, 0)));
							break;
						case 2:
							scene->AddCamera(factory->CreateFreeCamera(scene, vec3(0, 0, 0)));
							break;
						case 3:
							scene->AddObjectToScene(factory->CreateSphere(scene, vec3(0, 0, 0)));
							break;
						default:
							break;
						}
					}
				ImGui::EndPopup();
			}
			/*if (ImGui::Begin("Create Object", &creator))
			{
				if (ImGui::Button("Create"))
				{
					GameObject* result = ((Factory*)QwerkE::Services::GetService(eEngineServices::Factory_Entity))->CreateEmptyGameObject(
						m_SceneManager->GetCurrentScene(), vec3(0,0,0));
					result->SetName(StringAppend("Object", std::to_string(UniqueID()).c_str()));
					m_SceneManager->GetCurrentScene()->AddObjectToScene(result);
					creator = false;
				}
				ImGui::End();
			}*/

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
			case 1: // Cameras
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
			}
			ImGui::PopItemWidth();
			ImGui::End();
		}
		else
			ImGui::End();
	}

}