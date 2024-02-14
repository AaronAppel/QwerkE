#include "QE_SceneGraph.h"

#include <map>
#include <string>

#include "Libraries/Enum/QC_enum.h"
#include "Libraries/imgui/QC_imgui.h"

#include "QC_StringHelpers.h"

#include "QF_GameObject.h"
#include "QF_RenderComponent.h"
#include "QF_RenderRoutine.h"
#include "QF_Resources.h"
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
		Serialization::DeserializeJsonFromFile(ObjectSchematicsFolderPath(objectRecipeName), *rComp);
		rComp->Activate();
		return rComp;
	}

	GameObject* CreateSkyBox(Scene* scene, vec3 position)
	{
		if (!scene)
			return nullptr;

		GameObject* t_SkyBox = new GameObject(scene);
		t_SkyBox->SetPosition(position);
		t_SkyBox->SetTag(GO_Tag_SkyBox);
		t_SkyBox->SetRenderOrder(-1);
		t_SkyBox->SetName("SkyBox" + std::to_string(Resources::CreateGUID()));

		AddModelComponentFromSchematic(t_SkyBox, null_object_schematic);

		RenderRoutine* renderRoutine = new RenderRoutine();
		t_SkyBox->AddRoutine((Routine*)renderRoutine);

		if (scene->AddObjectToScene(t_SkyBox))
			return t_SkyBox;

		delete t_SkyBox;
		return nullptr;
	}

	GameObject* CreateTestCube(Scene* scene, vec3 position)
	{
		if (!scene)
			return nullptr;

		GameObject* t_Cube = new GameObject(scene, position);
		t_Cube->SetName("Object" + std::to_string(Resources::CreateGUID()));
		t_Cube->SetRenderOrder(50);
		t_Cube->SetTag(GO_Tag_TestModel);

		Renderable renderable;
		renderable.SetMaterial(Resources::GetMaterial("brickwall.msch"));
		renderable.SetShader(Resources::GetShaderProgram("LitMaterialNormal.ssch"));

		Mesh* mesh = new Mesh();
		mesh = MeshFactory::CreateCube(vec3(10, 10, 10), vec2(1, 1), false);
		mesh->SetName("Cube");
		mesh->SetFileName("None");
		Resources::AddMesh("Cube", mesh);
		renderable.SetMesh(mesh);

		RenderComponent* rComp = new RenderComponent();
		rComp->AddRenderable(renderable);

		t_Cube->AddComponent(rComp);

		// render routine
		t_Cube->AddRoutine((Routine*)new RenderRoutine());

		if (scene->AddObjectToScene(t_Cube))
			return t_Cube;

		delete t_Cube;
		return nullptr;
	}

	GameObject* CreateEmptyGameObject(Scene* scene, vec3 position)
	{
		if (scene == nullptr) return nullptr;

		GameObject* t_Object = new GameObject(scene, position);

		if (scene)
		{
			if (scene->AddObjectToScene(t_Object))
			{
				t_Object->SetName(std::string("abc") + std::to_string(0)); // #TODO Improve string concatenation
				return t_Object;
			}
		}

		delete t_Object;
		return nullptr;
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
							{
								GameObject* newGameObject = new GameObject(currentScene);
								Serialization::DeserializeJsonFromFile(ObjectSchematicsFolderPath("GameObject.osch"), *newGameObject); // #TODO Replace hard coded strings
								newGameObject->OnSceneLoaded(currentScene);
								currentScene->AddObjectToScene(newGameObject);
							}
							break;

						case eSceneGraphCreateTypes::Light:
							{
								GameObject* newLight = new GameObject(currentScene);
								Serialization::DeserializeJsonFromFile(ObjectSchematicsFolderPath("Light.osch"), *newLight);
								newLight->OnSceneLoaded(currentScene);
								currentScene->AddLight(newLight);
							}
							break;

						case eSceneGraphCreateTypes::Camera:
							{
								GameObject* newCamera = new GameObject(currentScene);
								Serialization::DeserializeJsonFromFile(ObjectSchematicsFolderPath("Camera.osch"), *newCamera);
								newCamera->SetPosition(vec3(0, 0, 0));
								newCamera->OnSceneLoaded(currentScene);
								currentScene->AddCamera(newCamera);
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