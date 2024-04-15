#pragma once

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QC_imgui.h"
#endif

#include "QF_ComponentCamera.h"
#include "QF_ComponentTransform.h"
#include "QF_ComponentScript.h"

#include "QC_Time.h"

#include "QF_EntityHandle.h"
#include "QF_Input.h"
#include "QF_InputMouseDragTracker.h"
#include "QF_Math.h"
#include "QF_Scriptable.h"
#include "QF_Scripting.h"
#include "QF_Settings.h"

namespace QwerkE {

	class ScriptableCamera : public Scriptable
	{
	public:
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

			ImGui::DefaultDebugWindow([&]()
			{
				ImGui::DragFloat("PixelRatio", &pixelRatio, .05f);

				vec2f mouseTrackerDelta = m_MouseDragTracker.MouseFrameDelta();
				ImGui::DragFloat2("MouseDragTracker", &mouseTrackerDelta.x, .05f);
			});

			const vec3f transformForward = transform.Forward();

			const bx::Vec3 direction =
			{
				bx::cos(camera.m_verticalAngle) * bx::sin(camera.m_horizontalAngle),
				bx::sin(camera.m_verticalAngle),
				bx::cos(camera.m_verticalAngle) * bx::cos(camera.m_horizontalAngle),
			};

			const bx::Vec3 right =
			{
				bx::sin(camera.m_horizontalAngle - bx::kPiHalf),
				0.0f,
				bx::cos(camera.m_horizontalAngle - bx::kPiHalf),
			};

			const UserSettings& userSettings = Settings::GetUserSettings();
			if (Input::IsKeyDown(userSettings.key_camera_MoveForward))
			{
				vec3f pos = transform.GetPosition();
				bx::Vec3 eye = bx::mad(direction, deltaTime * camera.m_MoveSpeed, bx::Vec3(pos.x, pos.y, pos.z));
				transform.SetPosition(vec3f(eye.x, eye.y, eye.z));
				// transform.m_Matrix[14] += (camera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
			}
			if (Input::IsKeyDown(userSettings.key_camera_MoveBackward))
			{
				vec3f pos = transform.GetPosition();
				bx::Vec3 eye = bx::mad(direction, -deltaTime * camera.m_MoveSpeed, bx::Vec3(pos.x, pos.y, pos.z));
				transform.SetPosition(vec3f(eye.x, eye.y, eye.z));
				// transform.m_Matrix[14] -= (camera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
			}
			if (Input::IsKeyDown(userSettings.key_camera_MoveLeft))
			{
				transform.m_Matrix[12] -= (camera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
			}
			if (Input::IsKeyDown(userSettings.key_camera_MoveRight))
			{
				transform.m_Matrix[12] += (camera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
			}

			const bx::Vec3 up = bx::cross(right, direction);
			if (Input::IsKeyDown(userSettings.key_camera_MoveDown))
			{
				transform.m_Matrix[13] += (camera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
			}
			if (Input::IsKeyDown(userSettings.key_camera_MoveUp))
			{
				transform.m_Matrix[13] -= (camera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
			}
			if (Input::IsKeyDown(userSettings.key_camera_RotateRight))
			{
				LOG_TRACE("{0} Camera rotate right", __FUNCTION__);
			}
			if (Input::IsKeyDown(userSettings.key_camera_RotateLeft))
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
				vec3f targetPosition = vec3f();
				camera.m_LookAtPosition = targetPosition;
			}
			else if (const bool useDirectionalLookAt = true)
			{
				// #TODO Only re-calculate if forward changed
				constexpr float scalar = 1.f;
				vec3f forwardPosition = transform.GetPosition() + (transform.Forward() * scalar);
				camera.m_LookAtPosition = forwardPosition;
			}

			camera.m_up = bx::cross(right, direction);
		}

		eScriptTypes ScriptType() override
		{
			return eScriptTypes::Camera;
		}

		Input::MouseDragTracker m_MouseDragTracker = Input::MouseDragTracker(eKeys::eKeys_MouseButton2);
		bool orbit = false;
	};

}