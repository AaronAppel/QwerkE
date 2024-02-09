#pragma once

#include <cstdint>

#include "Libraries/glew/GL/glew.h"
#include "Libraries/FlatheadGames/MyMatrix.h"
#include "Libraries/Mirror/Source/Mirror.h"

#include "QF_Constants.h"
#include "QF_Enums.h"
#include "QF_Component.h"
#include "QC_Vector.h"

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
        ComponentCamera(vec3 position = vec3::Zero(), vec3 up = g_WORLDUP, float yaw = gc_YAW, float pitch = gc_PITCH);
        ~ComponentCamera();

        void Setup();

        const mat4* GetViewMatrix() const { return m_ViewMatrix; }
        const mat4* GetProjectionMatrix() const { return m_ProjMatrix; }

        void ProcessKeyboard(eCamera_Movement direction, float deltaTime);

        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
        void Rotate() {}; // #TODO Deprecate

        void ProcessMouseScroll(float yoffset);
        void ZoomCamera(float yOffset) { ProcessMouseScroll(yOffset); };

        void UpdateCameraVectors();

        void SetTargetPosition(vec3 position);
        void SetViewportSize(vec2 size) { m_ViewportSize = size; UpdateCameraVectors(); };

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

        float m_Yaw = gc_YAW;
        float m_Pitch = gc_PITCH;

        float m_MovementSpeed = gc_SPEED;
        float m_MouseSensitivity = gc_SENSITIVTY;
        float m_Zoom = gc_ZOOM;

        vec3 m_TargetPosition = m_Position + m_Forward * 1.5f;

        vec2 m_ViewportSize = vec2(1280, 720);

        mat4* m_ViewMatrix = new mat4();
        mat4* m_ProjMatrix = new mat4();
    };

}
