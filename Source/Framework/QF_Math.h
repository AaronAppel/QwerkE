#pragma once

namespace QwerkE {

	namespace Math
	{

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
		inline T ClampRollover(T min, T value, T max)
		{
			auto range = max - min;

			if (value < min)
			{
				return value + range;
			}
			else if (value > max)
			{
				return value - range;
			}
			return value;
		}

	}

}
