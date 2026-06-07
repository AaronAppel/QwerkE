#pragma once

// #NOTE Color3 and Color4 can be deprecated after switching to custom vec3 and vec4 types for QwerkE
// #include "QF_Vector3.h"
// #include "QF_Vector4.h"

namespace QwerkE {

	struct Color3
	{
		Color3() = default;

		Color3(float aRed, float aGreen, float aBlue) :
			r(aRed), g(aGreen), b(aBlue)
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
		union {
			struct {
				float r, g, b, a;
			};

			struct {
				float h, s, v;
			};
		};

		Color4() = default;

		Color4(const vec3f& aOther) :
			r(aOther.r), g(aOther.g), b(aOther.b)
		{
		}

		Color4(const vec3f& aOther, float aAlpha) :
			r(aOther.x), g(aOther.y), b(aOther.z), a(aAlpha)
		{
		}

		Color4(float aX, float aY, float aZ, float aAlpha) :
			r(aX), g(aY), b(aZ), a(aAlpha)
		{
		}

		Color4 operator*(float aValue) const {
			return { r * aValue, g * aValue, b * aValue, a };
		}

		Color4 operator+(const Color4& aOther) const {
			return { r + aOther.r, g + aOther.g, b + aOther.b, a + aOther.a };
		}
	};

}
