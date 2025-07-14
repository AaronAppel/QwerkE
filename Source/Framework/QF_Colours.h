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
	};

}
