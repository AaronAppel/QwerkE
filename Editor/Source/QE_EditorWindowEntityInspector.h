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

			void DrawInternal()
			{
                if (!m_CurrentEntity)
                {
                    Scene* currentScene = Scenes::GetCurrentScene();
                    m_CurrentEntity = currentScene->GetCurrentCameraEntity();
                }

                if (!m_CurrentEntity)
                {
                    ImGui::Text("Null current entity");
                    return;
                }

                Scene* entityScene = m_CurrentEntity.GetScene();
                if (!m_CurrentEntity)
                {
                    ImGui::Text("Null entity scene");
                    return;
                }

                // #TODO Nicer looking '+/-' buttons : https://youtu.be/oESRecjuLNY?t=1787
                std::string nameBuffer = m_CurrentEntity.EntityName();
                nameBuffer.reserve(INT8_MAX);

                constexpr float scalar = 1.6f; // #NOTE Aesthetic
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x / scalar);
                // #TODO Helper ImGui::InputText("Title", std::string&) Handle .data() and .capacity. Makybe even have a temp string to use as a buffer
                if (ImGui::InputText(("##" + nameBuffer).c_str(), nameBuffer.data(), nameBuffer.capacity()))
                {
                    ComponentInfo& info = m_CurrentEntity.GetComponent<ComponentInfo>();
                    info.m_EntityName = nameBuffer;
                }
                ImGui::SameLine();
                ImGui::Text(std::to_string(m_CurrentEntity.EntityGuid()).c_str());
                ImGui::PopItemWidth();

                const char* enabledState = m_CurrentEntity.IsEnabled() ? "Deactivate" : "Activate";
                ImGui::Separator();
                if (ImGui::Button(enabledState))
                {
                    m_CurrentEntity.SetIsEnabled(!m_CurrentEntity.IsEnabled());
                }
                ImGui::SameLine();
                if (ImGui::Button("Destroy"))
                {
                    entityScene->DestroyEntity(m_CurrentEntity);
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

                DrawEditComponent<ComponentInfo>(m_CurrentEntity); // Draw above
                DrawEditComponent<ComponentTransform>(m_CurrentEntity);

                DrawEditComponent<ComponentCamera>(m_CurrentEntity);
                DrawEditComponent<ComponentLight>(m_CurrentEntity);
                DrawEditComponent<ComponentMesh>(m_CurrentEntity);
                if (DrawEditComponent<ComponentScript>(m_CurrentEntity))
                {
                    ComponentScript& script = m_CurrentEntity.GetComponent<ComponentScript>();

                    for (auto& pair : script.m_ScriptInstances)
                    {
                        if (pair.second && !pair.second->GetEntity())
                        {
                            pair.second->SetEntity(EntityHandle(m_CurrentEntity.GetScene(), m_CurrentEntity.EntityGuid()));
                            pair.second->OnCreate(); // #TODO Can remove component and break iterator
                            break;
                        }
                    }
                    script.Bind(m_CurrentEntity);
                }
			}

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

            template <typename T>
            bool DrawEditComponent(EntityHandle& entity)
            {
                if (entity.HasComponent<T>())
                {
                    const float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
                    T& component = entity.GetComponent<T>();

                    const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<T>();

                    const float minusButtonWidth = ImGui::GetContentRegionAvail().x - lineHeight * 0.5f;

                    const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
                    const bool nodeOpen = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, typeInfo->stringName.c_str());

                    if (!std::is_same_v<T, ComponentTransform> && !std::is_same_v<T, ComponentInfo>)
                    {
                        ImGui::SameLine(minusButtonWidth);
                        if (ImGui::Button("-", ImVec2{ lineHeight, lineHeight }))
                        {
                            entity.RemoveComponent<T>();
                        }
                    }

                    if (nodeOpen)
                    {
                        ImGui::TreePop();

                        if (std::is_same_v<T, ComponentCamera> &&
                            entity.HasComponent<ComponentCamera>() &&
                            ImGui::Button("Switch To"))
                        {
                            Scenes::GetCurrentScene()->SetCurrentCameraEntity(entity);
                        }
                        return Inspector::InspectObject(component);
                    }
                }
                return false;
            }

            void OnEntitySelected(const EntityHandle& entity) override { m_CurrentEntity = entity; }

            EntityHandle m_CurrentEntity;
		};

	}

}