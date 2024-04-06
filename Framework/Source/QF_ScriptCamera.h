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
				m_Entity.RemoveComponent<ComponentScript>();
			}
		}

		void OnUpdate(float deltaTime) override
		{
			if (!m_Entity.HasComponent<ComponentTransform>() ||
				!m_Entity.HasComponent<ComponentCamera>())
			{
				LOG_ERROR("Entity ___ missing ScriptableCamera!");
				m_Entity.RemoveComponent<ComponentScript>();
			}

			const QwerkE::UserSettings& userSettings = QwerkE::Settings::GetUserSettings();

			ComponentCamera& camera = m_Entity.GetComponent<ComponentCamera>();
			if (QwerkE::Input::GetIsKeyDown(userSettings.key_camera_MoveForward))
			{
				camera.m_Eye.z = camera.m_Eye.z + (camera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
			}
			if (QwerkE::Input::GetIsKeyDown(userSettings.key_camera_MoveBackward))
			{
				camera.m_Eye.z = camera.m_Eye.z - (camera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
			}
			if (QwerkE::Input::GetIsKeyDown(userSettings.key_camera_MoveLeft))
			{
				camera.m_Eye.x = camera.m_Eye.x - (camera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
			}
			if (QwerkE::Input::GetIsKeyDown(userSettings.key_camera_MoveRight))
			{
				camera.m_Eye.x = camera.m_Eye.x + (camera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
			}
			if (QwerkE::Input::GetIsKeyDown(userSettings.key_camera_MoveDown))
			{
				camera.m_Eye.y = camera.m_Eye.y + (camera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
			}
			if (QwerkE::Input::GetIsKeyDown(userSettings.key_camera_MoveUp))
			{
				camera.m_Eye.y = camera.m_Eye.y - (camera.m_MoveSpeed * (float)Time::PreviousFrameDuration());
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
	};

}