#include "QE_EditorWindowImGuiExtendedDemo.h"

#include "Libraries/imgui-knobs/imgui-knobs.h"

#include "Libraries/ImCoolBar/ImCoolBar.h"
#include "Libraries/imgui_toggle/imgui_toggle.h"
#include "Libraries/imgui_toggle/imgui_toggle_presets.h"

#include "Libraries/ImGuizmo/ImZoomSlider.h"

#define IMSPINNER_DEMO
#include "Libraries/imspinner/imspinner.h"

namespace QwerkE {

	namespace Editor {

        static void local_DrawKnobs();
        static bool s_IsShowingKnobs = true;

		static void imgui_toggle_example();
		static void imgui_toggle_simple();
		static void imgui_toggle_custom();
		static void imgui_toggle_state(const ImGuiToggleConfig& config, ImGuiToggleStateConfig& state);
        static bool s_IsShowingToggles = true;

        static void local_DrawImCoolBar();
        static bool s_IsShowingCoolBar = true;

        static bool s_IsShowingSpinnersDemo = true;
        static bool m_IsShowingPieSelector = true;
        static bool m_IsShowingScrollers = true;
        static bool m_IsShowingProgressIndicators = true;
        static bool m_IsShowingImZoomSlider = false; // Not sure how it's supposed to work

		void EditorWindowImGuiExtendedDemo::DrawInternal()
		{
            // #NOTe More widgets, libraries, etc can be found here:
            // - https://github.com/ocornut/imgui/wiki/Useful-Extensions
            // - https://github.com/TimoSalomaki/awesome-dear-imgui

            ImGui::Checkbox("Knobs", &s_IsShowingKnobs);
            ImGui::SameLine();
            ImGui::Checkbox("Toggles", &s_IsShowingToggles);
            ImGui::SameLine();
            ImGui::Checkbox("Cool Bar", &s_IsShowingCoolBar);
            ImGui::SameLine();
            ImGui::Checkbox("Pie Select", &m_IsShowingPieSelector);
            ImGui::SameLine();
            ImGui::Checkbox("Scrollers", &m_IsShowingScrollers);
            ImGui::SameLine();
            ImGui::Checkbox("Progress", &m_IsShowingProgressIndicators);
            ImGui::SameLine();
            ImGui::Checkbox("ZoomSlider", &m_IsShowingImZoomSlider);
            ImGui::Separator();

            if (ImGui::CollapsingHeader("Spinners"))
            {
                ImSpinner::demoSpinners();
                ImGui::Separator();
            }

            if (s_IsShowingKnobs)
            {
                local_DrawKnobs();
                ImGui::Separator();
            }

            if (s_IsShowingToggles)
            {
                static bool toggleExample = false;
                ImGui::Toggle("##AnimatedToggle", &toggleExample, ImGuiToggleFlags_Animated);
                ImGui::SameLine();
                ImGui::Text("Toggles");
                if (ImGui::CollapsingHeader("Example"))
                {
                    imgui_toggle_example();
                }
                if (ImGui::CollapsingHeader("Simple"))
                {
                    imgui_toggle_simple();
                }
                if (ImGui::CollapsingHeader("Custom"))
                {
                    imgui_toggle_custom();
                }
                // imgui_toggle_state(const ImGuiToggleConfig & config, ImGuiToggleStateConfig & state);
                ImGui::Separator();
            }

            if (s_IsShowingCoolBar)
            {
                local_DrawImCoolBar();
            }

            if (m_IsShowingPieSelector)
            {
                // From: https://github.com/ocornut/imgui/issues/434
                static const char* test_data = "Menu";
                const char* items[] = { "Orange", "Blue", "Purple", "Gray", "Yellow", "Las Vegas" };
                int items_count = sizeof(items) / sizeof(*items);

                static int selected = -1;

                ImGui::Button(selected >= 0 ? items[selected] : "Menu", ImVec2(50, 50));
                if (ImGui::IsItemActive())          // Don't wait for button release to activate the pie menu
                    ImGui::OpenPopup("##piepopup");

                ImVec2 pie_menu_center = ImGui::GetIO().MouseClickedPos[0];
                int n = ImGui::PiePopupSelectMenu(pie_menu_center, "##piepopup", items, items_count, &selected);
                // if (n >= 0)
                //     printf("returned %d\n", n);
                ImGui::Separator();
            }

            if (m_IsShowingScrollers)
            {
                static int spinInt = 0;
                static float spinFloat = 0.f;
                static double spinDouble = 0;

                ImGui::PushItemWidth(150.f);
                ImGui::ScrollerInt("Spin Int", &spinInt);
                ImGui::ScrollerFloat("Spin Float", &spinFloat);
                ImGui::ScrollerDouble("Spin Double", &spinDouble);
                ImGui::PopItemWidth();
                ImGui::Separator();
            }

            if (m_IsShowingProgressIndicators)
            {
                const ImU32 col = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
                const ImU32 bg = ImGui::GetColorU32(ImGuiCol_Button);

                const float indicator_radius = 45.f;
                const ImVec4 main_color = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
                const ImVec4 backdrop_color = ImVec4(0.25f, .75f, 1.0f, 1.0f);

                const int circle_count = 25;
                static float speed = 5.f;

                ImGui::Knob("IndicatorSpeedKnob", &speed, 0.f, 10.f, .05f);
                ImGui::SameLine();
                ImGui::LoadingIndicatorCircle("##LoadingCirclesIndicator",
                    indicator_radius,
                    main_color,
                    backdrop_color,
                    circle_count, speed
                );

                ImGui::Spinner("##spinner", 15, 6, col);
                ImGui::BufferingBar("##buffer_bar", 0.7f, ImVec2(400, 6), bg, col);
                ImGui::Text("Loading %c", "|/-\\"[(int)(ImGui::GetTime() / 0.05f) & 3]);
                ImGui::Separator();

                static float progress = 0.5f;
                ImGui::DragFloat("##", &progress);
                ImGui::ProgressBar(progress);
            }

            if (m_IsShowingImZoomSlider)
            {
                ImGui::SameLine();
                ImGui::PushID(18);
                static float viewLower = 1.0f;
                static float viewHigher = 100.0f;
                ImZoomSlider::ImZoomSlider(0.f, 1.f, viewLower, viewHigher, 0.01f, ImZoomSlider::ImGuiZoomSliderFlags_Vertical);
                ImGui::PopID();
            }

            ImGui::Bullet();
            ImGui::Text("Bullet");
		}

