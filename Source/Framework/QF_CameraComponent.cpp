#include "QF_Component.h"

#include "Libraries/glfw/GLFW/glfw3.h"
#include "Libraries/FlatheadGames/MyMatrix.h"

#include "QF_Enums.h"

#include "QF_CameraComponent.h"
#include "QF_GameObject.h"
#include "QF_Window.h"

namespace QwerkE {

	// public functions
	// Constructor with vectors
	ComponentCamera::ComponentCamera(vec3 position, vec3 up, float yaw, float pitch)
	{
		m_Position = position;
		m_CamUp = up;
		m_Yaw = yaw;
		m_Pitch = pitch;

		m_ProjMatrix = new mat4();
		m_ViewMatrix = new mat4();

		UpdateCameraVectors();
		m_ComponentTag = Component_Camera;

        m_ViewportSize = Window::GetResolution();

		SetTargetPosition(m_Position + m_Forward);
		UpdateCameraVectors();
	}

	// Constructor with scalar values
	ComponentCamera::ComponentCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	{
		m_Position = vec3(posX, posY, posZ);
		m_CamUp = vec3(upX, upY, upZ);
		m_Yaw = yaw;
		m_Pitch = pitch;
		UpdateCameraVectors();

        m_ViewportSize = Window::GetResolution();

		SetTargetPosition(m_Position + m_Forward);
		UpdateCameraVectors();
	}

	ComponentCamera::~ComponentCamera()
	{
		delete m_ViewMatrix;
		delete m_ProjMatrix;
	}

	void ComponentCamera::Setup()
	{
		m_Position.x = m_pParent->GetPosition().x;
		m_Position.y = m_pParent->GetPosition().y;
		m_Position.z = m_pParent->GetPosition().z;

		UpdateCameraVectors();
	}

	void ComponentCamera::SetTargetPosition(vec3 position)
	{
		m_TargetPosition.x = position.x;
		m_TargetPosition.y = position.y;
		m_TargetPosition.z = position.z;
	}

	void ComponentCamera::UpdateParentPosition(vec3 m_Position)
	{
		m_pParent->SetPosition(m_Position);
	}

	void ComponentCamera::ProcessMouseScroll(float yoffset)
	{
		if (m_Zoom >= 1.0f && m_Zoom <= 45.0f)
			m_Zoom -= yoffset;
		if (m_Zoom <= 1.0f)
			m_Zoom = 1.0f;
		if (m_Zoom >= 45.0f)
			m_Zoom = 45.0f;
	}

	// private functions
	void ComponentCamera::UpdateCameraVectors()
	{
		// Calculate the new m_Forward vector
		/*vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Forward = glm::normalize(front);*/


		// Also re-calculate the m_Right and Up vector
		//m_Right = glm::normalize(glm::cross(m_Forward, m_WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		//m_CamUp = glm::normalize(glm::cross(m_Right, m_Forward));

		// view
		m_ViewMatrix->CreateLookAtView(m_Position, m_CamUp, m_Position - m_Forward); // view
		// m_ViewMatrix->CreateLookAtView(m_Position, m_CamUp, m_TargetPosition);
		// *m_ViewMatrix = glm::lookAt(m_Position, m_TargetPosition + m_Forward, m_CamUp);

		// projection
		m_ProjMatrix->CreatePerspectiveHFoV(m_Zoom * 0.5f, m_ViewportSize.x / m_ViewportSize.y, m_CAMNEAR, m_CAMFAR);
		// *m_ProjMatrix = glm::perspective(glm::radians(m_Zoom), m_ViewportSize.x / m_ViewportSize.y, m_CAMNEAR, m_CAMFAR);
	}

}
