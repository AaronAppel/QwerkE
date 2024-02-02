#pragma once

// #TODO Change file name from QF_...CameraComponent -> QF_ComponentCamera...
namespace QwerkE {

    class FirstPersonCameraComponent : public ComponentCamera
    {
    public:
        FirstPersonCameraComponent(vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 up = g_WORLDUP, float yaw = gc_YAW, float pitch = gc_PITCH);

        void UpdateCameraVectors();

        void ProcessKeyboard(eCamera_Movement direction, float deltaTime) override;
        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) override;
    };

}
