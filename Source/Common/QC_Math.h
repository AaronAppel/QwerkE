#pragma once

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

	}

}
