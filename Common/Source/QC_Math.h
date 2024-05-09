#pragma once

namespace QwerkE {

	namespace Math
	{
		constexpr double PI() { return 3.1415926535897932384626433832795; }
		constexpr float PI_f() { return 3.1415926535897932384626433832795f; }

		#define DEG_TO_RAD  0.01745329251994329576923690768489
		#define RAD_TO_DEG  57.295779513082320876798154814105

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
