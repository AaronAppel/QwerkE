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

	}

}