        void local_DrawKnobs()
        {
            static float val1 = 0;
            if (ImGuiKnobs::Knob("Gain", &val1, -6.0f, 6.0f, 0.1f, "%.1fdB", ImGuiKnobVariant_Tick)) {
                // value was changed
            }

            ImGui::SameLine();

            static float val2 = 0;
            if (ImGuiKnobs::Knob("Mix", &val2, -1.0f, 1.0f, 0.1f, "%.1f", ImGuiKnobVariant_Stepped)) {
                // value was changed
            }

            // Double click to reset
            if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0)) {
                val2 = 0;
            }

            ImGui::SameLine();

            static float val3 = 0;

            // Custom colors
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(255.f, 0, 0, 0.7f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(255.f, 0, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 255.f, 0, 1));
            // Push/PopStyleColor() for each colors used (namely ImGuiCol_ButtonActive and ImGuiCol_ButtonHovered for primary and ImGuiCol_Framebg for Track)
            if (ImGuiKnobs::Knob("Pitch", &val3, -6.0f, 6.0f, 0.1f, "%.1f", ImGuiKnobVariant_WiperOnly)) {
                // value was changed
            }

            ImGui::PopStyleColor(3);


            ImGui::SameLine();

            // Custom min/max angle
            static float val4 = 0;
            if (ImGuiKnobs::Knob("Dry", &val4, -6.0f, 6.0f, 0.1f, "%.1f", ImGuiKnobVariant_Stepped, 0, 0, 10, 1.570796f, 3.141592f)) {
                // value was changed
            }

            ImGui::SameLine();

            // Int value
            static int val5 = 1;
            if (ImGuiKnobs::KnobInt("Wet", &val5, 1, 10, 0.1f, "%i", ImGuiKnobVariant_Stepped, 0, 0, 10)) {
                // value was changed
            }

            ImGui::SameLine();

            // Vertical drag only
            static float val6 = 1;
            if (ImGuiKnobs::Knob("Vertical", &val6, 0.f, 10.f, 0.1f, "%.1f", ImGuiKnobVariant_Space, 0, ImGuiKnobFlags_DragVertical)) {
                // value was changed
            }

