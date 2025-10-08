#pragma once

// #TODO Improve
#include <iostream>
#include <cmath>
#include <array>

#include "QF_TypeDefs.h" // #TODO Circular order dependencies?

namespace QwerkE {

	namespace Math {

		// Vectors
#ifdef _QGLM
		inline float Magnitude(const vec3f& a_Other) { return glm::length(a_Other); }
#endif // _QGLM
		inline float VectorMagnitude(float a_X, float a_Y) { return std::sqrt(a_X * a_X + a_Y * a_Y); }
		inline float VectorMagnitude(float a_X, float a_Y, float a_Z) { return std::sqrt(a_X * a_X + a_Y * a_Y + a_Z * a_Z); }

	}

}
