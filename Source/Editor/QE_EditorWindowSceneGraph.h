#pragma once

#ifdef _QENUM
#include "Libraries/Enum/QC_enum.h"
#endif

#include "QF_ComponentHelpers.h"
#include "QF_Scene.h"

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		QC_ENUM(eSceneGraphCreateTypes, u8, Empty, Light, Camera, Script, Mesh) // #TODO Use a proper eComponentTypes enum everywhere

		class EditorWindowSceneGraph : public EditorWindow
		{
		public:
			EditorWindowSceneGraph(GUID guid = GUID()) : EditorWindow("Scene Graph", EditorWindowTypes::SceneGraph, guid) { }

		private:

			void DrawInternal()
			{
				// #TODO A scene list dropdown seems pretty useful and repeated in many places.
				// Create a helper struct for it to reduce duplicated code and ease usage.
				const std::vector<Scene*>& scenes = Scenes::LookAtScenes();
				if (scenes.empty())
				{
					ImGui::Text("No scene(s) found");
					return;
				}

				std::vector<const char*> sceneNames;
				sceneNames.reserve(3);

				for (size_t i = 0; i < scenes.size(); i++)
				{
					sceneNames.push_back(scenes[i]->GetSceneName().c_str());
				}

				if (m_LastSceneIndex >= scenes.size())
				{
					int b = 0; // #TODO Catch case
				}

				const u32 sceneFileNameWidth = (u32)strlen(sceneNames[m_LastSceneIndex]) * (ImGui::g_pixelsPerCharacter);

				ImGui::Dummy(ImVec2(0, 0));

				constexpr u32 s_DropDownArrowSize = 45;
				ImGui::PushItemWidth((float)sceneFileNameWidth + (float)s_DropDownArrowSize);

				snprintf(m_ScenesComboLabelBuffer, sizeof(m_ScenesComboLabelBuffer), "##Scenes: %i##%llu", (int)sceneNames.size(), GetGuid());

				if (ImGui::Combo(m_ScenesComboLabelBuffer, &m_LastSceneIndex, sceneNames.data(), (s32)scenes.size()))
				{
					m_CurrentScene = scenes[m_LastSceneIndex];
					// #NOTE Scene transition changes removes above lines for below
					// #TODO SetActive(true/false) ?
					// Scenes::SetCurrentScene(index);
				}
				ImGui::PopItemWidth();

				if (nullptr == m_CurrentScene)
				{
					m_CurrentScene = Scenes::GetCurrentScene();
				}

				if (m_CurrentScene == nullptr)
				{
					ImGui::Text("No scene selected");
					return;
				}

				ImGui::SameLine(ImGui::GetContentRegionAvail().x - 8.f);
				if (ImGui::Button("+"))
				{
					ImGui::OpenPopup("Create Entity");
				}
				ImGui::Separator();

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
								m_CurrentScene->CreateEntity();
								break;

							case eSceneGraphCreateTypes::Light:
								m_CurrentScene->CreateEntity().AddComponent<ComponentLight>();
								break;

							case eSceneGraphCreateTypes::Camera:
								m_CurrentScene->CreateEntity().AddComponent<ComponentCamera>();
								break;

							case eSceneGraphCreateTypes::Script:
								m_CurrentScene->CreateEntity().AddComponent<ComponentScript>();
								break;

							case eSceneGraphCreateTypes::Mesh:
								m_CurrentScene->CreateEntity().AddComponent<ComponentMesh>().Initialize();
								break;
							}
						}
					}
					ImGui::EndPopup();
				}

				auto viewTransforms = m_CurrentScene->ViewComponents<ComponentTransform>();
				for (auto& entity : viewTransforms)
				{
					EntityHandle handle(m_CurrentScene, entity);
					ComponentInfo& info = handle.GetComponent<ComponentInfo>();

					if (ImGui::Button((info.m_EntityName + "##" + std::to_string(info.m_Guid)).c_str()))
					{
						Editor::OnEntitySelected(handle);
						break;
					}

					// #TODO Testing drag and drop behaviour
					if (ImGui::BeginDragDropSource())
					{
						ImGui::SetDragDropPayload("GUID", (void*)&info.m_Guid, sizeof(GUID));
						ImGui::EndDragDropSource();
					}
				}
			}

			void OnSceneReload() override { m_CurrentScene = nullptr; }

			Scene* m_CurrentScene = nullptr;
			char m_ScenesComboLabelBuffer[33] = "Scenes:    ##0000000000000000000";
			s32 m_LastSceneIndex = 0;
		};

	}

}