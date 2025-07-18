#pragma once

// #TODO Improve
#include <iostream>
#include <cmath>
#include <array>

#include "QC_TypeDefs.h"

namespace QwerkE {

	namespace Math {

#define M_PI       3.14159265358979323846f   // pi
#define DEG(x) ((x) * 180.0f / M_PI)

		constexpr double PI() { return 3.1415926535897932384626433832795; }
		constexpr float PI_f() { return 3.1415926535897932384626433832795f; }

		#define DEG_TO_RAD  0.01745329251994329576923690768489
		#define RAD_TO_DEG  57.295779513082320876798154814105

		inline constexpr float DegToRad(const float degrees)
		{
			return degrees * M_PI / 180.f;
		}

		inline constexpr float RadToDeg(const float radians)
		{
			return radians * 180.f / M_PI;
		}

		template <typename T>
		inline T Abs(T value)
		{
			if (value < (T)0)
			{
				return -value;
			}
			return value;
		}

		template <typename T>
		inline T Clamp(T min, T value, T max)
		{
			if (value < min)
			{
				return min;
			}
			else if (value > max)
			{
				return max;
			}
			return value;
		}

		template <typename T>
		inline T ClampRollover(T min, T value, T max) // #TODO Add tests
		{
			auto range = max - min + 1; // #NOTE Inclusive

			if (value < min)
			{
				return value + range; // #TODO What if the value is super low so + range still isn't in range?
			}
			else if (value > max)
			{
				return value - range;
			}
			return value;
		}

		inline unsigned char Digits(unsigned long long value)
		{
			unsigned char digits = 1; // #TODO Test
			while (value /= 10)
			{
				++digits;
			}
			return digits;
		}

		inline unsigned long long Pow(int base, const signed int exponent)
		{
			if (exponent == 0) // #TODO Test with (loops a lot with exponent of -2)
				return 1;

			unsigned long long result = base;
			for (signed int i = 0; i < exponent - 1; i++)
			{
				result *= base;
			}
			return result;
		}

		std::array<float, 3> RotationMatrixToEulerZYX(const float R[3][3]);

		inline vec3f MatrixPosition(const float a_Matrix[16]) { return vec3f(a_Matrix[12], a_Matrix[13], a_Matrix[14]); }
		vec3f MatrixRotation(const float a_Matrix[16]);
		vec3f MatrixScale(const float a_Matrix[16]);

		void MatrixRotateAroundY(float a_Matrix[16], const float a_YawDegrees);
		void MatrixRotateAxis(float a_Matrix[16], const vec3f& a_Axis, const float a_Degrees);

#ifdef _QGLM
		inline float Magnitude(const vec2f& a_Other) { return glm::length(a_Other); }
		inline float Magnitude(const vec3f& a_Other) { return glm::length(a_Other); }
#endif // _QGLM

	}

}
