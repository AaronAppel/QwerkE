#pragma once

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QwerkE_imgui.h"
#endif

#include "QF_ComponentHelpers.h"
#include "QF_EntityHandle.h"
#include "QF_GameActions.h"
#include "QF_Input.h"
#include "QF_InputMouseDragTracker.h"
#include "QF_Math.h"
#include "QF_Scriptable.h"
#include "QF_Time.h"

#include "../Source/Editor/QE_Settings.h" // #TODO Remove from QF_* domain

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
			float m_MousePixelRatio = 5.f; // #TODO Review name and purpose. Higher values mean slower camera movement
		};

		Camera() = default;

		void Update(const Input::GameActions& gameActions, float* a_Matrix, float deltaTime)
		{
			vec2f cameraRotation = vec2f(0.0f, 0.0f);
			vec3f cameraMovement = { 0.0f, 0.0f, 0.0f };

			// #TODO Consider using contextualized input or polling the game state to know if the window is focused, or if the game is paused vs running

			cameraRotation.x -= Input::MouseDelta().x / m_CameraParams.m_MousePixelRatio * deltaTime;
			cameraRotation.y -= Input::MouseDelta().y / m_CameraParams.m_MousePixelRatio * deltaTime;

			const float mouseScroll = Input::MouseScrollDelta();
			if (mouseScroll != 0.f)
			{
				// #TODO Consider other FoV hotkey
				// m_EditorCamera.m_Fov -= mouseScroll;

				const float scrollSpeedModifier = 2.0f; // #TODO Expose in data
				cameraMovement += Math::MatrixForward(a_Matrix) * mouseScroll * scrollSpeedModifier;
			}

			if (Input::GamepadsCount > 0)
			{
				constexpr float controllerStickDeadzone = 0.15f;
				vec2f leftStick = Input::GamepadAxis(e_QGamepadAxisLeftStick);
				leftStick.x = (leftStick.x > controllerStickDeadzone || leftStick.x < -controllerStickDeadzone) ? leftStick.x : 0.0f;
				leftStick.y = (leftStick.y > controllerStickDeadzone || leftStick.y < -controllerStickDeadzone) ? leftStick.y : 0.0f;
				cameraMovement += leftStick.x * Math::MatrixRight(a_Matrix);
				cameraMovement -= leftStick.y * Math::MatrixForward(a_Matrix);

				if (Input::GamepadDown(e_GamepadBumperLeft))
				{
					cameraMovement += Math::MatrixDown(a_Matrix);
				}
				if (Input::GamepadDown(e_GamepadBumperRight))
				{
					cameraMovement += Math::MatrixUp(a_Matrix);
				}

				const float gamepadCameraRotationSpeed = 1.5f;
				if (Input::GamepadAxis(e_QGamepadAxisRightStick).x > controllerStickDeadzone)
				{
					cameraRotation.x += Input::GamepadAxis(e_QGamepadAxisRightStick).x * gamepadCameraRotationSpeed;
				}
				if (Input::GamepadAxis(e_QGamepadAxisRightStick).x < -controllerStickDeadzone)
				{
					cameraRotation.x += Input::GamepadAxis(e_QGamepadAxisRightStick).x * gamepadCameraRotationSpeed;
				}
				if (Input::GamepadAxis(e_QGamepadAxisRightStick).y > controllerStickDeadzone)
				{
					cameraRotation.y += Input::GamepadAxis(e_QGamepadAxisRightStick).y * gamepadCameraRotationSpeed;
				}
				if (Input::GamepadAxis(e_QGamepadAxisRightStick).y < -controllerStickDeadzone)
				{
					cameraRotation.y += Input::GamepadAxis(e_QGamepadAxisRightStick).y * gamepadCameraRotationSpeed;
				}
			}

			if (Input::KeyDown(gameActions.Camera_MoveForward))
			{
				cameraMovement += Math::MatrixForward(a_Matrix);
			}
			if (Input::KeyDown(gameActions.Camera_MoveBackward))
			{
				cameraMovement += Math::MatrixBackward(a_Matrix);
			}
			if (Input::KeyDown(gameActions.Camera_MoveRight))
			{
				cameraMovement += Math::MatrixRight(a_Matrix);
			}
			if (Input::KeyDown(gameActions.Camera_MoveLeft))
			{
				cameraMovement += Math::MatrixLeft(a_Matrix);
			}
			if (Input::KeyDown(gameActions.Camera_MoveDown))
			{
				cameraMovement += Math::MatrixDown(a_Matrix);
			}
			if (Input::KeyDown(gameActions.Camera_MoveUp))
			{
				cameraMovement += Math::MatrixUp(a_Matrix);
			}

			const float keyboardCameraRotationSpeed = 1.0f;
			if (Input::KeyDown(gameActions.Camera_RotateRight))
			{
				cameraRotation.x += keyboardCameraRotationSpeed;
			}
			if (Input::KeyDown(gameActions.Camera_RotateLeft))
			{
				cameraRotation.x -= keyboardCameraRotationSpeed;
			}

			// #TODO Balance values so maximum movement and rotation speed match for gamepad and KBM.
			// Both devices control the camera similarly at max input magnitude but gamepads can lerp between min and max.
			// Try to balance mouse movement to behave similar to gamepad in most cases, baring extreme mouse movement.

			if (cameraRotation.x != 0.0f || cameraRotation.y != 0.0f)
			{
				const float rotationSpeedMultiplier = Input::KeyDown(QKey::e_ShiftAny) ? 2.f : 1.f; // #TODO Expose in data

				m_CameraParams.m_Pitch -= cameraRotation.y * rotationSpeedMultiplier * Time::PreviousFrameDurationUnscaled();
				m_CameraParams.m_Yaw -= cameraRotation.x * rotationSpeedMultiplier * Time::PreviousFrameDurationUnscaled();

				// UpdateCameraRotation();

				const vec3f& scale = Math::MatrixScale(a_Matrix);
				const vec3f& translate = Math::MatrixPosition(a_Matrix);

				bx::mtxSRT(a_Matrix,
					scale.x, scale.y, scale.z,
					m_CameraParams.m_Pitch, m_CameraParams.m_Yaw, 0.f,
					translate.x, translate.y, translate.z);
			}

			if (cameraMovement.x != 0.0f || cameraMovement.y != 0.0f || cameraMovement.z != 0.0f)
			{
				const float moveSpeedMultiplier = Input::KeyDown(QKey::e_ShiftAny) ? 2.5f : 1.5f; // #TODO Expose in data
				Math::MatrixTranslate(a_Matrix, cameraMovement, m_CameraParams.m_MovementSpeed * moveSpeedMultiplier * Time::PreviousFrameDurationUnscaled());
			}
		}

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