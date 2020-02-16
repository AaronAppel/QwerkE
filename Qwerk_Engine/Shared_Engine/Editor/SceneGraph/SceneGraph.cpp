#include "SceneGraph.h"
#include "../QwerkE_Framework/Libraries/imgui/imgui.h"
#include "../QwerkE_Framework/Scenes/Scene.h"
#include "../QwerkE_Framework/Entities/GameObject.h"
#include "../Editor.h"
#include "../EntityEditor/EntityEditor.h"
#include "../imgui_Editor.h"
#include "../QwerkE_Framework/Systems/Factory/Factory.h"
#include "../../Utilities/StringHelpers.h"
#include "../../Systems/Resources/GUID.h"

#include <map>
#include <string>

namespace QwerkE {

	SceneGraph::SceneGraph(Editor* editor)
		: m_Editor((imgui_Editor*)editor)
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

			// Object creation
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
						Scene* scene = m_Scenes->GetCurrentScene();

						switch (i)
						{
						case 0:
							scene->AddObjectToScene(Factory::CreateEmptyGameObject(scene, vec3(0, 0, 0)));
							break;
						case 1:
							scene->AddLight(Factory::CreateLight(scene, vec3(0, 0, 0)));
							break;
						case 2:
							scene->AddCamera(Factory::CreateFreeCamera(scene, vec3(0, 0, 0)));
							break;
						case 3:
							scene->AddObjectToScene(Factory::CreateSphere(scene, vec3(0, 0, 0)));
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
						m_Scenes->GetCurrentScene(), vec3(0,0,0));
					result->SetName(StringAppend("Object", std::to_string(UniqueID()).c_str()));
					m_Scenes->GetCurrentScene()->AddObjectToScene(result);
					creator = false;
				}
				ImGui::End();
			}*/

			ImGui::Separator();

			std::map<std::string, GameObject*> entities = m_Scenes->GetCurrentScene()->GetObjectList();
			std::vector<GameObject*> cameras = m_Scenes->GetCurrentScene()->GetCameraList();
			std::vector<GameObject*> lights = m_Scenes->GetCurrentScene()->GetLightList();
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
				for (unsigned int i = 0; i < cameras.size(); i++)
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
				for (unsigned int i = 0; i < lights.size(); i++)
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