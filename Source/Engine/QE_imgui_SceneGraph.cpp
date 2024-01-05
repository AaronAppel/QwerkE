#include "QE_SceneGraph.h"

#include <map>
#include <string>

#include "imgui/imgui.h"

#include "QC_StringHelpers.h"

#include "../QwerkE_Framework/Source/Core/Scenes/Scene.h"
#include "../QwerkE_Framework/Source/Core/Scenes/Scenes.h"
#include "../QwerkE_Framework/Source/Core/Scenes/Entities/GameObject.h"
#include "../QwerkE_Framework/Source/Core/Factory/Factory.h"
#include "QF_GUID.h"

#include "QE_Editor.h"
#include "QE_EntityEditor.h"

namespace QwerkE {

	SceneGraph::SceneGraph(EntityEditor* entityEditor)
		: m_EntityEditor(entityEditor)
	{
	}

	SceneGraph::~SceneGraph()
	{
	}

	void SceneGraph::Draw()
	{
		if (ImGui::Begin("SceneGraph"))
		{
			Scene* currentScene = Scenes::GetCurrentScene();

			if (currentScene == nullptr)
				return;

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
						switch (i)
						{
						case 0:
							currentScene->AddObjectToScene(Factory::CreateEmptyGameObject(currentScene, vec3(0, 0, 0)));
							break;
						case 1:
							currentScene->AddLight(Factory::CreateLight(currentScene, vec3(0, 0, 0)));
							break;
						case 2:
							currentScene->AddCamera(Factory::CreateFreeCamera(currentScene, vec3(0, 0, 0)));
							break;
						case 3:
							currentScene->AddObjectToScene(Factory::CreateSphere(currentScene, vec3(0, 0, 0)));
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

			std::map<std::string, GameObject*> entities = currentScene->GetObjectList();
			std::vector<GameObject*> cameras = currentScene->GetCameraList();
			std::vector<GameObject*> lights = currentScene->GetLightList();
			std::map<std::string, GameObject*>::iterator thing;

			int itemWidth = 100;
			int itemsPerRow = (int)ImGui::GetWindowWidth() / itemWidth + 1;

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
						m_EntityEditor->SetCurrentEntity(thing->second);
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
						m_EntityEditor->SetCurrentEntity(cameras[i]);
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
						m_EntityEditor->SetCurrentEntity(lights[i]);
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