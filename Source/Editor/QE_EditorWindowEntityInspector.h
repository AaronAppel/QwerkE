#pragma once

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
#endif

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
                if (!m_CurrentSelectedEntity)
                {
                    // #TODO Also need to serialize the scene of last selected entity
                    if (Scene* currentScene = Scenes::GetCurrentScene())
                    {
                        if (GUID::Invalid != m_LastSelectedEntityGuid)
                        {
                            m_CurrentSelectedEntity = currentScene->GetEntityByGuid(m_LastSelectedEntityGuid);
                        }
                        else
                        {
                            auto viewTransforms = currentScene->ViewComponents<ComponentTransform>();
                            m_CurrentSelectedEntity = EntityHandle(currentScene, viewTransforms[0]); // #TODO Error checking
                        }
                        m_LastSelectedEntityGuid = m_CurrentSelectedEntity ? m_CurrentSelectedEntity.EntityGuid() : GUID::Invalid;
                    }
                }

                if (!m_CurrentSelectedEntity)
                {
                    ImGui::Text("Null current entity");
                    return;
                }

                // #TODO Could also use same hotkey in scene graph to disable from there. Probably better
                const EditorWindow* focusedWindow = Editor::GetLastFocusedWindow();
                if (ImGui::IsWindowFocused() || focusedWindow && EditorWindowTypes::SceneGraph == (u32)focusedWindow->Type())
                {
                    if (IsHotkeyPressed(e_SceneGraphToggleActive))
                    {
                        m_CurrentSelectedEntity.SetIsEnabled(!m_CurrentSelectedEntity.IsEnabled());
                        HotkeyPoll(e_SceneGraphToggleActive); // #TODO Poll resets m_IsChanged flag. Find a better way to do that
                    }
                }

                {   // Edit entity name
                    Buffer buffer(INT8_MAX); // #TODO Could be re-used/persistent and updated on entity change
                    buffer.Fill('\0');
                    strcpy(buffer.As<char>(), m_CurrentSelectedEntity.EntityName().c_str());

                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x / 1.8f); // #NOTE Aesthetic scalar value
                    if (ImGui::InputText(("##" + m_WindowName + "EntityName").c_str(), buffer.As<char>(), buffer.SizeInBytes()))
                    {
                        m_CurrentSelectedEntity.GetComponent<ComponentInfo>().m_EntityName = buffer.As<char>();
                    }
                }

                const char* enabledState = m_CurrentSelectedEntity.IsEnabled() ? "Deactivate" : "Activate";
                ImGui::Separator();
                if (ImGui::Button(enabledState))
                {
                    m_CurrentSelectedEntity.SetIsEnabled(!m_CurrentSelectedEntity.IsEnabled());
                }
                ImGui::SameLine();
                if (ImGui::Button("Destroy"))
                {
                    if (Scene* entityScene = m_CurrentSelectedEntity.GetScene())
                    {
                        entityScene->DestroyEntity(m_CurrentSelectedEntity);
                        m_CurrentSelectedEntity.Invalidate();
                        m_LastSelectedEntityGuid = GUID::Invalid;
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
                    priv_AddComponentEntries(EntityComponentsList{}, m_CurrentSelectedEntity);
                    ImGui::EndPopup();
                }

                priv_EditComponentEntries(EntityComponentsList{}, m_CurrentSelectedEntity);
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
                        else if (std::is_same_v<Component, ComponentAudio>)
                        {
                            ComponentAudio* audio = (ComponentAudio*)&component;
                            audio->Initialize();
                        }
                        else if (std::is_same_v<Component, ComponentPhysics>)
                        {
                            ComponentPhysics* physics = (ComponentPhysics*)&component;
                            physics->Initialize(entityHandle.GetScene());
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
                    const bool nodeOpen = ImGui::TreeNodeEx((void*)typeid(Component).hash_code(), treeNodeFlags, typeInfo->stringName.c_str()); // #TODO Review typeid().hash_code()

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
                            ComponentScript& script = m_CurrentSelectedEntity.GetComponent<ComponentScript>();

                            // #TODO Review script.AddScript(pair.first) use rather than below logic
                            for (auto& pair : script.ScriptInstances())
                            {
                                if (pair.second && !pair.second->GetEntity())
                                {
                                    pair.second->SetEntity(EntityHandle(m_CurrentSelectedEntity.GetScene(), m_CurrentSelectedEntity.EntityGuid()));
                                    pair.second->OnCreate(); // #TODO OnCreate() nested calls can remove component and break pair iterator
                                    break;
                                }
                            }
                            script.Bind(m_CurrentSelectedEntity);
                        }
                    }
                }(), ...);
            }

            template<typename... Components>
            void priv_EditComponentEntries(TemplateArgumentList<Components...>, EntityHandle entityHandle)
            {
                priv_EditComponentEntry<Components...>(entityHandle);
            }

            void OnEntitySelected(EntityHandle& entity) override
            {
                m_CurrentSelectedEntity = entity;
                m_LastSelectedEntityGuid = m_CurrentSelectedEntity ? m_CurrentSelectedEntity.EntityGuid() : GUID::Invalid;
            }

            MIRROR_PRIVATE_MEMBERS

            EntityHandle m_CurrentSelectedEntity;

            GUID m_LastSelectedEntityGuid = GUID::Invalid;
		};

	}

}