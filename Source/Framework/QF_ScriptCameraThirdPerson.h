#pragma once

#ifdef _QGLM
#include "Libraries/glm/glm.hpp"
#include "Libraries/glm/gtc/matrix_transform.hpp"
#endif // _QGLM

namespace QwerkE {

    class ThirdPersonCamera
    {
    public:
        ThirdPersonCamera(glm::vec3 targetPos = glm::vec3(0.0f))
            : target(targetPos) {
        }

        void Mouse(float xOffset, float yOffset)
        {
            xOffset *= mouseSensitivity;
            yOffset *= mouseSensitivity;

            yaw += xOffset;
            pitch -= yOffset; // invert Y if needed

            // Clamp pitch to avoid flipping
            if (pitch > 89.0f)  pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;
        }

        void MouseScroll(float scrollOffset)
        {
            distanceToTarget -= scrollOffset * zoomSensitivity;
            if (distanceToTarget < minDistance) distanceToTarget = minDistance;
            if (distanceToTarget > maxDistance) distanceToTarget = maxDistance;
        }

        glm::mat4 getViewMatrix() const
        {
            glm::vec3 cameraPos = calculateCameraPosition();
            return glm::lookAt(cameraPos, target, glm::vec3(0, 1, 0));

            // #TODO Camera collision detection for avoiding clipping other objects
            // glm::vec3 desiredPos = calculateCameraPosition();
            // glm::vec3 dir = glm::normalize(desiredPos - target);
            // float desiredDist = glm::length(desiredPos - target);
            //
            // // Pseudocode collision check (replace with actual query)
            // float blockedDist = physics.raycast(target, dir, desiredDist); // returns distance or desiredDist if clear
            //
            // glm::vec3 actualPos = target + dir * blockedDist;
            // return glm::lookAt(actualPos, target, glm::vec3(0, 1, 0));
        }

        void Update()
        {
            glm::vec3 currentTarget = glm::vec3(0.0f); // Intermediate/adjusted target position

            // currentTarget = glm::mix(currentTarget, newTarget, followSpeed * deltaTime);
            target = currentTarget;
        }

    private:
        glm::vec3 calculateCameraPosition() const {
            float yawRad = glm::radians(yaw);
            float pitchRad = glm::radians(pitch);

            glm::vec3 offset;
            offset.x = distanceToTarget * cos(pitchRad) * cos(yawRad);
            offset.y = distanceToTarget * sin(pitchRad);
            offset.z = distanceToTarget * cos(pitchRad) * sin(yawRad);

            // return target - offset; // camera is behind the target

            // Additional over-the-shoulder offset
            glm::vec3 cameraPos = target - offset;

            // Over-the-shoulder offset (rotate with yaw)
            glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), glm::normalize(offset)));
            glm::vec3 up(0, 1, 0);

            cameraPos += right * shoulderOffset.x;
            cameraPos += up * shoulderOffset.y;

            return cameraPos;
        }

        glm::vec3 target = glm::vec3(0.0f); // position to orbit around
        float distanceToTarget = 5.0f;
        float pitch = 20.0f;   // vertical angle (up/down)
        float yaw = -90.0f;    // horizontal angle (left/right)

        float mouseSensitivity = 0.1f;
        float zoomSensitivity = 1.0f;
        float minDistance = 2.0f;
        float maxDistance = 15.0f;

        float followTargetSpeed = 5.0f;

        glm::vec3 shoulderOffset = glm::vec3(0.5f, 1.0f, 0.0f); // Right and up
    };

}