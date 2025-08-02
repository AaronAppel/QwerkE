#pragma once

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QwerkE_imgui.h"
#endif

#include "QC_Math.h"
#include "QC_Time.h"

#include "QF_ComponentHelpers.h"

#include "QF_EntityHandle.h"
#include "QF_GameActions.h"
#include "QF_Input.h"
#include "QF_InputMouseDragTracker.h"
#include "QF_Scriptable.h"

#include "../Source/Editor/QE_Settings.h" // #TODO Remove from QF_* domain

#ifdef _QGLM
#include "Libraries/glm/glm.hpp"
#include "Libraries/glm/gtc/matrix_transform.hpp"
#include "Libraries/glm/gtc/constants.hpp"
#endif // _QGLM

namespace QwerkE {

	// #TODO See how hard/complex it would be to support 1 camera class that handles all types of behaviour.
	// First person, third person, orthographic, perspective, and whatever else
	class Camera
	{
	public:

		struct CameraParams
		{
			enum LookAtMode
			{
				LookForward = 0,
				LookAtPosition
			};

			CameraParams() = default;

			// #TODO Serialize members
			float m_Pitch = 0.0f;
			float m_Yaw = 0.0f;
			float m_Fov = 45.f;

			Input::MouseDragTracker m_MouseDragTracker = Input::MouseDragTracker(QKey::e_MouseButton2);

			LookAtMode m_LookAtMode = LookForward;

			float m_MovementSpeed = 5.f;
			float m_RotationSpeed = 5.f;
		};

		Camera() = default;

		void Move(float* a_Matrix, const vec3f& a_PositionOffset)
		{
			Math::MatrixTranslate(a_Matrix, a_PositionOffset, 1.f);
		}

		void Move(float* a_Matrix, const vec3f& a_Axis, const float a_Magnitude)
		{
			Math::MatrixTranslate(a_Matrix, a_Axis, a_Magnitude);
		}

		void Pitch(float* a_Matrix, float a_PitchOffset)
		{
			m_CameraParams.m_Pitch += a_PitchOffset;
			UpdateCameraRotation(a_Matrix);
		}

		void Yaw(float* a_Matrix, float a_YawOffset)
		{
			m_CameraParams.m_Yaw += a_YawOffset;
			UpdateCameraRotation(a_Matrix);
		}

		void Zoom(const float a_FovIncrement)
		{
			m_CameraParams.m_Fov += a_FovIncrement;
		}

	private:
		void UpdateCameraRotation(float* a_Matrix) {
			const vec3f& scale = Math::MatrixScale(a_Matrix);
			const vec3f& translate = Math::MatrixPosition(a_Matrix);

			bx::mtxSRT(a_Matrix,
				scale.x, scale.y, scale.z,
				m_CameraParams.m_Pitch, m_CameraParams.m_Yaw, 0.f,
				translate.x, translate.y, translate.z);
		}

		CameraParams m_CameraParams;
	};

}