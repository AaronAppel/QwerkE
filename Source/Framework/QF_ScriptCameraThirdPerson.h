#pragma once

namespace QwerkE {

    class ThirdPersonCamera
    {
    public:
        ThirdPersonCamera(vec3f targetPos = vec3f::Zero())
            : target(targetPos)
        { }

        void Mouse(float xOffset, float yOffset)
        {
            xOffset *= mouseSensitivityX;
            yOffset *= mouseSensitivityY;

            yawDegrees += xOffset;
            pitchDegrees -= yOffset; // invert Y if needed

            pitchDegrees = Math::Clamp(-89.0f, pitchDegrees, 89.0f); // Clamp pitch to avoid flipping
        }

        void MouseScroll(float scrollOffset)
        {
            distanceToTarget -= scrollOffset * zoomSensitivity;
            if (distanceToTarget < minDistance) distanceToTarget = minDistance;
            if (distanceToTarget > maxDistance) distanceToTarget = maxDistance;
        }

        mat4f getViewMatrix() const
        {
            vec3f cameraPos = calculateCameraPosition();
            return mat4f::LookAt(cameraPos, target, vec3f(0, 1, 0));

            // #TODO Camera collision detection for avoiding clipping other objects
            // vec3f desiredPos = calculateCameraPosition();
            // vec3f dir = glm::normalize(desiredPos - target);
            // float desiredDist = glm::length(desiredPos - target);
            //
            // // Pseudocode collision check (replace with actual query)
            // float blockedDist = physics.raycast(target, dir, desiredDist); // returns distance or desiredDist if clear
            //
            // vec3f actualPos = target + dir * blockedDist;
            // return glm::lookAt(actualPos, target, vec3f(0, 1, 0));
        }

        void Update()
        {
            vec3f currentTarget = vec3f(0.0f); // Intermediate/adjusted target position

            // currentTarget = glm::mix(currentTarget, newTarget, followSpeed * deltaTime);
            target = currentTarget;
        }

    private:
        vec3f calculateCameraPosition() const {
            float yawRad = Math::ToRad * yawDegrees;
            float pitchRad = Math::ToRad * pitchDegrees;

            vec3f offset;
            offset.x = distanceToTarget * cos(pitchRad) * cos(yawRad);
            offset.y = distanceToTarget * sin(pitchRad);
            offset.z = distanceToTarget * cos(pitchRad) * sin(yawRad);

            // return target - offset; // camera is behind the target

            // Additional over-the-shoulder offset
            vec3f cameraPos = target - offset;

            // Over-the-shoulder offset (rotate with yaw)
            vec3f right = vec3f::Up().Cross(offset.Normalized()).Normalized(); // #TODO Review hard coded up vector

            cameraPos += right * shoulderOffset.x;
            cameraPos += vec3f::Up() * shoulderOffset.y;

            return cameraPos;
        }

        vec3f target = vec3f(0.0f); // position to orbit around
        float distanceToTarget = 5.0f;
        float pitchDegrees = 20.0f;   // vertical angle (up/down)
        float yawDegrees = -90.0f;    // horizontal angle (left/right)

        float mouseSensitivityX = 0.1f; // #TODO Review controller or alternate sensitivity
        float mouseSensitivityY = 0.1f;

        float zoomSensitivity = 1.0f;
        float minDistance = 2.0f;
        float maxDistance = 15.0f;

        float followTargetSpeed = 5.0f;

        vec3f shoulderOffset = vec3f(0.5f, 1.0f, 0.0f); // Right and up
    };

}