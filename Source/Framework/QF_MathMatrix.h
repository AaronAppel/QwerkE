#pragma once

// #TODO Improve
#include <iostream>
#include <cmath>
#include <array>

#include "QF_TypeDefs.h" // #TODO Circular order dependencies?

namespace QwerkE {

	namespace Math {

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
		// 1, 0, 0, 0,  Right (X)
		// 0, 1, 0, 0,  Up (Y)
		// 0, 0, 1, 0,  Forward (Z)
		// 0, 0, 0, 1   Translation
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
