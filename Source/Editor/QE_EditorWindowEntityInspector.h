#pragma once

#include "QE_EditorWindow.h"

#include "QF_Buffer.h"
#include "QF_ComponentHelpers.h"
#include "QF_EntityHandle.h"
#include "QF_Scene.h"
#include "QF_Scenes.h"
#include "QF_ScriptHelpers.h"

#include "QE_EditorInspector.h"

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
                    if (Scene* currentScene = Scenes::GetCurrentScene())
                    {
                        m_CurrentEntity = currentScene->GetCurrentCameraEntity();
                    }
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

                {   // Edit entity name
                    Buffer buffer(INT8_MAX); // #TODO Could be re-used/persistent and updated on entity change
                    buffer.Fill('\0');
                    strcpy(buffer.As<char>(), m_CurrentEntity.EntityName().c_str());

                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x / 1.8f); // #NOTE Aesthetic scalar value
                    if (ImGui::InputText(("##" + m_WindowName + "EntityName").c_str(), buffer.As<char>(), buffer.SizeInBytes()))
                    {
                        m_CurrentEntity.GetComponent<ComponentInfo>().m_EntityName = buffer.As<char>();
                    }
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
                    // #TODO Use EntityComponentsList
                    AddComponentEntry<ComponentCamera>(m_CurrentEntity);
                    AddComponentEntry<ComponentInfo>(m_CurrentEntity);
                    AddComponentEntry<ComponentLight>(m_CurrentEntity);
                    AddComponentEntry<ComponentMesh>(m_CurrentEntity);
                    AddComponentEntry<ComponentScript>(m_CurrentEntity);
                    AddComponentEntry<ComponentTransform>(m_CurrentEntity);
                    ImGui::EndPopup();
                }

                // #TODO Use EntityComponentsList
                DrawEditComponent<ComponentInfo>(m_CurrentEntity); // Draw above
                DrawEditComponent<ComponentTransform>(m_CurrentEntity);

                DrawEditComponent<ComponentCamera>(m_CurrentEntity);
                DrawEditComponent<ComponentLight>(m_CurrentEntity);
                DrawEditComponent<ComponentMesh>(m_CurrentEntity);
                if (DrawEditComponent<ComponentScript>(m_CurrentEntity))
                {
                    ComponentScript& script = m_CurrentEntity.GetComponent<ComponentScript>();

                    // #TODO Review script.AddScript(pair.first) use rather than below logic
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

                    const float buttonWidth = ImGui::GetContentRegionAvail().x - lineHeight * 0.5f;

                    const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
                    const bool nodeOpen = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, typeInfo->stringName.c_str());

                    if (std::is_same_v<T, ComponentMesh>)
                    {
                        std::string popUpName = "Context " + typeInfo->stringName;

                        // ImGui::SameLine(buttonWidth * 2);
                        // if (ImGui::Button("&", ImVec2{ lineHeight, lineHeight }))
                        // {
                        //     ImGui::OpenPopup(popUpName.c_str());
                        // }

                        if (ImGui::IsItemClicked(ImGui::MouseRight))
                        {
                            ImGui::OpenPopup(popUpName.c_str());
                        }

                        if (ImGui::BeginPopup(popUpName.c_str()))
                        {
                            if (ImGui::Button("Initialize"))
                            {
                                entity.GetComponent<ComponentMesh>().Initialize();
                            }
                            ImGui::EndPopup();
                        }
                    }

                    if (!std::is_same_v<T, ComponentTransform> && !std::is_same_v<T, ComponentInfo>)
                    {
                        ImGui::SameLine(buttonWidth);
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

            void OnEntitySelected(EntityHandle& entity) override { m_CurrentEntity = entity; }

            EntityHandle m_CurrentEntity;
		};

	}

}