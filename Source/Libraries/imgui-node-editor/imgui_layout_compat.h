#pragma once

#include <imgui.h>
#include <vector>

// Legacy support for migrating to new imgui v1.92.6 WIP

// Additional fixes:
/*
Also replace:
    ImGui::BeginHorizontal(id.AsPointer());
     with
    ImGui::PushID(id.AsPointer());
    ImGui::BeginHorizontal("##horizontal");
*/

namespace ImGui
{
    // ------------------------------------------------------------
    // Spring (flexible spacing)
    // ------------------------------------------------------------
    inline void Spring(float weight = 1.0f, float spacing = 0.0f)
    {
        float width = ImGui::GetContentRegionAvail().x;

        if (width > 0.0f)
            ImGui::Dummy(ImVec2(width * weight, 0.0f));

        if (spacing > 0.0f)
            ImGui::SameLine(0.0f, spacing);
    }

    // ------------------------------------------------------------
    // Horizontal layout
    // ------------------------------------------------------------
    struct HorizontalLayout
    {
        bool first = true;
        bool isChild = false;
    };

    inline std::vector<HorizontalLayout>& GetHorizontalStack()
    {
        static std::vector<HorizontalLayout> stack;
        return stack;
    }

    inline bool BeginHorizontal(const char* str_id = nullptr,
        const ImVec2& size = ImVec2(0, 0),
        float /*align*/ = 0.0f)
    {
        if (str_id)
            ImGui::PushID(str_id);

        bool useChild = (size.x != 0.0f || size.y != 0.0f);

        if (useChild)
            ImGui::BeginChild(str_id ? str_id : "##horizontal", size, false);
        else
            ImGui::BeginGroup();

        GetHorizontalStack().push_back({ true, useChild });
        return true;
    }

    inline void EndHorizontal()
    {
        auto& stack = GetHorizontalStack();
        if (!stack.empty())
        {
            if (stack.back().isChild)
                ImGui::EndChild();
            else
                ImGui::EndGroup();

            stack.pop_back();
        }

        ImGui::PopID();
    }

    inline void NextColumn()
    {
        auto& stack = GetHorizontalStack();
        if (!stack.empty())
        {
            auto& layout = stack.back();
            if (!layout.first)
                ImGui::SameLine();
            layout.first = false;
        }
    }

    // ------------------------------------------------------------
    // Vertical layout
    // ------------------------------------------------------------
    struct VerticalLayout
    {
        bool isChild;
    };

    inline std::vector<VerticalLayout>& GetVerticalStack()
    {
        static std::vector<VerticalLayout> stack;
        return stack;
    }

    inline bool BeginVertical(const char* str_id = nullptr,
        const ImVec2& size = ImVec2(0, 0),
        float /*align*/ = 0.0f)
    {
        if (str_id)
            ImGui::PushID(str_id);

        bool useChild = (size.x != 0.0f || size.y != 0.0f);

        if (useChild)
            ImGui::BeginChild(str_id ? str_id : "##vertical", size, false);
        else
            ImGui::BeginGroup();

        GetVerticalStack().push_back({ useChild });
        return true;
    }

    inline void EndVertical()
    {
        auto& stack = GetVerticalStack();
        if (!stack.empty())
        {
            if (stack.back().isChild)
                ImGui::EndChild();
            else
                ImGui::EndGroup();

            stack.pop_back();
        }

        ImGui::PopID();
    }
}
