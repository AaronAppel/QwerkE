#include "QC_Math.h"

#if _QGLM
#include "Libraries/glm/glm.hpp"
#include "Libraries/glm/gtc/matrix_transform.hpp"  // for glm::rotate
#include "Libraries/glm/gtc/constants.hpp"         // for glm::pi
#endif // _QGLM

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

        void MatrixRotationXYZ(const float a_Matrix[16], float& pitch, float& yaw, float& roll) {
            // Column-major matrix layout:
            // [m00, m10, m20, m30]
            // [m01, m11, m21, m31]
            // [m02, m12, m22, m32]
            // [m03, m13, m23, m33]

            // Extracting rotation values
            float m00 = a_Matrix[0], m01 = a_Matrix[4], m02 = a_Matrix[8];
            float m10 = a_Matrix[1], m11 = a_Matrix[5], m12 = a_Matrix[9];
            float m20 = a_Matrix[2], m21 = a_Matrix[6], m22 = a_Matrix[10];

            // Pitch (rotation about X-axis)
            pitch = std::atan2(m21, m22);

            // Yaw (rotation about Y-axis)
            yaw = std::atan2(-m20, std::sqrt(m21 * m21 + m22 * m22));

            // Roll (rotation about Z-axis)
            roll = std::atan2(m10, m00);
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

        vec3f MatrixScale(const float a_Matrix[16]) // Column Major
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

        void MatrixRotateAroundY(float a_Matrix[16], const float a_YawDegrees)
        {
            const float radians = DegToRad(a_YawDegrees);
            const float cosTheta = std::cos(radians);
            const float sinTheta = std::sin(radians);

            float matrix[4][4] = {
                { a_Matrix[0], a_Matrix[1], a_Matrix[2], a_Matrix[3]},
                { a_Matrix[4], a_Matrix[5], a_Matrix[6], a_Matrix[7]},
                { a_Matrix[8], a_Matrix[9], a_Matrix[10], a_Matrix[11]},
                { a_Matrix[12], a_Matrix[13], a_Matrix[14], a_Matrix[15]},
            };

            // Rotation matrix around Y-axis
            float rotationY[4][4] = {
                { cosTheta, 0.0f, sinTheta, 0.0f },
                { 0.0f,     1.0f, 0.0f,     0.0f },
                { -sinTheta, 0.0f, cosTheta, 0.0f },
                { 0.0f,     0.0f, 0.0f,     1.0f }
            };

            // Result matrix
            float result[4][4] = { };

            // Matrix multiplication: result = rotationY * matrix (world-space rotation)
            for (int row = 0; row < 4; ++row) {
                for (int col = 0; col < 4; ++col) {
                    result[row][col] = matrix[row][col];
                    for (int k = 0; k < 4; ++k) {
                        result[row][col] += rotationY[row][k] * matrix[k][col];
                    }
                }
            }

            a_Matrix[0] = result[0][0];
            a_Matrix[1] = result[0][1];
            a_Matrix[2] = result[0][2];
            a_Matrix[3] = result[0][3];
            a_Matrix[4] = result[1][0];
            a_Matrix[5] = result[1][1];
            a_Matrix[6] = result[1][2];
            a_Matrix[7] = result[1][3];
            a_Matrix[8] = result[2][0];
            a_Matrix[9] = result[2][1];
            a_Matrix[10] = result[2][2];
            a_Matrix[11] = result[2][3];
            a_Matrix[12] = result[3][0];
            a_Matrix[13] = result[3][1];
            a_Matrix[14] = result[3][2];
            a_Matrix[15] = result[3][3];
        }

        // Rotates a matrix around an arbitrary axis
        void MatrixRotateAxis(float a_Matrix[16], const vec3f& a_Axis, const float a_Degrees)
        {
            const float radians = DegToRad(a_Degrees);

            glm::mat4 matrix = {
                a_Matrix[0], a_Matrix[1], a_Matrix[2], a_Matrix[3],
                a_Matrix[4], a_Matrix[5], a_Matrix[6], a_Matrix[7],
                a_Matrix[8], a_Matrix[9], a_Matrix[10], a_Matrix[11],
                a_Matrix[12], a_Matrix[13], a_Matrix[14], a_Matrix[15],
            };
            glm::vec3 normalizedAxis = glm::normalize(glm::vec3(a_Axis.x, a_Axis.y, a_Axis.z));
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), radians, normalizedAxis);

            matrix = rotation * matrix; // world space: rotate relative to global axes
            // matrix = matrix * rotation; // #NOTE Local space: rotate relative to object's axes

            a_Matrix[0] = matrix[0][0];
            a_Matrix[1] = matrix[0][1];
            a_Matrix[2] = matrix[0][2];
            a_Matrix[3] = matrix[0][3];
            a_Matrix[4] = matrix[1][0];
            a_Matrix[5] = matrix[1][1];
            a_Matrix[6] = matrix[1][2];
            a_Matrix[7] = matrix[1][3];
            a_Matrix[8] = matrix[2][0];
            a_Matrix[9] = matrix[2][1];
            a_Matrix[10] = matrix[2][2];
            a_Matrix[11] = matrix[2][3];
            a_Matrix[12] = matrix[3][0];
            a_Matrix[13] = matrix[3][1];
            a_Matrix[14] = matrix[3][2];
            a_Matrix[15] = matrix[3][3];
        }

        // Multiplies two 4x4 column-major matrices: result = a * b
        void multiplyMatrices(const float a[16], const float b[16], float result[16]) {
            float temp[16];
            for (int col = 0; col < 4; ++col) {
                for (int row = 0; row < 4; ++row) {
                    temp[col * 4 + row] =
                        a[0 * 4 + row] * b[col * 4 + 0] +
                        a[1 * 4 + row] * b[col * 4 + 1] +
                        a[2 * 4 + row] * b[col * 4 + 2] +
                        a[3 * 4 + row] * b[col * 4 + 3];
                }
            }
            std::memcpy(result, temp, 16 * sizeof(float));
        }

        void MatrixRotateAxis2(float a_Matrix[16], const vec3f& a_Axis, const float a_Degrees)
        {
            const float radians = DegToRad(a_Degrees);
            // Normalize the axis
            float length = std::sqrt(a_Axis.x * a_Axis.x + a_Axis.y * a_Axis.y + a_Axis.z * a_Axis.z);
            if (length == 0.0f) return; // Avoid division by zero
            float x = a_Axis.x / length;
            float y = a_Axis.y / length;
            float z = a_Axis.z / length;

            float c = std::cos(radians);
            float s = std::sin(radians);
            float one_minus_c = 1.0f - c;

            // Build the rotation matrix (column-major order)
            float rotation[16] = {
                c + x * x * one_minus_c,       y * x * one_minus_c + z * s, z * x * one_minus_c - y * s, 0.0f,
                x * y * one_minus_c - z * s,   c + y * y * one_minus_c,     z * y * one_minus_c + x * s, 0.0f,
                x * z * one_minus_c + y * s,   y * z * one_minus_c - x * s, c + z * z * one_minus_c,     0.0f,
                0.0f,                          0.0f,                        0.0f,                        1.0f
            };

            // Multiply: matrix = rotation * matrix
            float result[16];
            multiplyMatrices(rotation, a_Matrix, result);
            std::memcpy(a_Matrix, result, 16 * sizeof(float));
        }

        void MatrixRotateAxis3(float a_Matrix[16], const vec3f& a_Axis, const float a_Degrees)
        {
            const float radians = DegToRad(a_Degrees);
            // Normalize the axis
            float length = std::sqrt(a_Axis.x * a_Axis.x + a_Axis.y * a_Axis.y + a_Axis.z * a_Axis.z);
            if (length == 0.0f) return; // Avoid division by zero
            float x = a_Axis.x / length;
            float y = a_Axis.y / length;
            float z = a_Axis.z / length;

            float c = std::cos(radians);
            float s = std::sin(radians);
            float one_minus_c = 1.0f - c;

            // Build the rotation matrix (column-major order)
            float rotation[16] = {
                c + x * x * one_minus_c,       y * x * one_minus_c + z * s, z * x * one_minus_c - y * s, 0.0f,
                x * y * one_minus_c - z * s,   c + y * y * one_minus_c,     z * y * one_minus_c + x * s, 0.0f,
                x * z * one_minus_c + y * s,   y * z * one_minus_c - x * s, c + z * z * one_minus_c,     0.0f,
                0.0f,                          0.0f,                        0.0f,                        1.0f
            };

            // Multiply: matrix = rotation * matrix
            // float result[16];
            // multiplyMatrices(rotation, a_Matrix, result);
            // std::memcpy(a_Matrix, result, 16 * sizeof(float));

            // Only rotate the top-left 3x3 (orientation)
            float temp[16];
            multiplyMatrices(rotation, a_Matrix, temp);

            // Copy only the upper-left 3x3 back into the original matrix
            for (int col = 0; col < 3; ++col) {
                for (int row = 0; row < 3; ++row) {
                    a_Matrix[col * 4 + row] = temp[col * 4 + row];
                }
            }
            // Leave translation (last column) untouched
        }

        void MatrixIdentity(float a_Matrix[16])
        {
            a_Matrix[0] = 1.f; a_Matrix[4] = 0.f; a_Matrix[8] = 0.f; a_Matrix[12] = 0.f;
            a_Matrix[1] = 0.f; a_Matrix[5] = 1.f; a_Matrix[9] = 0.f; a_Matrix[13] = 0.f;
            a_Matrix[2] = 0.f; a_Matrix[6] = 0.f; a_Matrix[10] = 1.f; a_Matrix[14] = 0.f;
            a_Matrix[3] = 0.f; a_Matrix[7] = 0.f; a_Matrix[11] = 0.f; a_Matrix[15] = 1.f;
        }

    }

}
