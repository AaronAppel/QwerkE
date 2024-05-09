#pragma once

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QC_imgui.h"
#endif

#include "QF_ComponentHelpers.h"

#include "QC_Time.h"

#include "QF_EntityHandle.h"
#include "QF_Input.h"
#include "QF_InputMouseDragTracker.h"
#include "QF_Math.h"
#include "QF_Scriptable.h"

#include "../Editor/Source/QE_Settings.h" // #TODO Remove from QF_* domain

namespace QwerkE {

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
			ComponentTransform& transform = m_Entity.GetComponent<ComponentTransform>();

			static float pixelRatio = 5.f; // #TODO Review name and purpose. Higher values mean slower camera movement

			m_MouseDragTracker.Update();
			if (m_MouseDragTracker.IsActive())
			{
				static float yaw = 0.f;
				yaw -= m_MouseDragTracker.MouseFrameDelta().x / pixelRatio * deltaTime;

				// Pitch transform.m_Matrix[6];
				static float pitch = 0.f;
				pitch -= m_MouseDragTracker.MouseFrameDelta().y / pixelRatio * deltaTime;

				constexpr bx::Vec3 scale = { 1.f, 1.f, 1.f };
				bx::Vec3 rotate = { pitch, yaw, 0.f };
				const vec3f& translate = transform.GetPosition();

				bx::mtxSRT(transform.m_Matrix,
					scale.x, scale.y, scale.z,
					rotate.x, rotate.y, rotate.z,
					translate.x, translate.y, translate.z);
			}

			const vec3f transformForward = transform.Forward(); // #TODO Calculate and use proper forward
			const bx::Vec3 forward =
			{
				transformForward.x,
				transformForward.y,
				transformForward.z
			};

			// #TODO Review how hotkeys are managed by framework and customized by game
			eKeys key_camera_MoveForward = eKeys::eKeys_W;
			eKeys key_camera_MoveBackward = eKeys::eKeys_S;
			eKeys key_camera_MoveLeft = eKeys::eKeys_A;
			eKeys key_camera_MoveRight = eKeys::eKeys_D;
			eKeys key_camera_MoveUp = eKeys::eKeys_E;
			eKeys key_camera_MoveDown = eKeys::eKeys_Q;
			eKeys key_camera_RotateLeft = eKeys::eKeys_R;
			eKeys key_camera_RotateRight = eKeys::eKeys_T;

#ifdef _QEDITOR // #TODO Review moving editor logic out of framework
			const UserSettings& userSettings = Settings::GetUserSettings(); // #TODO Review as UserSettings adds dependency between QF and QE
			key_camera_MoveForward = userSettings.key_camera_MoveForward;
			key_camera_MoveBackward = userSettings.key_camera_MoveBackward;
			key_camera_MoveLeft = userSettings.key_camera_MoveLeft;
			key_camera_MoveRight = userSettings.key_camera_MoveRight;
			key_camera_MoveUp = userSettings.key_camera_MoveUp;
			key_camera_MoveDown = userSettings.key_camera_MoveDown;
			key_camera_RotateLeft = userSettings.key_camera_RotateLeft;
			key_camera_RotateRight = userSettings.key_camera_RotateRight;
#endif
			if (Input::IsKeyDown(key_camera_MoveForward))
			{
				vec3f pos = transform.GetPosition();
				bx::Vec3 eye = bx::mad(forward, deltaTime * camera.m_MoveSpeed, bx::Vec3(pos.x, pos.y, pos.z));
				transform.SetPosition(vec3f(eye.x, eye.y, eye.z));
				// transform.m_Matrix[14] += (camera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
			}
			if (Input::IsKeyDown(key_camera_MoveBackward))
			{
				vec3f pos = transform.GetPosition();
				bx::Vec3 eye = bx::mad(forward, -deltaTime * camera.m_MoveSpeed, bx::Vec3(pos.x, pos.y, pos.z));
				transform.SetPosition(vec3f(eye.x, eye.y, eye.z));
				// transform.m_Matrix[14] -= (camera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
			}

			const bx::Vec3 right =
			{
				transform.m_Matrix[0],
				transform.m_Matrix[4],
				transform.m_Matrix[8]
			};

			if (Input::IsKeyDown(key_camera_MoveLeft))
			{
				vec3f pos = transform.GetPosition();
				bx::Vec3 eye = bx::mad(right, -deltaTime * camera.m_MoveSpeed, bx::Vec3(pos.x, pos.y, pos.z));
				transform.SetPosition(vec3f(eye.x, eye.y, eye.z));
				// transform.m_Matrix[12] -= (camera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
			}
			if (Input::IsKeyDown(key_camera_MoveRight))
			{
				vec3f pos = transform.GetPosition();
				bx::Vec3 eye = bx::mad(right, deltaTime * camera.m_MoveSpeed, bx::Vec3(pos.x, pos.y, pos.z));
				transform.SetPosition(vec3f(eye.x, eye.y, eye.z));
				// transform.m_Matrix[12] += (camera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
			}

			const bx::Vec3 up = bx::cross(right, forward);

			if (Input::IsKeyDown(key_camera_MoveDown))
			{
				transform.m_Matrix[13] += (camera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
			}
			if (Input::IsKeyDown(key_camera_MoveUp))
			{
				transform.m_Matrix[13] -= (camera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
			}
			if (Input::IsKeyDown(key_camera_RotateRight))
			{
				LOG_TRACE("{0} Camera rotate right", __FUNCTION__);
			}
			if (Input::IsKeyDown(key_camera_RotateLeft))
			{
				constexpr float rotationSpeed = Math::PI_f();
				bx::mtxRotateXYZ(transform.m_Matrix, 0.f, rotationSpeed * deltaTime, 0.f);
				LOG_TRACE("{0} Camera rotate left", __FUNCTION__);
			}

			const vec2f& mouseScroll = Input::MouseScrollDelta();
			if (mouseScroll.y != 0.f)
			{
				camera.m_Fov -= mouseScroll.y;
			}

			if (const bool useTargetLookAt = false)
			{
				// #TODO Add option to reference an existing transform component in the scene
				vec3f targetPosition = camera.m_LookAtPosition;
				camera.m_LookAtPosition = targetPosition;
			}
			else if (const bool useDirectionalLookAt = true)
			{
				// #TODO Only re-calculate if forward changed
				constexpr float scalar = 1.f;
				const float* position = nullptr;
				vec3f forwardPosition = transform.GetPosition() + (transform.Forward() * scalar);
				camera.m_LookAtPosition = forwardPosition;
			}

			m_up = bx::cross(right, forward);

#ifdef _QEDITOR // #TODO This shouldn't be in the QC domain
			ImGui::DefaultDebugWindow([&]()
			{
				ImGui::DragFloat("PixelRatio", &pixelRatio, .05f);

				vec2f mouseTrackerDelta = m_MouseDragTracker.MouseFrameDelta();
				ImGui::DragFloat2("MouseDragTracker", &mouseTrackerDelta.x, .05f);

				vec3f forward = transformForward;
				ImGui::DragFloat3("TransformForward", &forward.x, .05f);
			});
#endif
		}

		eScriptTypes ScriptType() override
		{
			return eScriptTypes::Camera;
		}

		Input::MouseDragTracker m_MouseDragTracker = Input::MouseDragTracker(eKeys::eKeys_MouseButton2);

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

		// #TESTING New camera movement
		bx::Vec3 m_up = bx::InitZero;
		float m_horizontalAngle;
		float m_verticalAngle;
	};

}