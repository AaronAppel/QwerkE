#pragma once

namespace QwerkE {

	struct Colours
	{
		struct Colour3
		{
			Colour3() = default;
			Colour3(float a_R, float a_G, float a_B)
			{
				this->r = a_R;
				this->g = a_G;
				this->b = a_B;
			}
			float r = 0.f;
			float g = 0.f;
			float b = 0.f;
		};

		struct Colour4
		{
			Colour4() = default;
			Colour4(const vec3f& a_Other)
			{
				this->r = a_Other.r;
				this->g = a_Other.g;
				this->b = a_Other.b;
			}
			float r = 0.f;
			float g = 0.f;
			float b = 0.f;
			float a = 1.f;
		};

		// #TODO Helpers:
		// - RGB to HSV and vice versa
		// - Int to colour by range vec4f IntToColour(min, value, max), and vec4f IntToColour(float percentage);
		// - Colour categories: Complementary, etc

		// #TODO Decide types and naming
		// #TODO RGB, HSV, etc?

		// #TODO: Green, Blue, Purple, Orange, Brown, Teal, Cyan, Yellow, White, Black, Marsh, Fuschia, (pastels, saturated, dark/light, etc)
		// Can use resources:
		// - https://colorbrewer2.org/#type=sequential&scheme=BuGn&n=3

		// #TODO Reference ImGui::ColorButton, ImGui::ColorConvertHSVtoRGB, and

		static vec3f Red() { return { 1.f, 0.f, 0.f }; }
		static vec4f Red4() { return { Red(), 1.f}; }

		static vec3f Gray() { return { 0.75f, 0.75f, 0.75f }; }
		static vec4f Gray4() { return { Gray(), 1.f}; }

		const vec3f Magenta() { return { 0.85f, 1.f, 0.48f }; };
		const vec4f Magenta4() { return { Magenta(), 1.0f }; };

		// #TODO Colour conversion
		// https://github.com/ChrisG0x20/ColorWheel/blob/master/ColorWheel/main.cpp#L125
		// float arr[4];
		// ImGui::ColorPicker4("Label", arr, ImGuiColorEditFlags_::);
		// // ImGui::ColorTooltip(desc_id, &col.x, flags& (ImGuiColorEditFlags_InputMask_ | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf));
		// // GetColorU32(ImGuiCol_FrameBg)
		// // RenderColorRectWithAlphaCheckerboard(window->DrawList, bb_inner.Min, bb_inner.Max, GetColorU32(col_source), grid_step, ImVec2(off, off), rounding);
		// // ColorConvertHSVtoRGB(col_rgb.x, col_rgb.y, col_rgb.z, col_rgb.x, col_rgb.y, col_rgb.z);
		//
		// // Render Hue Wheel
		// const float aeps = 0.5f / wheel_r_outer; // Half a pixel arc length in radians (2pi cancels out).
		// const int segment_per_arc = ImMax(4, (int)wheel_r_outer / 12);
		// for (int n = 0; n < 6; n++)
		// {
		//     const float a0 = (n) / 6.0f * 2.0f * IM_PI - aeps;
		//     const float a1 = (n + 1.0f) / 6.0f * 2.0f * IM_PI + aeps;
		//     const int vert_start_idx = draw_list->VtxBuffer.Size;
		//     draw_list->PathArcTo(wheel_center, (wheel_r_inner + wheel_r_outer) * 0.5f, a0, a1, segment_per_arc);
		//     draw_list->PathStroke(col_white, 0, wheel_thickness);
		//     const int vert_end_idx = draw_list->VtxBuffer.Size;
		//
		//     // Paint colors over existing vertices
		//     ImVec2 gradient_p0(wheel_center.x + ImCos(a0) * wheel_r_inner, wheel_center.y + ImSin(a0) * wheel_r_inner);
		//     ImVec2 gradient_p1(wheel_center.x + ImCos(a1) * wheel_r_inner, wheel_center.y + ImSin(a1) * wheel_r_inner);
		//     ShadeVertsLinearColorGradientKeepAlpha(draw_list, vert_start_idx, vert_end_idx, gradient_p0, gradient_p1, col_hues[n], col_hues[n + 1]);
		// }
		//
		// // Render Cursor + preview on Hue Wheel
		// float cos_hue_angle = ImCos(H * 2.0f * IM_PI);
		// float sin_hue_angle = ImSin(H * 2.0f * IM_PI);
		// ImVec2 hue_cursor_pos(wheel_center.x + cos_hue_angle * (wheel_r_inner + wheel_r_outer) * 0.5f, wheel_center.y + sin_hue_angle * (wheel_r_inner + wheel_r_outer) * 0.5f);
		// float hue_cursor_rad = value_changed_h ? wheel_thickness * 0.65f : wheel_thickness * 0.55f;
		// int hue_cursor_segments = draw_list->_CalcCircleAutoSegmentCount(hue_cursor_rad); // Lock segment count so the +1 one matches others.
		// draw_list->AddCircleFilled(hue_cursor_pos, hue_cursor_rad, hue_color32, hue_cursor_segments);
		// draw_list->AddCircle(hue_cursor_pos, hue_cursor_rad + 1, col_midgrey, hue_cursor_segments);
		// draw_list->AddCircle(hue_cursor_pos, hue_cursor_rad, col_white, hue_cursor_segments);
		//
		// // Render SV triangle (rotated according to hue)
		// ImVec2 tra = wheel_center + ImRotate(triangle_pa, cos_hue_angle, sin_hue_angle);
		// ImVec2 trb = wheel_center + ImRotate(triangle_pb, cos_hue_angle, sin_hue_angle);
		// ImVec2 trc = wheel_center + ImRotate(triangle_pc, cos_hue_angle, sin_hue_angle);
		// ImVec2 uv_white = GetFontTexUvWhitePixel();
		// draw_list->PrimReserve(3, 3);
		// draw_list->PrimVtx(tra, uv_white, hue_color32);
		// draw_list->PrimVtx(trb, uv_white, col_black);
		// draw_list->PrimVtx(trc, uv_white, col_white);
		// draw_list->AddTriangle(tra, trb, trc, col_midgrey, 1.5f);
		// sv_cursor_pos = ImLerp(ImLerp(trc, tra, ImSaturate(S)), trb, ImSaturate(1 - V));
	};

}
