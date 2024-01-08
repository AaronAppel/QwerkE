#include "QE_SceneGraph.h"

#include <map>
#include <string>

#include "Libraries/enum.h"
#include "Libraries/imgui/imgui.h"

#include "QC_StringHelpers.h"

#include "QF_Scene.h"
#include "QF_Scenes.h"
#include "QF_GameObject.h"
#include "QF_Factory.h"

#include "QE_Editor.h"
#include "QE_EntityEditor.h"

#include "QC_ReflectionMacros.h"

// #TODO Create a #define to wrap the name. Maybe something like SERIALIZE_ENUM would be better
// #TODO Review enum declarations using enum.h library
BETTER_ENUM(eSceneGraphFilter, int, Actors, Cams, Lights)
BETTER_ENUM(eSceneGraphCreateTypes, int, Empty, Light, Camera)

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

			if (ImGui::BeginPopup("Create Object"))
			{
				ImGui::Text("Object Types");
				ImGui::Separator();

				for (int i = 0; i < eSceneGraphCreateTypes::_size_constant; i++)
					if (ImGui::Selectable(ENUM_TO_STR(eSceneGraphCreateTypes::_from_index(i))))
					{
						switch (i)
						{
						case eSceneGraphCreateTypes::Empty:
							currentScene->AddObjectToScene(Factory::CreateEmptyGameObject(currentScene, vec3(0, 0, 0)));
							break;

						case eSceneGraphCreateTypes::Light:
							currentScene->AddLight(Factory::CreateLight(currentScene, vec3(0, 0, 0)));
							break;

						case eSceneGraphCreateTypes::Camera:
							currentScene->AddCamera(Factory::CreateFreeCamera(currentScene, vec3(0, 0, 0)));
							break;
						}
					}
				ImGui::EndPopup();
			}
			ImGui::Separator();

			int itemWidth = 100;
			int itemsPerRow = (int)ImGui::GetWindowWidth() / itemWidth + 1;
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
						}
						sameLineCounter++;
					}
				}
				break;

			case eSceneGraphFilter::Cams:
				{
					std::vector<GameObject*> cameras = currentScene->GetCameraList();
					for (unsigned int i = 0; i < cameras.size(); i++)
					{
						if (sameLineCounter % itemsPerRow) ImGui::SameLine();

						if (ImGui::Button(cameras[i]->GetName().c_str()))
						{
							m_EntityEditor->SetCurrentEntity(cameras[i]);
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