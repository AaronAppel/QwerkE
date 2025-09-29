
#include "QC_Matrix4.h"

namespace QwerkE {

    struct Quaternion final // #TODO Should be in it's own file
    {
        float x, y, z, w;

        Quaternion() = default;
        Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        static Quaternion FromMatrix(const Matrix4f& a_Matrix)
        {
            float trace = a_Matrix.m[0] + a_Matrix.m[5] + a_Matrix.m[10];
            Quaternion q;

            if (trace > 0.0f)
            {
                float s = 0.5f / std::sqrt(trace + 1.0f);
                q.w = 0.25f / s;
                q.x = (a_Matrix.m[9] - a_Matrix.m[6]) * s;
                q.y = (a_Matrix.m[2] - a_Matrix.m[8]) * s;
                q.z = (a_Matrix.m[4] - a_Matrix.m[1]) * s;
            }
            else
            {
                if (a_Matrix.m[0] > a_Matrix.m[5] && a_Matrix.m[0] > a_Matrix.m[10])
                {
                    float s = 2.0f * std::sqrt(1.0f + a_Matrix.m[0] - a_Matrix.m[5] - a_Matrix.m[10]);
                    q.w = (a_Matrix.m[9] - a_Matrix.m[6]) / s;
                    q.x = 0.25f * s;
                    q.y = (a_Matrix.m[1] + a_Matrix.m[4]) / s;
                    q.z = (a_Matrix.m[2] + a_Matrix.m[8]) / s;
                }
                else if (a_Matrix.m[5] > a_Matrix.m[10])
                {
                    float s = 2.0f * std::sqrt(1.0f + a_Matrix.m[5] - a_Matrix.m[0] - a_Matrix.m[10]);
                    q.w = (a_Matrix.m[2] - a_Matrix.m[8]) / s;
                    q.x = (a_Matrix.m[1] + a_Matrix.m[4]) / s;
                    q.y = 0.25f * s;
                    q.z = (a_Matrix.m[6] + a_Matrix.m[9]) / s;
                }
                else
                {
                    float s = 2.0f * std::sqrt(1.0f + a_Matrix.m[10] - a_Matrix.m[0] - a_Matrix.m[5]);
                    q.w = (a_Matrix.m[4] - a_Matrix.m[1]) / s;
                    q.x = (a_Matrix.m[2] + a_Matrix.m[8]) / s;
                    q.y = (a_Matrix.m[6] + a_Matrix.m[9]) / s;
                    q.z = 0.25f * s;
                }
            }

            return q;
        }

        Matrix4f ToMatrix() const
        {
            Matrix4f result = Matrix4f::Identity();

            float xx = x * x;
            float yy = y * y;
            float zz = z * z;
            float xy = x * y;
            float xz = x * z;
            float yz = y * z;
            float wx = w * x;
            float wy = w * y;
            float wz = w * z;

            // Column-major order
            result.m[0] = 1.0f - 2.0f * (yy + zz);
            result.m[1] = 2.0f * (xy + wz);
            result.m[2] = 2.0f * (xz - wy);
            result.m[3] = 0.0f;

            result.m[4] = 2.0f * (xy - wz);
            result.m[5] = 1.0f - 2.0f * (xx + zz);
            result.m[6] = 2.0f * (yz + wx);
            result.m[7] = 0.0f;

            result.m[8] = 2.0f * (xz + wy);
            result.m[9] = 2.0f * (yz - wx);
            result.m[10] = 1.0f - 2.0f * (xx + yy);
            result.m[11] = 0.0f;

            result.m[12] = 0.0f;
            result.m[13] = 0.0f;
            result.m[14] = 0.0f;
            result.m[15] = 1.0f;

            return result;
        }
    };

}