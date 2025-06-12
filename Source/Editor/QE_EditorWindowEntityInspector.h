#pragma once

#include "QE_EditorWindow.h"

#include "QF_Buffer.h"
#include "QF_ComponentHelpers.h"
#include "QF_EntityHandle.h"
#include "QF_Scene.h"
#include "QF_Scenes.h"
#include "QF_ScriptHelpers.h"

#include "QE_EditorHotKeys.h"
#include "QE_EditorInspector.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowEntityInspector : public EditorWindow
		{
		public:
			EditorWindowEntityInspector(GUID guid = GUID()) : EditorWindow("Entity Inspector", EditorWindowTypes::EntityInspector, guid) { }

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

                if (ImGui::IsWindowFocused())
                {
                    if (IsHotkeyPressed(e_SceneGraphToggleActive))
                    {
                        m_CurrentEntity.SetIsEnabled(!m_CurrentEntity.IsEnabled());
                        HotkeyPoll(e_SceneGraphToggleActive); // #TODO Poll resets m_IsChanged flag. Find a better way to do that
                    }
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
                    if (Scene* entityScene = m_CurrentEntity.GetScene())
                    {
                        entityScene->DestroyEntity(m_CurrentEntity);
                        m_CurrentEntity.Invalidate();
                    }
                    else
                    {
                        ImGui::Text("Null entity scene");
                    }
                    return;
                }

                ImGui::SameLineEnd("Add Component");
                if (ImGui::Button("Add Component"))
                {
                    ImGui::OpenPopup("ComponentList");
                }

                if (ImGui::BeginPopup("ComponentList"))
                {
                    priv_AddComponentEntries(EntityComponentsList{}, m_CurrentEntity);
                    ImGui::EndPopup();
                }

                priv_EditComponentEntries(EntityComponentsList{}, m_CurrentEntity);
			}

            template <typename... Component>
            void priv_AddComponentEntry(EntityHandle entityHandle)
            {
                ([&]() {
                    const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<Component>();
                    if (!entityHandle.HasComponent<Component>() && ImGui::MenuItem(typeInfo->stringName.c_str()))
                    {
                        Component& component = entityHandle.AddComponent<Component>();
                        if (std::is_same_v<Component, ComponentScript>)
                        {
                            ComponentScript* script = (ComponentScript*)&component;

                            for (auto& pair : script->ScriptInstances())
                            {
                                if (pair.second && !pair.second->GetEntity())
                                {
                                    pair.second->SetEntity(entityHandle);
                                }
                            }
                        }
                        else if (std::is_same_v<Component, ComponentMesh>)
                        {
                            ComponentMesh* mesh = (ComponentMesh*)&component;
                            mesh->Initialize();
                        }

                        ImGui::CloseCurrentPopup();
                    }
                    }(), ...);
            }

            template<typename... Components>
            void priv_AddComponentEntries(TemplateArgumentList<Components...>, EntityHandle entityHandle)
            {
                priv_AddComponentEntry<Components...>(entityHandle);
            }

            template <typename... Component>
            void priv_EditComponentEntry(EntityHandle entityHandle)
            {
                ([&]() {
                    if (!entityHandle.HasComponent<Component>())
                        return;

                    const float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
                    Component& component = entityHandle.GetComponent<Component>();

                    const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<Component>();

                    const float buttonWidth = ImGui::GetContentRegionAvail().x - lineHeight * 0.5f;

                    const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
                    const bool nodeOpen = ImGui::TreeNodeEx((void*)typeid(Component).hash_code(), treeNodeFlags, typeInfo->stringName.c_str());

                    if (std::is_same_v<Component, ComponentMesh>)
                    {
                        std::string popUpName = "Context " + typeInfo->stringName;

                        // #TODO Fix context menu button
                        // ImGui::SameLineEnd(1, 0.5f);
                        ImGui::SameLine(ImGui::GetContentRegionAvail().x - lineHeight);
                        if (ImGui::Button(":", ImVec2{ lineHeight, lineHeight }))
                        {
                            ImGui::OpenPopup(popUpName.c_str());
                        }

                        if (ImGui::IsItemClicked(ImGui::MouseRight))
                        {
                            ImGui::OpenPopup(popUpName.c_str());
                        }

                        if (ImGui::BeginPopup(popUpName.c_str()))
                        {
                            if (ImGui::Button("Initialize"))
                            {
                                entityHandle.GetComponent<ComponentMesh>().Initialize();
                            }
                            ImGui::EndPopup();
                        }
                    }

                    if (!std::is_same_v<Component, ComponentTransform> && !std::is_same_v<Component, ComponentInfo>)
                    {
                        ImGui::SameLine(buttonWidth);
                        if (ImGui::Button("-", ImVec2{ lineHeight, lineHeight }))
                        {
                            entityHandle.RemoveComponent<Component>();
                        }
                    }

                    if (nodeOpen)
                    {
                        ImGui::TreePop();

                        if (std::is_same_v<Component, ComponentCamera> &&
                            entityHandle.HasComponent<ComponentCamera>() &&
                            ImGui::Button("Switch To"))
                        {
                            Scenes::GetCurrentScene()->SetCurrentCameraEntity(entityHandle);
                        }

                        if (Inspector::InspectObject(component) &&
                            std::is_same_v<Component, ComponentScript>)
                        {
                            ComponentScript& script = m_CurrentEntity.GetComponent<ComponentScript>();

                            // #TODO Review script.AddScript(pair.first) use rather than below logic
                            for (auto& pair : script.ScriptInstances())
                            {
                                if (pair.second && !pair.second->GetEntity())
                                {
                                    pair.second->SetEntity(EntityHandle(m_CurrentEntity.GetScene(), m_CurrentEntity.EntityGuid()));
                                    pair.second->OnCreate(); // #TODO OnCreate() nested calls can remove component and break pair iterator
                                    break;
                                }
                            }
                            script.Bind(m_CurrentEntity);
                        }
                    }
                }(), ...);
            }

            template<typename... Components>
            void priv_EditComponentEntries(TemplateArgumentList<Components...>, EntityHandle entityHandle)
            {
                priv_EditComponentEntry<Components...>(entityHandle);
            }

            void OnEntitySelected(EntityHandle& entity) override { m_CurrentEntity = entity; }

            EntityHandle m_CurrentEntity;
		};

	}

}