            ImGui::SameLine();

            static float val7 = 500.0f;
            if (ImGuiKnobs::Knob("Logarithmic", &val7, 20, 20000, 20.0f, "%.1f", ImGuiKnobVariant_WiperOnly, 0, ImGuiKnobFlags_Logarithmic | ImGuiKnobFlags_AlwaysClamp)) {
                // value was changed
            }
        }

        // From: https://github.com/cmdwtf/imgui_toggle/blob/main/EXAMPLE.md
        static void imgui_toggle_example()
        {
            // use some lovely gray backgrounds for "off" toggles
            // the default would otherwise use your theme's frame background colors.
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.45f, 0.45f, 0.45f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.65f, 0.65f, 0.65f, 1.0f));

            // a toggle that will allow the user to view the demo for simple toggles or a custom toggle
            static bool show_custom_toggle = true;
            ImGui::Toggle(
                show_custom_toggle ? "Showing Custom Toggle" : "Showing Simple Toggles"
                , &show_custom_toggle);

            ImGui::Separator();

            if (show_custom_toggle)
            {
                imgui_toggle_custom();
            }
            else
            {
                imgui_toggle_simple();
            }

            // pop the color styles
            ImGui::PopStyleColor(2);
        }

        // From: https://github.com/cmdwtf/imgui_toggle/blob/main/EXAMPLE.md
        static void imgui_toggle_simple()
        {
            static bool values[] = { true, true, true, true, true, true, true, true };
            size_t value_index = 0;

            const ImVec4 green(0.16f, 0.66f, 0.45f, 1.0f);
            const ImVec4 green_hover(0.0f, 1.0f, 0.57f, 1.0f);
            const ImVec4 salmon(1.0f, 0.43f, 0.35f, 1.0f);
            const ImVec4 green_shadow(0.0f, 1.0f, 0.0f, 0.4f);

            // a default and default animated toggle
            ImGui::Toggle("Default Toggle", &values[value_index++]);
            ImGui::Toggle("Animated Toggle", &values[value_index++], ImGuiToggleFlags_Animated);

            // this toggle draws a simple border around it's frame and knob
            ImGui::Toggle("Bordered Knob", &values[value_index++], ImGuiToggleFlags_Bordered, 1.0f);

            // this toggle draws a simple shadow around it's frame and knob
            ImGui::PushStyleColor(ImGuiCol_BorderShadow, green_shadow);
            ImGui::Toggle("Shadowed Knob", &values[value_index++], ImGuiToggleFlags_Shadowed, 1.0f);

            // this toggle draws the shadow & and the border around it's frame and knob.
            ImGui::Toggle("Bordered + Shadowed Knob", &values[value_index++], ImGuiToggleFlags_Bordered | ImGuiToggleFlags_Shadowed, 1.0f);
            ImGui::PopStyleColor(1);

            // this toggle uses stack-pushed style colors to change the way it displays
            ImGui::PushStyleColor(ImGuiCol_Button, green);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, green_hover);
            ImGui::Toggle("Green Toggle", &values[value_index++]);
            ImGui::PopStyleColor(2);

            ImGui::Toggle("Toggle with A11y Labels", &values[value_index++], ImGuiToggleFlags_A11y);

            // this toggle shows no label
            ImGui::Toggle("##Toggle With Hidden Label", &values[value_index++]);
        }

        // From: https://github.com/cmdwtf/imgui_toggle/blob/main/EXAMPLE.md
        static void imgui_toggle_custom()
        {
            static ImGuiToggleConfig config;
            static bool toggle_custom = true;

            ImGui::NewLine();

            ImGui::Toggle("Customized Toggle", &toggle_custom, config);

            ImGui::NewLine();

            // these first settings are used no matter the toggle's state.
            ImGui::Text("Persistent Toggle Settings");

            // animation duration controls how long the toggle animates, in seconds. if set to 0, animation is disabled.
            if (ImGui::SliderFloat("Animation Duration (seconds)", &config.AnimationDuration, ImGuiToggleConstants::AnimationDurationMinimum, 2.0f))
            {
                // if the user adjusted the animation duration slider, go ahead and turn on the animation flags.
                config.Flags |= ImGuiToggleFlags_Animated;
            }

            // frame rounding sets how round the frame is when drawn, where 0 is a rectangle, and 1 is a circle.
            ImGui::SliderFloat("Frame Rounding (scale)", &config.FrameRounding, ImGuiToggleConstants::FrameRoundingMinimum, ImGuiToggleConstants::FrameRoundingMaximum);

            // knob rounding sets how round the knob is when drawn, where 0 is a rectangle, and 1 is a circle.
            ImGui::SliderFloat("Knob Rounding (scale)", &config.KnobRounding, ImGuiToggleConstants::KnobRoundingMinimum, ImGuiToggleConstants::KnobRoundingMaximum);

            // size controls the width and the height of the toggle frame
            ImGui::SliderFloat2("Size (px: w, h)", &config.Size.x, 0.0f, 200.0f, "%.0f");

            // width ratio sets how wide the toggle is with relation to the frame height. if Size is non-zero, this is unused.
            ImGui::SliderFloat("Width Ratio (scale)", &config.WidthRatio, ImGuiToggleConstants::WidthRatioMinimum, ImGuiToggleConstants::WidthRatioMaximum);

            // a11y style sets the type of additional on/off indicator drawing
            if (ImGui::Combo("A11y Style", &config.A11yStyle,
                "Label\0"
                "Glyph\0"
                "Dot\0"
                "\0"))
            {
                // if the user adjusted the a11y style combo, go ahead and turn on the a11y flag.
                config.Flags |= ImGuiToggleFlags_A11y;
            }

            // some tabs to adjust the "state" settings of the toggle (configuration dependent on if the toggle is on or off.)
            if (ImGui::BeginTabBar("State"))
            {
                if (ImGui::BeginTabItem("\"Off State\" Settings"))
                {
                    imgui_toggle_state(config, config.Off);
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("\"On State\"Settings"))
                {
                    imgui_toggle_state(config, config.On);
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }

            ImGui::Separator();

            // flags for various toggle features
            ImGui::Text("Flags");
            ImGui::Columns(2);
            ImGui::Text("Meta Flags");
            ImGui::NextColumn();
            ImGui::Text("Individual Flags");
            ImGui::Separator();
            ImGui::NextColumn();

            // should the toggle have borders (sets all border flags)
            ImGui::CheckboxFlags("Bordered", &config.Flags, ImGuiToggleFlags_Bordered);

            // should the toggle have shadows (sets all shadow flags)
            ImGui::CheckboxFlags("Shadowed", &config.Flags, ImGuiToggleFlags_Shadowed);

            ImGui::NextColumn();

            // should the toggle animate
            ImGui::CheckboxFlags("Animated", &config.Flags, ImGuiToggleFlags_Animated);

            // should the toggle have a bordered frame
            ImGui::CheckboxFlags("BorderedFrame", &config.Flags, ImGuiToggleFlags_BorderedFrame);

            // should the toggle have a bordered knob
            ImGui::CheckboxFlags("BorderedKnob", &config.Flags, ImGuiToggleFlags_BorderedKnob);

            // should the toggle have a shadowed frame
            ImGui::CheckboxFlags("ShadowedFrame", &config.Flags, ImGuiToggleFlags_ShadowedFrame);

            // should the toggle have a shadowed knob
            ImGui::CheckboxFlags("ShadowedKnob", &config.Flags, ImGuiToggleFlags_ShadowedKnob);

            // should the toggle draw a11y glyphs
            ImGui::CheckboxFlags("A11y", &config.Flags, ImGuiToggleFlags_A11y);
            ImGui::Columns();

            ImGui::Separator();

            // what follows are some configuration presets. check the source of those functions to see how they work.
            ImGui::Text("Configuration Style Presets");

            if (ImGui::Button("Reset to Default"))
            {
                config = ImGuiTogglePresets::DefaultStyle();
            }
            ImGui::SameLine();

            if (ImGui::Button("Rectangle"))
            {
                config = ImGuiTogglePresets::RectangleStyle();
            }
            ImGui::SameLine();

            if (ImGui::Button("Glowing"))
            {
                config = ImGuiTogglePresets::GlowingStyle();
            }
            ImGui::SameLine();

            if (ImGui::Button("iOS"))
            {
                config = ImGuiTogglePresets::iOSStyle();
            }
            ImGui::SameLine();

            if (ImGui::Button("Material"))
            {
                config = ImGuiTogglePresets::MaterialStyle();
            }
            ImGui::SameLine();

            if (ImGui::Button("Minecraft"))
            {
                config = ImGuiTogglePresets::MinecraftStyle();
            }
        }

        // From: https://github.com/cmdwtf/imgui_toggle/blob/main/EXAMPLE.md
        static void imgui_toggle_state(const ImGuiToggleConfig& config, ImGuiToggleStateConfig& state)
        {
            // some values to use for slider limits
            const float border_thickness_max_pixels = 50.0f;
            const float max_height = config.Size.y > 0 ? config.Size.y : ImGui::GetFrameHeight();
            const float half_max_height = max_height * 0.5f;

            // knob offset controls how far into or out of the frame the knob should draw.
            ImGui::SliderFloat2("Knob Offset (px: x, y)", &state.KnobOffset.x, -half_max_height, half_max_height);

            // knob inset controls how many pixels the knob is set into the frame. negative values will cause it to grow outside the frame.
            // for circular knobs, we will just use a single value, while for we will use top/left/bottom/right offsets.
            const bool is_rounded = config.KnobRounding >= 1.0f;
            if (is_rounded)
            {
                float inset_average = state.KnobInset.GetAverage();
                ImGui::SliderFloat("Knob Inset (px)", &inset_average, -half_max_height, half_max_height);
                state.KnobInset = inset_average;
            }
            else
            {
                ImGui::SliderFloat4("Knob Inset (px: t, l, b, r)", state.KnobInset.Offsets, -half_max_height, half_max_height);
            }

            // how thick should the frame border be (if enabled)
            ImGui::SliderFloat("Frame Border Thickness (px)", &state.FrameBorderThickness, 0.0f, border_thickness_max_pixels);

            // how thick should the knob border be (if enabled)
            ImGui::SliderFloat("Knob Border Thickness (px)", &state.KnobBorderThickness, 0.0f, border_thickness_max_pixels);
        }

        // From: https://github.com/aiekick/ImCoolBar
        auto coolbar_button = [](const char* label) -> bool {
            float w = ImGui::GetCoolBarItemWidth();
            auto font_ptr = ImGui::GetIO().Fonts->Fonts[0];
            font_ptr->Scale = ImGui::GetCoolBarItemScale();
            ImGui::PushFont(font_ptr);
            bool res = ImGui::Button(label, ImVec2(w, w));
            ImGui::PopFont();
            return res;
            };

        // From: https://github.com/aiekick/ImCoolBar
        static void local_DrawImCoolBar()
        {
            if (ImGui::BeginCoolBar("##CoolBarMain", ImCoolBarFlags_::ImCoolBarFlags_Horizontal, ImVec2(0.5f, 1.0f))) {
                if (ImGui::CoolBarItem()) {
                    if (coolbar_button("A")) {

                    }
                }
                if (ImGui::CoolBarItem()) {
                    if (coolbar_button("B")) {

                    }
                }
                if (ImGui::CoolBarItem()) {
                    if (coolbar_button("C")) {

                    }
                }
                if (ImGui::CoolBarItem()) {
                    if (coolbar_button("D")) {

                    }
                }
                if (ImGui::CoolBarItem()) {
                    if (coolbar_button("E")) {

                    }
                }
                if (ImGui::CoolBarItem()) {
                    if (coolbar_button("F")) {

                    }
                }
                if (ImGui::CoolBarItem()) {
                    if (coolbar_button("G")) {

                    }
                }
                if (ImGui::CoolBarItem()) {
                    if (coolbar_button("H")) {

                    }
                }
                if (ImGui::CoolBarItem()) {
                    if (coolbar_button("I")) {

                    }
                }
                if (ImGui::CoolBarItem()) {
                    if (coolbar_button("J")) {

                    }
                }
                if (ImGui::CoolBarItem()) {
                    if (coolbar_button("K")) {

                    }
                }
                if (ImGui::CoolBarItem()) {
                    if (coolbar_button("L")) {

                    }
                }
                if (ImGui::CoolBarItem()) {
                    if (coolbar_button("M")) {

                    }
                }
                ImGui::EndCoolBar();
            }
        }

	}

}