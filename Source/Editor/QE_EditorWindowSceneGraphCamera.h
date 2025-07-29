#pragma once

#include "Libraries/bx/include/bx/math.h"

#include "QF_Input.h"

namespace QwerkE {

	namespace Editor {

		class Camera
		{
		public:
			Camera()
			{
				Reset();
				// Update(0.0f);
			}

			void Reset()
			{
				m_eye.x = 0.0f;
				m_eye.y = 0.0f;
				m_eye.z = -35.0f;
				m_at.x = 0.0f;
				m_at.y = 0.0f;
				m_at.z = -1.0f;
				m_up.x = 0.0f;
				m_up.y = 1.0f;
				m_up.z = 0.0f;

				m_horizontalAngle = 0.01f;
				m_verticalAngle = 0.0f;
				m_mouseSpeed = 0.0020f;
				m_gamepadSpeed = 0.04f;
				m_moveSpeed = 30.0f;
				m_mouseWasDown = false;
			}

			void Update(float _deltaTime, float* _viewMtx)
			{
				vec2f mousePos = Input::MousePos();
				const Input::GameActions& gameActions = Input::GetGameActions();
				const float moveSpeedMultiplier = Input::KeyDown(QKey::e_ShiftAny) ? 8.f : 1.f; // #TODO Move hard coded value
				constexpr float controllerStickDeadzone = 0.15f;

				const float deltaZ = Input::MouseScrollDelta();

				if (m_mouseWasDown)
				{
					vec2f mouseDelta = Input::MouseDelta();

					m_horizontalAngle += m_mouseSpeed * float(mouseDelta.x);
					m_verticalAngle -= m_mouseSpeed * float(mouseDelta.y);
				}

				// entry::GamepadHandle handle = { 0 };
				// m_horizontalAngle += m_gamepadSpeed * inputGetGamepadAxis(handle, entry::GamepadAxis::RightX)/32768.0f;
				// m_verticalAngle   -= m_gamepadSpeed * inputGetGamepadAxis(handle, entry::GamepadAxis::RightY)/32768.0f;
				// const int32_t gpx = inputGetGamepadAxis(handle, entry::GamepadAxis::LeftX);
				// const int32_t gpy = inputGetGamepadAxis(handle, entry::GamepadAxis::LeftY);
				// m_keys |= gpx < -16834 ? CAMERA_KEY_LEFT     : 0;
				// m_keys |= gpx >  16834 ? CAMERA_KEY_RIGHT    : 0;
				// m_keys |= gpy < -16834 ? CAMERA_KEY_FORWARD  : 0;
				// m_keys |= gpy >  16834 ? CAMERA_KEY_BACKWARD : 0;

				const bx::Vec3 forward =
				{
					bx::cos(m_verticalAngle) * bx::sin(m_horizontalAngle),
					bx::sin(m_verticalAngle),
					bx::cos(m_verticalAngle) * bx::cos(m_horizontalAngle),
				};

				const bx::Vec3 right =
				{
					bx::sin(m_horizontalAngle - bx::kPiHalf),
					0.0f,
					bx::cos(m_horizontalAngle - bx::kPiHalf),
				};

				const bx::Vec3 up = bx::cross(right, forward);

				m_eye = bx::mad(forward, deltaZ * _deltaTime * m_moveSpeed, m_eye);

				if (Input::KeyDown(gameActions.Camera_MoveForward) || Input::GamepadAxis(e_QGamepadAxisLeftStick).y < -controllerStickDeadzone)
				{
					m_eye = bx::mad(forward, _deltaTime * m_moveSpeed, m_eye);
				}

				if (Input::KeyDown(gameActions.Camera_MoveBackward) || Input::GamepadAxis(e_QGamepadAxisLeftStick).y > controllerStickDeadzone)
				{
					m_eye = bx::mad(forward, -_deltaTime * m_moveSpeed, m_eye);
				}

				if (Input::KeyDown(gameActions.Camera_MoveLeft) || Input::GamepadAxis(e_QGamepadAxisLeftStick).x < -controllerStickDeadzone)
				{
					m_eye = bx::mad(right, _deltaTime * m_moveSpeed, m_eye);
				}

				if (Input::KeyDown(gameActions.Camera_MoveRight) || Input::GamepadAxis(e_QGamepadAxisLeftStick).x > controllerStickDeadzone)
				{
					m_eye = bx::mad(right, -_deltaTime * m_moveSpeed, m_eye);
				}

				if (Input::KeyDown(gameActions.Camera_MoveUp) || Input::GamepadDown(e_GamepadBumperRight))
				{
					m_eye = bx::mad(up, _deltaTime * m_moveSpeed, m_eye);
				}

				if (Input::KeyDown(gameActions.Camera_MoveDown) || Input::GamepadDown(e_GamepadBumperLeft))
				{
					m_eye = bx::mad(up, -_deltaTime * m_moveSpeed, m_eye);
				}

				m_at = bx::add(m_eye, forward);
				m_up = bx::cross(right, forward);
				bx::mtxLookAt(_viewMtx, bx::load<bx::Vec3>(&m_eye.x), bx::load<bx::Vec3>(&m_at.x), bx::load<bx::Vec3>(&m_up.x));
			}
		};

	}

}
