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
	class ScriptableCamera : public Scriptable
	{
	public:
		ScriptableCamera() = default;

		void OnCreate() override
		{
			HasRequiredComponents<ComponentTransform, ComponentCamera, ComponentScript>();
		}

		void OnUpdate(float deltaTime) override
		{
			if (!HasRequiredComponents<ComponentTransform, ComponentCamera, ComponentScript>())
				return;

			ComponentCamera& camera = m_Entity.GetComponent<ComponentCamera>();
			ComponentTransform& cameraTransform = m_Entity.GetComponent<ComponentTransform>();

			const Input::GameActions& gameActions = Input::GetGameActions();

			vec2f cameraRotation = vec2f(0.0f, 0.0f);
			vec3f cameraMovement = { 0.0f, 0.0f, 0.0f };

            // #TODO Consider using contextualized input or polling the game state to know if the window is focused, or if the game is paused vs running

            static float pixelRatio = 5.f; // #TODO Review name and purpose. Higher values mean slower camera movement
			cameraRotation.x -= Input::MouseDelta().x / pixelRatio * deltaTime;
			cameraRotation.y -= Input::MouseDelta().y / pixelRatio * deltaTime;

			const float mouseScroll = Input::MouseScrollDelta();
			if (mouseScroll != 0.f)
			{
				// #TODO Consider other FoV hotkey
				// m_EditorCamera.m_Fov -= mouseScroll;

				const float scrollSpeedModifier = 2.0f; // #TODO Expose in data
				cameraMovement += cameraTransform.Forward() * mouseScroll * scrollSpeedModifier;
			}

            if (Input::GamepadsCount > 0)
            {
                constexpr float controllerStickDeadzone = 0.15f;
                vec2f leftStick = Input::GamepadAxis(e_QGamepadAxisLeftStick);
                leftStick.x = (leftStick.x > controllerStickDeadzone || leftStick.x < -controllerStickDeadzone) ? leftStick.x : 0.0f;
                leftStick.y = (leftStick.y > controllerStickDeadzone || leftStick.y < -controllerStickDeadzone) ? leftStick.y : 0.0f;
                cameraMovement += leftStick.x * cameraTransform.Right();
                cameraMovement -= leftStick.y * cameraTransform.Forward();

                if (Input::GamepadDown(e_GamepadBumperLeft))
                {
                    cameraMovement += cameraTransform.Down();
                }
                if (Input::GamepadDown(e_GamepadBumperRight))
                {
                    cameraMovement += cameraTransform.Up();
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
				cameraMovement += cameraTransform.Forward();
            }
            if (Input::KeyDown(gameActions.Camera_MoveBackward))
            {
				cameraMovement += cameraTransform.Backward();
            }
            if (Input::KeyDown(gameActions.Camera_MoveRight))
            {
				cameraMovement += cameraTransform.Right();
            }
            if (Input::KeyDown(gameActions.Camera_MoveLeft))
            {
				cameraMovement += cameraTransform.Left();
            }
            if (Input::KeyDown(gameActions.Camera_MoveDown))
            {
                cameraMovement += cameraTransform.Down();
            }
            if (Input::KeyDown(gameActions.Camera_MoveUp))
            {
				cameraMovement += cameraTransform.Up();
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

                m_Pitch -= cameraRotation.y * rotationSpeedMultiplier * Time::PreviousFrameDurationUnscaled();
                m_Yaw -= cameraRotation.x * rotationSpeedMultiplier * Time::PreviousFrameDurationUnscaled();

				// UpdateCameraRotation();

				const vec3f& scale = cameraTransform.Scale();
				const vec3f& translate = cameraTransform.Position();

				bx::mtxSRT(cameraTransform.m_Matrix,
					scale.x, scale.y, scale.z,
					m_Pitch, m_Yaw, 0.f,
					translate.x, translate.y, translate.z);
            }

            if (cameraMovement.x != 0.0f || cameraMovement.y != 0.0f || cameraMovement.z != 0.0f)
            {
                const float moveSpeedMultiplier = Input::KeyDown(QKey::e_ShiftAny) ? 2.5f : 1.5f; // #TODO Expose in data
				cameraTransform.Translate(cameraMovement, m_MovementSpeed * moveSpeedMultiplier * Time::PreviousFrameDurationUnscaled());
            }

			// #TODO Support m_RotationMode

#if _QDEBUG
			if (constexpr bool debugUiEnabled = true)
			{
				ImGui::DefaultWindow([&]() {
					ImGui::DragFloat("PixelRatio", &pixelRatio, .05f);

					vec2f mouseTrackerDelta = m_MouseDragTracker.MouseFrameDelta();
					ImGui::DragFloat2("MouseDragTracker", &mouseTrackerDelta.x, .05f);

					vec3f forward = cameraTransform.Forward();
					ImGui::DragFloat3("TransformForward", &forward.x, .05f);
					});
			}
#endif // _QDEBUG
		}

		eScriptTypes ScriptType() override
		{
			return eScriptTypes::e_Camera;
		}

		Input::MouseDragTracker m_MouseDragTracker = Input::MouseDragTracker(QKey::e_MouseButton2);

		enum RotationMode
		{
			OrbitTarget = 0,
			OrbitSelf
		};

		enum LookAtMode
		{
			LookAtTarget = 0,
			LookForward
		};

		bool m_RotationMode = OrbitSelf;

		// #TODO Serialize pitch and yaw
		float m_Pitch = 0.0f;
		float m_Yaw = 0.0f;
		float m_MovementSpeed = 5.f;
		float m_RotationSpeed = 5.f;
	};

}