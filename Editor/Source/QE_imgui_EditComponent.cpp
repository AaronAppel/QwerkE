#include "QE_EditComponent.h"

#include <map>
#include <string>
#include <vector>

#include "imgui/imgui.h"

#include "QC_StringHelpers.h"

#include "QF_ComponentCamera.h"
#include "QF_ComponentInfo.h"
#include "QF_ComponentLight.h"
#include "QF_ComponentMesh.h"
#include "QF_ComponentScript.h"
#include "QF_ComponentTransform.h"

#include "QF_EntityHandle.h"
#include "QF_Enums.h"
#include "QF_Scenes.h"

#include "QE_EditorInspector.h"

namespace QwerkE {

#define DrawEditComponent(COMPONENT_TYPE) \
        if (entity.HasComponent<COMPONENT_TYPE>()) \
        { \
            COMPONENT_TYPE& component = entity.GetComponent<COMPONENT_TYPE>(); \
            if (ImGui::CollapsingHeader(#COMPONENT_TYPE, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_None)) \
            { \
                Inspector::InspectObject(component, "Entity Editor"); \
            } \
        }

    template <typename T>
    bool DrawEditComponentT(EntityHandle& entity)
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
                return Inspector::InspectObject(component, "Entity Editor");
            }
        }
        return false;
    }

    void EditComponent::Draw(EntityHandle& entity)
    {
        if (!entity)
            return;

        DrawEditComponentT<ComponentTransform>(entity); // Draw above

        DrawEditComponentT<ComponentCamera>(entity);
        DrawEditComponentT<ComponentLight>(entity);
        DrawEditComponentT<ComponentMesh>(entity);
        if (DrawEditComponentT<ComponentScript>(entity))
        {
            ComponentScript& script = entity.GetComponent<ComponentScript>();

            for (auto& pair : script.m_ScriptInstances)
            {
                if (pair.second && !pair.second->GetEntity())
                {
                    pair.second->SetEntity(EntityHandle(Scenes::GetCurrentScene(), entity.EntityGuid()));
                    pair.second->OnCreate(); // #TODO Can remove component and break iterator
                    break;
                }
            }
            script.Bind(entity);
        }

        m_LastEntity = entity;
    }

}
