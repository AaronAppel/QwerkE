#include "QE_SceneGraph.h"

#include <map>
#include <string>

#include "Libraries/imgui/imgui.h"

#include "QC_StringHelpers.h"

#include "QF_Scene.h"
#include "QF_Scenes.h"
#include "QF_GameObject.h"
#include "QF_Factory.h"

#include "QE_Editor.h"
#include "QE_EntityEditor.h"

namespace QwerkE {

	SceneGraph::SceneGraph(EntityEditor* entityEditor)
		: m_EntityEditor(entityEditor)
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

			ImGui::Separator();

			std::map<std::string, GameObject*> entities = currentScene->GetObjectList(); // #TODO Move down inside switch
			std::map<std::string, GameObject*>::iterator it;

			std::vector<GameObject*> cameras = currentScene->GetCameraList(); // #TODO Move down inside switch
			std::vector<GameObject*> lights = currentScene->GetLightList(); // #TODO Move down inside switch

			int itemWidth = 100;
			int itemsPerRow = (int)ImGui::GetWindowWidth() / itemWidth + 1;

			int counter = 0; // #TODO Remove
			ImGui::PushItemWidth(50);

			switch (m_CurrentList)
			{
			case 0: // Actors
				for (it = entities.begin(); it != entities.end(); it++)
				{
					if (counter % itemsPerRow) ImGui::SameLine();

					if (ImGui::Button(it->second->GetName().c_str()))
					{
						m_EntityEditor->SetCurrentEntity(it->second);
					}
					counter++;
				}
				break;

			case 1: // Cameras
				for (unsigned int i = 0; i < cameras.size(); i++)
				{
					if (counter % itemsPerRow) ImGui::SameLine();

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