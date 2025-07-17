#include "QC_Math.h"

namespace QwerkE {

	namespace Math {

		std::array<float, 3> RotationMatrixToEulerZYX(const float R[3][3])
		{
			float x, y, z;

			if (std::abs(R[2][0]) != 1.0f) {
				y = -std::asin(R[2][0]);
				float cos_y = std::cos(y);
				x = std::atan2(R[2][1] / cos_y, R[2][2] / cos_y);
				z = std::atan2(R[1][0] / cos_y, R[0][0] / cos_y);
			}
			else {
				// Gimbal lock
				z = 0;
				if (R[2][0] == -1.0f) {
					y = M_PI / 2.0f;
					x = z + std::atan2(R[0][1], R[0][2]);
				}
				else {
					y = -M_PI / 2.0f;
					x = -z + std::atan2(-R[0][1], -R[0][2]);
				}
			}

			return { DEG(x), DEG(y), DEG(z) };  // Return degrees
		}

		vec3f MatrixRotation(const float a_Matrix[16])
		{
			vec3f returnRotation; // x=pitch, y=yaw, z=roll

			// Step 1: Extract scale
			float scaleX = std::sqrt(a_Matrix[0] * a_Matrix[0] + a_Matrix[1] * a_Matrix[1] + a_Matrix[2] * a_Matrix[2]);
			float scaleY = std::sqrt(a_Matrix[4] * a_Matrix[4] + a_Matrix[5] * a_Matrix[5] + a_Matrix[6] * a_Matrix[6]);
			float scaleZ = std::sqrt(a_Matrix[8] * a_Matrix[8] + a_Matrix[9] * a_Matrix[9] + a_Matrix[10] * a_Matrix[10]);

			// Step 2: Normalize the rotation part (upper 3x3) by removing scale
			float r00 = a_Matrix[0] / scaleX;
			float r01 = a_Matrix[4] / scaleY;
			float r02 = a_Matrix[8] / scaleZ;

			float r10 = a_Matrix[1] / scaleX;
			float r11 = a_Matrix[5] / scaleY;
			float r12 = a_Matrix[9] / scaleZ;

			float r20 = a_Matrix[2] / scaleX;
			float r21 = a_Matrix[6] / scaleY;
			float r22 = a_Matrix[10] / scaleZ;

			// Step 3: Convert to Euler angles (Y-X-Z order: yaw, pitch, roll)
			if (r20 < 1.0f) {
				if (r20 > -1.0f) {
					returnRotation.x = std::asin(-r20);
					returnRotation.y = std::atan2(r10, r00);
					returnRotation.z = std::atan2(r21, r22);
				}
				else {
					// r20 == -1
					returnRotation.x = M_PI / 2.0f;
					returnRotation.y = -std::atan2(-r12, r11);
					returnRotation.z = 0.0f;
				}
			}
			else {
				// r20 == +1
				returnRotation.x = -M_PI / 2.0f;
				returnRotation.y = std::atan2(-r12, r11);
				returnRotation.z = 0.0f;
			}

			// Convert radians to degrees (optional)
			const float RAD2DEG = 180.0f / static_cast<float>(M_PI);
			returnRotation.x *= RAD2DEG;
			returnRotation.y *= RAD2DEG;
			returnRotation.z *= RAD2DEG;

			return returnRotation;
		}

		vec3f MatrixScale(const float a_Matrix[16])
		{
			vec3f returnScale;

			// X axis scale = length of column 0 vector
			returnScale.x = std::sqrt(a_Matrix[0] * a_Matrix[0] +
				a_Matrix[1] * a_Matrix[1] +
				a_Matrix[2] * a_Matrix[2]);

			// Y axis scale = length of column 1 vector
			returnScale.y = std::sqrt(a_Matrix[4] * a_Matrix[4] +
				a_Matrix[5] * a_Matrix[5] +
				a_Matrix[6] * a_Matrix[6]);

			// Z axis scale = length of column 2 vector
			returnScale.z = std::sqrt(a_Matrix[8] * a_Matrix[8] +
				a_Matrix[9] * a_Matrix[9] +
				a_Matrix[10] * a_Matrix[10]);

			return returnScale;
		}

	}

}
