#include "imgui_extended.h"

#ifdef IMUI_EXTENDED_WIP

static void ImGui::AddLayoutSpring(ImGuiLayout& layout, float weight, float spacing)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    ImGuiLayoutItem* previous_item = &layout.Items[layout.CurrentItemIndex];

    // Undo item padding, spring should consume all space between items.
    if (layout.Type == ImGuiLayoutType_Horizontal)
        window->DC.CursorPos.x = previous_item->MeasuredBounds.Max.x;
    else
        window->DC.CursorPos.y = previous_item->MeasuredBounds.Max.y;

    previous_item = NULL; // may be invalid after call to GenerateLayoutItem()

    EndLayoutItem(layout);

    ImGuiLayoutItem* spring_item = GenerateLayoutItem(layout, ImGuiLayoutItemType_Spring);

    spring_item->MeasuredBounds.Min = spring_item->MeasuredBounds.Max = window->DC.CursorPos;

    if (weight < 0.0f)
        weight = 0.0f;

    if (spring_item->SpringWeight != weight)
        spring_item->SpringWeight = weight;

    if (spacing < 0.0f)
    {
        ImVec2 style_spacing = g.Style.ItemSpacing;
        if (layout.Type == ImGuiLayoutType_Horizontal)
            spacing = style_spacing.x;
        else
            spacing = style_spacing.y;
    }

    if (spring_item->SpringSpacing != spacing)
        spring_item->SpringSpacing = spacing;

    if (spring_item->SpringSize > 0.0f || spacing > 0.0f)
    {
        ImVec2 spring_size, spring_spacing;
        if (layout.Type == ImGuiLayoutType_Horizontal)
        {
            spring_spacing = ImVec2(0.0f, g.Style.ItemSpacing.y);
            spring_size = ImVec2(spacing + spring_item->SpringSize, layout.CurrentSize.y);
        }
        else
        {
            spring_spacing = ImVec2(g.Style.ItemSpacing.x, 0.0f);
            spring_size = ImVec2(layout.CurrentSize.x, spacing + spring_item->SpringSize);
        }

        PushStyleVar(ImGuiStyleVar_ItemSpacing, ImFloor(spring_spacing));
        Dummy(ImFloor(spring_size));
        PopStyleVar();
    }

    layout.CurrentItemIndex++;

    BeginLayoutItem(layout);
}

// Inserts spring separator in layout
//      weight <= 0     : spring will always have zero size
//      weight > 0      : power of current spring
//      spacing < 0     : use default spacing if pos_x == 0, no spacing if pos_x != 0
//      spacing >= 0    : enforce spacing amount
void ImGui::Spring(float weight/* = 1.0f*/, float spacing/* = -1.0f*/)
{
    ImGuiWindow* window = GetCurrentWindow();
    IM_ASSERT(window->DC.CurrentLayout);

    AddLayoutSpring(*window->DC.CurrentLayout, weight, spacing);
}

#endif // IMUI_EXTENDED_WIP
