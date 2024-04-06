#include "QE_SceneGraph.h"

#include <map>
#include <string>

#ifdef _QENTT
#include "Libraries/entt/entt.hpp"
#endif

#ifdef _QENUM
#include "Libraries/Enum/QC_enum.h"
#endif

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QC_imgui.h"
#endif

#include "QC_StringHelpers.h"

#include "QF_Scene.h"
#include "QF_Scenes.h"
#include "QF_Serialization.h"

#include "QE_Editor.h"
#include "QE_EntityEditor.h"

#include "QF_ComponentCamera.h"
#include "QF_ComponentLight.h"
#include "QF_ComponentMesh.h"
#include "QF_ComponentScript.h"

QC_ENUM(eSceneGraphFilter, u8, Actors, Cams, Lights)
QC_ENUM(eSceneGraphCreateTypes, u8, Empty, Light, Camera, Script, Mesh)

namespace QwerkE {

	SceneGraph::SceneGraph(EntityEditor* entityEditor)
		: m_EntityEditor(entityEditor)
	{
	}

	void SceneGraph::Draw()
	{
#ifdef _QDEARIMGUI
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

				// #TODO Option to create from prefab (or add option to create prefab instance another way)
				// #TODO Use NumberAppendOrIncrement() to create a unique object name
				for (int i = 0; i < eSceneGraphCreateTypes::_size_constant; i++)
				{
					if (ImGui::Selectable(ENUM_TO_STR(eSceneGraphCreateTypes::_from_index(i))))
					{
						switch (i)
						{
						case eSceneGraphCreateTypes::Empty:
							currentScene->CreateEntity();
							break;

						case eSceneGraphCreateTypes::Light:
							currentScene->CreateEntity().AddComponent<ComponentLight>();
							break;

						case eSceneGraphCreateTypes::Camera:
							currentScene->CreateEntity().AddComponent<ComponentCamera>();
							break;

						case eSceneGraphCreateTypes::Script:
							// currentScene->CreateEntity()->AddComponent<ComponentScript>();
							// #TODO Bind to a script type. Might need a sub-context menu option and list script options
							LOG_WARN("{0} Case not implemented yet", __FUNCTION__);
							break;

						case eSceneGraphCreateTypes::Mesh:
							currentScene->CreateEntity().AddComponent<ComponentMesh>();
							break;

						default:
							break;
						}
					}
				}
				ImGui::EndPopup();
			}
			ImGui::Separator();

			const float itemWidth = 100;
			const int itemsPerRow = (int)(ImGui::GetWindowWidth() / itemWidth + 1.f);
			int sameLineCounter = 0;
			ImGui::PushItemWidth(50);

			// For drawing scene hierarchy of nodes
			const bool selected = false;
			ImGuiTreeNodeFlags flags = (selected ? ImGuiTreeNodeFlags_Selected : 0 ) | ImGuiTreeNodeFlags_OpenOnArrow;
			flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

			switch (m_CurrentList)
			{
			case eSceneGraphFilter::Actors:
				{
					auto viewTransforms = currentScene->ViewComponents<ComponentTransform>();
					for (auto entity : viewTransforms)
					{
						EntityHandle handle(currentScene, entity);
						if (handle.HasComponent<ComponentCamera>() || handle.HasComponent<ComponentLight>())
							break;

						if (sameLineCounter % itemsPerRow) ImGui::SameLine();
						ComponentTransform& transform = viewTransforms.get<ComponentTransform>(entity);
						// #TODO Get name for camera
						// if (ImGui::Button(transform[i]->GetName().c_str()))
						{
							// m_EntityEditor->SetCurrentEntity(entity);
							// break;
						}
						sameLineCounter++;
					}
				}
				break;

			case eSceneGraphFilter::Cams: // #TODO Duplicated logic with lights. Could be templated or macro'd
				{
					auto viewCameras = currentScene->ViewComponents<ComponentCamera>();
					for (auto entity : viewCameras)
					{
						if (sameLineCounter % itemsPerRow) ImGui::SameLine();

						EntityHandle handle(currentScene, entity);
						ComponentInfo& info = handle.GetComponent<ComponentInfo>();
						if (ImGui::Button(info.m_EditorDisplayName))
						{
							m_EntityEditor->SetCurrentEntity(handle);
							break;
						}
						sameLineCounter++;
					}
				}
				break;

			case eSceneGraphFilter::Lights:
				auto viewLights = currentScene->ViewComponents<ComponentLight>();
				for (auto entity : viewLights)
				{
					if (sameLineCounter % itemsPerRow) ImGui::SameLine();

					EntityHandle handle(currentScene, entity);
					const ComponentInfo& info = handle.GetComponent<ComponentInfo>();
					if (ImGui::Button(info.m_EditorDisplayName))
					{
						m_EntityEditor->SetCurrentEntity(handle);
						break;
					}
					sameLineCounter++;
				}
				break;
			}
			ImGui::PopItemWidth();
		}
		ImGui::End();
#endif
	}

}