
#include "QC_Frustum.h"
#include "QC_Matrix4.h"
#include "QC_Vector3.h"

namespace QwerkE {

    class Camera final
    {
    public:
        Vector3f position = { 0, 0, 5 };
        Vector3f target = { 0, 0, 0 };
        Vector3f up = { 0, 1, 0 };

        float fovY = 60.0f * 3.1415926f / 180.0f;
        float aspectRatio = 16.0f / 9.0f;
        float nearZ = 0.1f;
        float farZ = 100.0f;

        bool isRightHanded = true; // Toggle between coordinate systems

        Matrix4f GetViewMatrix() const
        {
            if (isRightHanded)
                return Matrix4f::LookAt(position, target, up);
            else
            {
                // Invert Z axis for left-handed lookAt
                Vector3f forward = (target - position).Normalized();
                Vector3f right = Vector3f::Cross(up, forward).Normalized();
                Vector3f upVec = Vector3f::Cross(forward, right);

                return Matrix4f {
                    right.x, upVec.x, forward.x, 0.0f,
                    right.y, upVec.y, forward.y, 0.0f,
                    right.z, upVec.z, forward.z, 0.0f,
                    -Vector3f::Dot(right, position),
                    -Vector3f::Dot(upVec, position),
                    -Vector3f::Dot(forward, position),
                    1.0f
                };
            }
        }

        Matrix4f GetProjectionMatrix() const
        {
            return Matrix4f::Perspective(fovY, aspectRatio, nearZ, farZ);
        }

        Matrix4f GetViewProjectionMatrix() const
        {
            return GetProjectionMatrix() * GetViewMatrix();
        }

        Frustum GetFrustum() const
        {
            return Frustum::FromMatrix(GetViewProjectionMatrix());
        }

        // Optional: look direction, orbit, dolly, etc.
    };

}