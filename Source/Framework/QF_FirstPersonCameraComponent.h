#pragma once

namespace QwerkE {

    class FirstPersonCameraComponent : public CameraComponent
    {
    public:
        FirstPersonCameraComponent(vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 up = g_WORLDUP, float yaw = gc_YAW, float pitch = gc_PITCH);

        void UpdateCameraVectors();

        void ProcessKeyboard(eCamera_Movement direction, float deltaTime) override;
        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) override;
    };

}