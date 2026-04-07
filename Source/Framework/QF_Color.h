#pragma once

// #NOTE Color3 and Color4 can be deprecated after switching to custom vec3 and vec4 types for QwerkE
// #include "QF_Vector3.h"
// #include "QF_Vector4.h"

namespace QwerkE {

	struct Color3
	{
		Color3() = default;

		Color3(float a_R, float a_G, float a_B) :
			r(a_R), g(a_G), b(a_B)
		{
		}

		union {

			struct {
				float r, g, b;
			};

			struct {
				float h, s, v;
			};
		};
	};

	struct Color4
	{
		Color4() = default;

		Color4(const vec3f& a_Other) :
			r(a_Other.r), g(a_Other.g), b(a_Other.b), a(a_Other.a)
		{
		}

		Color4(const vec3f& a_Other, float a_OtherAlpha) :
			r(a_Other.x), g(a_Other.y), b(a_Other.z), a(a_Other.a)
		{
		}

		Color4(float a_OtherX, float a_OtherY, float a_OtherZ, float a_OtherAlpha) :
			r(a_OtherX), g(a_OtherY), b(a_OtherZ), a(a_OtherAlpha)
		{
		}

		Color4 operator*(float v) const {
			return { r * v, g * v, b * v, a };
		}

		Color4 operator+(const Color4& other) const {
			return { r + other.r, g + other.g, b + other.b, a + other.a };
		}

		union {

			struct {
				float r, g, b, a;
			};

			struct {
				float h, s, v;
			};
		};
	};

}
