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
    void DrawEditComponentT(EntityHandle& entity)
    {
        if (entity.HasComponent<T>())
        {
            const float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
            T& component = entity.GetComponent<T>();

            const Mirror::TypeInfo* typeInfo = Mirror::InfoForType<T>();

            if (ImGui::CollapsingHeader(typeInfo->stringName.c_str(), ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_None))
            {
                Inspector::InspectObject(component, "Entity Editor");
            }

            ImGui::SameLine(ImGui::GetContentRegionAvail().x - lineHeight * 0.5f);
            if (ImGui::Button("-", ImVec2{ lineHeight, lineHeight }))
            {
                // ImGui::OpenPopup("ComponentSettings");
            }
        }
    }

    void EditComponent::Draw(EntityHandle& entity)
    {
        if (!entity)
            return;

        // DrawEditComponent(ComponentInfo)
        // DrawEditComponent(ComponentTransform)
        DrawEditComponentT<ComponentTransform>(entity);

        DrawEditComponentT<ComponentCamera>(entity);
        DrawEditComponentT<ComponentLight>(entity);
        DrawEditComponentT<ComponentMesh>(entity);
        DrawEditComponentT<ComponentScript>(entity);

        // DrawEditComponent(ComponentCamera)
        // DrawEditComponent(ComponentLight)
        // DrawEditComponent(ComponentMesh)
        // DrawEditComponent(ComponentScript)

        m_LastEntity = entity;
    }

}
