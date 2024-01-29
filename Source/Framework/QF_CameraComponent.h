#pragma once

#include <cstdint>

#include "Libraries/glew/GL/glew.h"
#include "Libraries/FlatheadGames/MyMatrix.h"

#include "QF_Constants.h"
#include "QF_Enums.h"
#include "QF_Component.h"

const vec3 g_WORLDUP = vec3(0, 1, 0); // TODO: Fix inverted world Y-axis and move to a better defines file

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
        virtual ~ComponentCamera();

        void Setup();

        // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
        const mat4* GetViewMatrix() const { return m_ViewMatrix; }
        const mat4* GetProjectionMatrix() const { return m_ProjMatrix; }

        // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
        virtual void ProcessKeyboard(eCamera_Movement direction, float deltaTime) = 0;

        // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
        virtual void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) = 0;
        void Rotate() {};

        // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
        virtual void ProcessMouseScroll(float yoffset);
        void ZoomCamera(float yOffset) { ProcessMouseScroll(yOffset); };

        // Calculates the front vector from the Camera's (updated) Eular Angles
        virtual void UpdateCameraVectors(); // TODO: Think of making this pure virtual to create an abstract class

        /* Getters + Setters*/
        // Getters
        eCamType GetType() { return m_Type; };

        // Setters
        void SetType(eCamType type) { m_Type = type; };
        void SetTargetPosition(vec3 position);
        void SetViewportSize(vec2 size) { m_ViewportSize = size; UpdateCameraVectors(); };

    protected:
        ComponentCamera(vec3 position = vec3::Zero(), vec3 up = g_WORLDUP, float yaw = gc_YAW, float pitch = gc_PITCH);
        ComponentCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

        void UpdateParentPosition(vec3 m_Position);

        eCamType m_Type = CamType_NULL;

        vec3 m_Position = vec3(0, 0, 0);
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
