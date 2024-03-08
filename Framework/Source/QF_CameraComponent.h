#pragma once

#include <cstdint>

#include "Libraries/glew/glew.h"
#include "Libraries/Mirror/Source/Mirror.h"

#include "QF_Constants.h"
#include "QF_Enums.h"
#include "QF_Component.h"

const vec3 g_WORLDUP = vec3(0, 1, 0); // TODO: Fix inverted world Y-axis and move to a better defines file

// #TODO Change file name from QF_CameraComponent -> QF_ComponentCamera
namespace QwerkE {

    enum eCamera_Movement : std::uint8_t
    {
        FORWARD = 0,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN,
        RROTATE,
        LROTATE
    };

    class ComponentCamera : public Component
    {
    public:
        ComponentCamera(vec3 position = vec3(0.f), vec3 up = g_WORLDUP, float yaw = Constants::gc_YAW, float pitch = Constants::gc_PITCH);

        void Setup();

        const mat4& GetViewMatrix() const { return m_ViewMatrix; }
        const mat4& GetProjectionMatrix() const { return m_ProjMatrix; }

        void OnCamMove(eCamera_Movement direction, float deltaTime);

        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

        void ProcessMouseScroll(const vec2& mouseScroll);

        void UpdateCameraVectors();

        void SetTargetPosition(vec3 position);
        void SetViewportSize(vec2 size) { m_ViewportSize = size; UpdateCameraVectors(); };

        const vec3& CamRight() const { return m_Right; }
        const vec3& CamUp() const { return m_CamUp; }
        const vec3& CamForward() const { return m_Forward; }

    protected:
        MIRROR_PRIVATE_MEMBERS

        ComponentCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

        void UpdateParentPosition(vec3 m_Position);

        vec3 m_Position = vec3(0, 0, 0); // #TODO Deprecate position and use parent GameObject transform position
        vec3 m_Forward = vec3(0.0f, 0.0f, 1.0f);
        vec3 m_CamUp = g_WORLDUP;
        vec3 m_Right = vec3(1, 0, 0);

        float m_CAMNEAR = 0.01f;
        float m_CAMFAR = 1000.0f;

        float m_Yaw = Constants::gc_YAW;
        float m_Pitch = Constants::gc_PITCH;

        float m_MovementSpeed = Constants::gc_SPEED;
        float m_MouseSensitivity = Constants::gc_SENSITIVTY;
        float m_Zoom = Constants::gc_ZOOM;

        vec3 m_TargetPosition = m_Position + m_Forward * 1.5f;

        vec2 m_ViewportSize = vec2(1280, 720);

        mat4 m_ViewMatrix = mat4(1.f);
        mat4 m_ProjMatrix = mat4(1.f);
    };

}
