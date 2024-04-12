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

QC_ENUM(eSceneGraphCreateTypes, u8, Empty, Light, Camera, Script, Mesh) // #TODO Use a proper eComponentTypes enum everywhere

namespace QwerkE {

	SceneGraph::SceneGraph(EntityEditor* entityEditor)
		: m_EntityEditor(entityEditor)
	{ }

#define SceneGraphEntityList(COMPONENT_TYPE) \
	auto viewComponents = currentScene->ViewComponents<COMPONENT_TYPE>(); \
	for (auto entity : viewComponents) \
	{ \
		if (sameLineCounter % itemsPerRow) ImGui::SameLine(); \
 \
		EntityHandle handle(currentScene, entity); \
		ComponentInfo& info = handle.GetComponent<ComponentInfo>(); \
		if (ImGui::Button(info.m_EditorDisplayName)) \
		{ \
			m_EntityEditor->SetCurrentEntity(handle); \
			break; \
		} \
		sameLineCounter++; \
	}

	void SceneGraph::Draw()
	{
#ifdef _QDEARIMGUI
		if (ImGui::Begin("SceneGraph"))
		{
			Scene* currentScene = Scenes::GetCurrentScene();
			if (currentScene == nullptr)
				return;

			ImGui::Text((currentScene->GetSceneName() + ":").c_str());
			ImGui::SameLineEnd(1.f);
			if (ImGui::Button("+"))
			{
				ImGui::OpenPopup("Create Entity");
			}

			if (ImGui::BeginPopup("Create Entity")) // #TODO Support file and prefab instantiation
			{
				ImGui::Text("Entity Types");
				ImGui::Separator();

				// #TODO Option to create from prefab (or add option to create prefab instance another way)
				// #TODO Use NumberAppendOrIncrement() to create a unique object name
				for (int i = 0; i < eSceneGraphCreateTypes::_size_constant; i++)
				{
					if (ImGui::Selectable(ENUM_TO_STR(eSceneGraphCreateTypes::_from_index(i))))
					{
						// #TODO Look at a using a macro/template function for repition
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
							currentScene->CreateEntity().AddComponent<ComponentScript>();
							break;

						case eSceneGraphCreateTypes::Mesh:
							currentScene->CreateEntity().AddComponent<ComponentMesh>();
							break;
						}
					}
				}
				ImGui::EndPopup();
			}

			auto viewTransforms = currentScene->ViewComponents<ComponentTransform>();
			for (auto& entity : viewTransforms)
			{
				EntityHandle handle(currentScene, entity);
				ComponentInfo& info = handle.GetComponent<ComponentInfo>();
				if (ImGui::Button(info.m_EditorDisplayName))
				{
					m_EntityEditor->SetCurrentEntity(handle);
					break;
				}
			}
		}
		ImGui::End();
#endif
	}

}