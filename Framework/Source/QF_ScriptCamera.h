#pragma once

#include "QF_ComponentCamera.h"
#include "QF_ComponentTransform.h"

#include "QC_Time.h"

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
				LOG_ERROR("Entity ___ missing ScriptableCamera!");

				if (m_Entity.HasComponent<ComponentScript>())
				{
					m_Entity.RemoveComponent<ComponentScript>();
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
					m_Entity.RemoveComponent<ComponentScript>();
				}
				else
				{
					LOG_CRITICAL("Could not remove ComponentScript!");
				}
			}


			if (!m_Entity.HasComponent<ComponentScript>())
				return;

			const QwerkE::UserSettings& userSettings = QwerkE::Settings::GetUserSettings();

			ComponentCamera& camera = m_Entity.GetComponent<ComponentCamera>();
			ComponentTransform& transform = m_Entity.GetComponent<ComponentTransform>();

			if (QwerkE::Input::GetIsKeyDown(userSettings.key_camera_MoveForward))
			{
				transform.m_Matrix[14] += (camera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
			}
			if (QwerkE::Input::GetIsKeyDown(userSettings.key_camera_MoveBackward))
			{
				transform.m_Matrix[14] -= (camera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
			}
			if (QwerkE::Input::GetIsKeyDown(userSettings.key_camera_MoveLeft))
			{
				transform.m_Matrix[12] -= (camera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
			}
			if (QwerkE::Input::GetIsKeyDown(userSettings.key_camera_MoveRight))
			{
				transform.m_Matrix[12] += (camera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
			}
			if (QwerkE::Input::GetIsKeyDown(userSettings.key_camera_MoveDown))
			{
				transform.m_Matrix[13] += (camera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
			}
			if (QwerkE::Input::GetIsKeyDown(userSettings.key_camera_MoveUp))
			{
				transform.m_Matrix[13] -= (camera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
			}
			if (QwerkE::Input::GetIsKeyDown(userSettings.key_camera_RotateRight))
			{
				LOG_TRACE("{0} Camera rotate right", __FUNCTION__);
			}
			if (QwerkE::Input::GetIsKeyDown(userSettings.key_camera_RotateLeft))
			{
				LOG_TRACE("{0} Camera rotate left", __FUNCTION__);
			}

			const vec2f& mouseScroll = QwerkE::Input::MouseScrollDelta();
			if (QwerkE::Input::MouseScrollDelta().x != 0.f || QwerkE::Input::MouseScrollDelta().y != 0.f)
			{
			}
		}

		eScriptTypes ScriptType()
		{
			return eScriptTypes::Camera;
		}

	};

}