#pragma once

#include "QE_EditorWindow.h"

#include "QF_EntityComponents.h"
#include "QF_EntityHandle.h"
#include "QF_Scene.h"
#include "QF_Scenes.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowEntityInspector : public EditorWindow
		{
		public:
			EditorWindowEntityInspector::EditorWindowEntityInspector(GUID guid = GUID()) : EditorWindow("Entity Inspector", EditorWindowTypes::EntityInspector, guid) { }

		private:
            template <typename T>
            T* AddComponentEntry(EntityHandle& entity)
            {
                const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<T>();
                if (!entity.HasComponent<T>() && ImGui::MenuItem(typeInfo->stringName.c_str()))
                {
                    T& component = entity.AddComponent<T>();
                    if (std::is_same_v<T, ComponentScript>)
                    {
                        ComponentScript* script = (ComponentScript*)&component;

                        for (auto& pair : script->m_ScriptInstances)
                        {
                            if (pair.second && !pair.second->GetEntity())
                            {
                                pair.second->SetEntity(entity);
                            }
                        }
                    }
                    else if (std::is_same_v<T, ComponentMesh>)
                    {
                        ComponentMesh* mesh = (ComponentMesh*)&component;
                        mesh->Initialize();
                    }

                    ImGui::CloseCurrentPopup();
                    return &component;
                }
                return nullptr;
            }

			void DrawInternal()
			{
                Scene* currentScene = Scenes::GetCurrentScene();
                if (!currentScene)
                {
                    ImGui::Text("Null current scene");
                    return;
                }

                if (!m_CurrentEntity)
                {
                    if (Scene* scene = Scenes::GetCurrentScene())
                    {
                        m_CurrentEntity = scene->GetCurrentCameraEntity();
                    }
                }

                if (!m_CurrentEntity)
                {
                    ImGui::Text("Null current entity");
                    return;
                }

                // #TODO Nicer looking '+/-' buttons : https://youtu.be/oESRecjuLNY?t=1787
                std::string nameBuffer = m_CurrentEntity.EntityName();

                constexpr float scalar = 1.6f; // #NOTE Aesthetic
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x / scalar);
                if (ImGui::InputText(("##" + nameBuffer).c_str(), nameBuffer.data(), nameBuffer.size() + 1)) // #NOTE +1 to avoid deleting the last char on box selection
                {
                    ComponentInfo& info = m_CurrentEntity.GetComponent<ComponentInfo>();
                    info.m_EntityName = nameBuffer;
                }
                ImGui::SameLine();
                ImGui::Text(std::to_string(m_CurrentEntity.EntityGuid()).c_str());
                ImGui::PopItemWidth();

                ImGui::Separator();

                static bool showComponentSelector = false;
                const char* enabledState = m_CurrentEntity.IsEnabled() ? "Deactivate" : "Activate";
                if (ImGui::Button(enabledState))
                {
                    m_CurrentEntity.SetIsEnabled(!m_CurrentEntity.IsEnabled());
                }
                ImGui::SameLine();
                if (ImGui::Button("Destroy"))
                {
                    currentScene->DestroyEntity(m_CurrentEntity);
                }
                ImGui::SameLineEnd("Add Component");
                if (ImGui::Button("Add Component"))
                {
                    ImGui::OpenPopup("ComponentList");
                }

                if (ImGui::BeginPopup("ComponentList"))
                {
                    AddComponentEntry<ComponentCamera>(m_CurrentEntity);
                    AddComponentEntry<ComponentInfo>(m_CurrentEntity);
                    AddComponentEntry<ComponentLight>(m_CurrentEntity);
                    AddComponentEntry<ComponentMesh>(m_CurrentEntity);
                    AddComponentEntry<ComponentScript>(m_CurrentEntity);
                    AddComponentEntry<ComponentTransform>(m_CurrentEntity);
                    ImGui::EndPopup();
                }

                // #TOOD Deprecate m_EditComponent if it stays small
                if (m_CurrentEntity)
                {
                    // #TODO Deprecate contents of QE_imgui_EditComponent.cpp
                    // m_EditComponent->Draw(m_CurrentEntity);
                }
			}

            EntityHandle m_CurrentEntity;
		};

	}

}