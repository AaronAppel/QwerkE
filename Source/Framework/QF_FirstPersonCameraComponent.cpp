#include "QF_Component.h"

#include "QF_CameraComponent.h"
#include "QF_FirstPersonCameraComponent.h"
#include "QF_QwerkE_Enums.h"
#include "QF_GameObject.h"
#include "glew/GL/glew.h"

namespace QwerkE {

    FirstPersonCameraComponent::FirstPersonCameraComponent(vec3 position, vec3 up, float yaw, float pitch) :
        CameraComponent(position, up, yaw, pitch)
    {
    }

    void FirstPersonCameraComponent::UpdateCameraVectors()
    {
        m_ViewMatrix->CreateLookAtView(m_Position, m_CamUp, m_Forward);
        m_ProjMatrix->CreatePerspectiveHFoV(m_Zoom * 0.5f, m_ViewportSize.x / m_ViewportSize.y, m_CAMNEAR, m_CAMFAR);
    }

    void FirstPersonCameraComponent::ProcessKeyboard(eCamera_Movement direction, float deltaTime)
    {
        const float velocity = m_MovementSpeed * deltaTime;
        if (direction == FORWARD)
            m_Position += m_Forward * velocity;
        if (direction == BACKWARD)
            m_Position -= m_Forward * velocity;
        if (direction == LEFT)
            m_Position -= m_Right * velocity;
        if (direction == RIGHT)
            m_Position += m_Right * velocity;
        if (direction == UP)
            m_Position += m_CamUp * velocity;
        if (direction == DOWN)
            m_Position -= m_CamUp * velocity;
        if (direction == RROTATE) // #TODO Implement rotation. Document in features list and remove this code
            m_Position -= m_CamUp * velocity;
        if (direction == LROTATE)
            m_Position += m_CamUp * velocity;

        FirstPersonCameraComponent::UpdateCameraVectors();
        CameraComponent::UpdateParentPosition(m_Position);
    }

    void FirstPersonCameraComponent::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
    {
        xoffset *= m_MouseSensitivity;
        yoffset *= m_MouseSensitivity;

        m_Yaw += xoffset;
        m_Pitch += yoffset;

        if (constrainPitch)
        {
            if (m_Pitch > 89.0f) // #TODO Review precision
            {
                m_Pitch = 89.0f;
            }
            else if (m_Pitch < -89.0f)
            {
                m_Pitch = -89.0f;
            }
        }

        FirstPersonCameraComponent::UpdateCameraVectors();
    }

}
