#pragma once

// #TODO Improve
#include <iostream>
#include <cmath>
#include <array>

#include "QC_TypeDefs.h"

namespace QwerkE {

	namespace Math {

#define ROW_MAJOR 0 // #TODO Use to more easily swap, or even just to confirm matrix operations behaviour

		constexpr float PI = 3.1415927f;
		constexpr double PI_d = 3.14159265358979323846;

		constexpr float ToDeg = 180.f / PI; // 57.295776f
		constexpr float ToRad = PI / 180.f; // 0.017453292f

		constexpr double ToDeg_d = 180 / PI_d; // 57.29577951308232
		constexpr double ToRad_d = PI_d / 180; // 0.017453292519943295

		inline constexpr float DegToRad(const float degrees)
		{
			return degrees * PI / 180.f;
		}

		inline constexpr float RadToDeg(const float radians)
		{
			return radians * 180.f / PI;
		}

		template <typename T>
		inline bool Equal(const T a, const T b, const T tolerance)
		{
			T result = a - b;
			T absResult = result > 0 ? result : result * -1;
			T absTolerance = tolerance > 0 ? tolerance : tolerance * -1;
			return absResult < absTolerance;
		}

		inline bool Equal(const float a, const float b, const float tolerance = 0.0001f) // 1e-4
		{
			float result = a - b;
			float absResult = result > 0 ? result : result * -1;
			float absTolerance = tolerance > 0 ? tolerance : tolerance * -1;
			return absResult < absTolerance;
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

		// Vectors
#ifdef _QGLM
		inline float Magnitude(const vec2f& a_Other) { return glm::length(a_Other); }
		inline float Magnitude(const vec3f& a_Other) { return glm::length(a_Other); }
#endif // _QGLM
		inline float VectorMagnitude(float a_X, float a_Y) { return std::sqrt(a_X * a_X + a_Y * a_Y); }
		inline float VectorMagnitude(float a_X, float a_Y, float a_Z) { return std::sqrt(a_X * a_X + a_Y * a_Y + a_Z * a_Z); }

		// Matrices 3x3
		std::array<float, 3> RotationMatrixToEulerZYX(const float R[3][3]);

		// Matrices 4x4
		inline vec3f MatrixPosition(const float a_Matrix[16]) { return vec3f(a_Matrix[12], a_Matrix[13], a_Matrix[14]); }
		vec3f MatrixRotation(const float a_Matrix[16]);
		vec3f MatrixScale(const float a_Matrix[16]);

		void MatrixRotateAroundY(float a_Matrix[16], const float a_YawDegrees);
		void MatrixRotateAxis(float a_Matrix[16], const vec3f& a_Axis, const float a_Degrees);

		void MatrixRotateAxis2(float a_Matrix[16], const vec3f& a_Axis, const float a_Degrees);

		void MatrixRotateAxis3(float a_Matrix[16], const vec3f& a_Axis, const float a_Degrees);

		// #NOTE Column major 4x4 direction vectors
		// 1, 0, 0, 0,  (X) Right
		// 0, 1, 0, 0,  (Y) Up
		// 0, 0, 1, 0,  (Z) Forward
		// 0, 0, 0, 1   (T) Translation
		inline vec3f MatrixRight(const float a_Matrix[16]) { return vec3f(a_Matrix[0], a_Matrix[1], a_Matrix[2]); }
		inline vec3f MatrixLeft(const float a_Matrix[16]) { return vec3f(-a_Matrix[0], -a_Matrix[1], -a_Matrix[2]); }
		inline vec3f MatrixUp(const float a_Matrix[16]) { return vec3f(a_Matrix[4], a_Matrix[5], a_Matrix[6]); }
		inline vec3f MatrixDown(const float a_Matrix[16]) { return vec3f(-a_Matrix[4], -a_Matrix[5], -a_Matrix[6]); }
		inline vec3f MatrixForward(const float a_Matrix[16]) { return vec3f(a_Matrix[8], a_Matrix[9], a_Matrix[10]); }
		inline vec3f MatrixBackward(const float a_Matrix[16]) { return vec3f(-a_Matrix[8], -a_Matrix[9], -a_Matrix[10]); }

		inline void MatrixTranslate(float a_Matrix[16], const vec3f& a_Offset)
			{ a_Matrix[12] += a_Offset.x; a_Matrix[13] += a_Offset.y; a_Matrix[14] += a_Offset.z; }
		inline void MatrixTranslate(float a_Matrix[16], const vec3f& a_Axis, const float a_Magnitude)
			{ a_Matrix[12] += a_Axis.x * a_Magnitude; a_Matrix[13] += a_Axis.y * a_Magnitude; a_Matrix[14] += a_Axis.z * a_Magnitude; }

		void MatrixIdentity(float a_Matrix[16]);
		inline void MatrixZero(float a_Matrix[16]) { memset(a_Matrix, 0.0f, 16 * sizeof(float)); }
	}

}
