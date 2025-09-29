
namespace QwerkE {

    struct Plane final
    {
        float a, b, c, d; // Plane equation: ax + by + cz + d = 0

        // Normalize plane
        void Normalize()
        {
            float length = std::sqrt(a * a + b * b + c * c);
            if (length == 0.0f) return;
            a /= length; b /= length; c /= length; d /= length;
        }

        // Signed distance from point to plane
        float Distance(const Vector3& p) const
        {
            return a * p.x + b * p.y + c * p.z + d;
        }
    };

    struct Frustum final
    {
        Plane planes[6]; // Left, Right, Bottom, Top, Near, Far

        // Extract from a combined ViewProjection matrix
        static Frustum FromMatrix(const Matrix4& vp)
        {
            Frustum f;

            // Column-major matrix
            // Plane extraction (OpenGL-style)
            // Each plane is: row 4 ± row X
            f.planes[0] = {  // Left
                vp(0, 3) + vp(0, 0),
                vp(1, 3) + vp(1, 0),
                vp(2, 3) + vp(2, 0),
                vp(3, 3) + vp(3, 0)
            };

            f.planes[1] = {  // Right
                vp(0, 3) - vp(0, 0),
                vp(1, 3) - vp(1, 0),
                vp(2, 3) - vp(2, 0),
                vp(3, 3) - vp(3, 0)
            };

            f.planes[2] = {  // Bottom
                vp(0, 3) + vp(0, 1),
                vp(1, 3) + vp(1, 1),
                vp(2, 3) + vp(2, 1),
                vp(3, 3) + vp(3, 1)
            };

            f.planes[3] = {  // Top
                vp(0, 3) - vp(0, 1),
                vp(1, 3) - vp(1, 1),
                vp(2, 3) - vp(2, 1),
                vp(3, 3) - vp(3, 1)
            };

            f.planes[4] = {  // Near
                vp(0, 3) + vp(0, 2),
                vp(1, 3) + vp(1, 2),
                vp(2, 3) + vp(2, 2),
                vp(3, 3) + vp(3, 2)
            };

            f.planes[5] = {  // Far
                vp(0, 3) - vp(0, 2),
                vp(1, 3) - vp(1, 2),
                vp(2, 3) - vp(2, 2),
                vp(3, 3) - vp(3, 2)
            };

            // Normalize all planes
            for (Plane& p : f.planes)
                p.Normalize();

            return f;
        }

        // Point test
        bool IsPointInside(const Vector3& point) const
        {
            for (const Plane& p : planes)
            {
                if (p.Distance(point) < 0.0f)
                    return false;
            }
            return true;
        }

        // Add more: AABB, sphere, etc.
    };

}