#include "QE_SceneGraph.h"

#include <map>
#include <string>

#include "Libraries/Enum/QC_enum.h"
#include "Libraries/imgui/QC_imgui.h"

#include "QC_StringHelpers.h"

#include "QF_GameObject.h"
#include "QF_RenderComponent.h"
#include "QF_RenderRoutine.h"
#include "QF_Assets.h"
#include "QF_Scene.h"
#include "QF_Scenes.h"
#include "QF_Serialization.h"

#include "QE_Editor.h"
#include "QE_EntityEditor.h"

QC_ENUM(eSceneGraphFilter, int, Actors, Cams, Lights)
QC_ENUM(eSceneGraphCreateTypes, int, Empty, Light, Camera)

namespace QwerkE {

	SceneGraph::SceneGraph(EntityEditor* entityEditor)
		: m_EntityEditor(entityEditor)
	{
	}

	RenderComponent* AddModelComponentFromSchematic(GameObject* entity, const char* objectRecipeName)
	{
		RenderComponent* rComp = new RenderComponent();
		Serialization::DeserializeJsonFromFile(SchematicsFolderPath(objectRecipeName), *rComp);
		rComp->Activate();
		return rComp;
	}

	void SceneGraph::Draw()
	{
		if (ImGui::Begin("SceneGraph"))
		{
			Scene* currentScene = Scenes::GetCurrentScene();
			if (currentScene == nullptr)
				return;

			for (size_t i = 0; i < eSceneGraphFilter::_size_constant; i++)
			{
				if (i > 0) ImGui::SameLine();

				if (ImGui::Button(ENUM_TO_STR(eSceneGraphFilter::_from_index(i))))
				{
					m_CurrentList = i;
				}
			}

			ImGui::SameLine();
			if (ImGui::Button("+"))
			{
				ImGui::OpenPopup("Create Object");
			}

			if (ImGui::BeginPopup("Create Object")) // #TODO Support file and prefab instantiation
			{
				ImGui::Text("Object Types");
				ImGui::Separator();

				for (int i = 0; i < eSceneGraphCreateTypes::_size_constant; i++)
					if (ImGui::Selectable(ENUM_TO_STR(eSceneGraphCreateTypes::_from_index(i))))
					{
						switch (i)
						{
						case eSceneGraphCreateTypes::Empty:
							currentScene->CreateNewObjectFromSchematic("GameObject.osch");
							break;

						case eSceneGraphCreateTypes::Light:
							{
								GameObject* newLight = currentScene->CreateNewObjectFromSchematic("Light.osch");
								currentScene->AddLight(newLight); // #TODO Call from light component or game object
							}
							break;

						case eSceneGraphCreateTypes::Camera:
							{
								GameObject* newCamera = currentScene->CreateNewObjectFromSchematic("Camera.osch");
								currentScene->AddCamera(newCamera); // #TODO Call from camera component or game object
							}
							break;
						}
					}
				ImGui::EndPopup();
			}
			ImGui::Separator();

			const float itemWidth = 100;
			const int itemsPerRow = (int)(ImGui::GetWindowWidth() / itemWidth + 1.f);
			int sameLineCounter = 0;
			ImGui::PushItemWidth(50);

			switch (m_CurrentList)
			{
			case eSceneGraphFilter::Actors:
				{
					std::map<std::string, GameObject*> entities = currentScene->GetObjectList();
					std::map<std::string, GameObject*>::iterator it;
					for (it = entities.begin(); it != entities.end(); it++)
					{
						if (sameLineCounter % itemsPerRow) ImGui::SameLine();

						if (ImGui::Button(it->second->GetName().c_str()))
						{
							m_EntityEditor->SetCurrentEntity(it->second);
							break;
						}
						sameLineCounter++;
					}
				}
				break;

			case eSceneGraphFilter::Cams:
				{	// #TODO Move logic to a re-usable function as it's the same logic as for lights below, with different input
					std::vector<GameObject*> cameras = currentScene->GetCameraList();
					for (unsigned int i = 0; i < cameras.size(); i++)
					{
						if (sameLineCounter % itemsPerRow) ImGui::SameLine();

						if (ImGui::Button(cameras[i]->GetName().c_str()))
						{
							m_EntityEditor->SetCurrentEntity(cameras[i]);
							break;
						}
						sameLineCounter++;
					}
				}
				break;

			case eSceneGraphFilter::Lights:
				std::vector<GameObject*> lights = currentScene->GetLightList();
				for (unsigned int i = 0; i < lights.size(); i++)
				{
					if (sameLineCounter % itemsPerRow)
						ImGui::SameLine();

					if (ImGui::Button(lights[i]->GetName().c_str()))
					{
						m_EntityEditor->SetCurrentEntity(lights[i]);
						break;
					}
					sameLineCounter++;
				}
				break;
			}
			ImGui::PopItemWidth();
		}
		ImGui::End();
	}

}