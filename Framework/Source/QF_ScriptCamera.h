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
#include "QF_Math.h"
#include "QF_Scriptable.h"
#include "QF_Settings.h"

namespace QwerkE {

	class ScriptableCamera : public Scriptable
	{
	public:
		void OnCreate() override
		{
			if (!m_Entity.HasComponent<ComponentTransform>() ||
				!m_Entity.HasComponent<ComponentCamera>())
			{
				LOG_ERROR("Entity {0} missing ScriptableCamera!", m_Entity.GetComponent<ComponentInfo>().m_EditorDisplayName);

				if (m_Entity.HasComponent<ComponentScript>())
				{
					ComponentScript& script = m_Entity.GetComponent<ComponentScript>();
					script.RemoveScript(eScriptTypes::Camera);
				}
				else
				{
					LOG_CRITICAL("Could not remove ComponentScript!");
				}
			}
		}

		void OnUpdate(float deltaTime) override
		{
			if (!m_Entity.HasComponent<ComponentTransform>() ||
				!m_Entity.HasComponent<ComponentCamera>())
			{
				LOG_ERROR("Entity ___ missing ScriptableCamera!");

				if (m_Entity.HasComponent<ComponentScript>())
				{
					ComponentScript& script = m_Entity.GetComponent<ComponentScript>();
					script.RemoveScript(eScriptTypes::Camera);
				}
				else
				{
					LOG_CRITICAL("Could not remove ComponentScript!");
				}
			}

			if (!m_Entity.HasComponent<ComponentScript>())
				return;

			ComponentCamera& camera = m_Entity.GetComponent<ComponentCamera>();
			ComponentTransform& transform = m_Entity.GetComponent<ComponentTransform>();

			vec4f rotationVec;
			vec2f mouseFrameDelta = Input::MouseFrameDelta();
			if (Input::IsKeyDown(eKeys::eKeys_MouseButton2))
			{
				// bx::mtxRotateXYZ(rotation, -mouseFrameDelta.y * deltaTime, -mouseFrameDelta.x * deltaTime, 0.f);
				// bx::mtxMul(transform.m_Matrix, transform.m_Matrix, rotation);

				static float yaw = 0.f;
				yaw += mouseFrameDelta.x * deltaTime;

				constexpr bx::Vec3 scale = { 1.f, 1.f, 1.f };
				bx::Vec3 rotate = { 0.f, yaw, 0.f };
				const vec3f& translate = transform.GetPosition();

				bx::mtxSRT(transform.m_Matrix,
					scale.x, scale.y, scale.z,
					rotate.x, rotate.y, rotate.z,
					translate.x, translate.y, translate.z);

				rotationVec.y = -mouseFrameDelta.y;
				rotationVec.x = -mouseFrameDelta.x;

				// bx::vec4MulMtx(transform.m_Matrix, &rotationVec.x, transform.m_Matrix);
				// camera.m_LookAtPosition = ;
			}

			if (ImGui::Begin("MouseFrameDelta"))
			{
				ImGui::DragFloat2("MouseFrameDelta", &mouseFrameDelta.x, .1f);
			}
			ImGui::End();

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
				constexpr float rotationSpeed = Math::PI();
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

		bool orbit = false;
	};

}