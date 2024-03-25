#pragma once

#include "QF_Input.h"
#include "QF_Log.h"
#include "QF_Settings.h"

struct ComponentCamera
{
    int placeholder = 0; // So entt allows emplacing

    void Move()
    {
        const QwerkE::UserSettings& userSettings = QwerkE::Settings::GetUserSettings();

        if (QwerkE::Input::GetIsKeyDown(userSettings.key_camera_MoveForward))
        {
            LOG_TRACE("{0} Camera move forward", __FUNCTION__);
        }
        if (QwerkE::Input::GetIsKeyDown(userSettings.key_camera_MoveBackward))
        {
            LOG_TRACE("{0} Camera move backward", __FUNCTION__);
        }
        if (QwerkE::Input::GetIsKeyDown(userSettings.key_camera_MoveLeft))
        {
            LOG_TRACE("{0} Camera move left", __FUNCTION__);
        }
        if (QwerkE::Input::GetIsKeyDown(userSettings.key_camera_MoveRight))
        {
            LOG_TRACE("{0} Camera move right", __FUNCTION__);
        }
        if (QwerkE::Input::GetIsKeyDown(userSettings.key_camera_MoveDown))
        {
            LOG_TRACE("{0} Camera move down", __FUNCTION__);
        }
        if (QwerkE::Input::GetIsKeyDown(userSettings.key_camera_MoveUp))
        {
            LOG_TRACE("{0} Camera move up", __FUNCTION__);
        }
        if (QwerkE::Input::GetIsKeyDown(userSettings.key_camera_RotateRight))
        {
            LOG_TRACE("{0} Camera rotate right", __FUNCTION__);
        }
        if (QwerkE::Input::GetIsKeyDown(userSettings.key_camera_RotateLeft))
        {
            LOG_TRACE("{0} Camera rotate left", __FUNCTION__);
        }

        const vec2& mouseScroll = QwerkE::Input::MouseScrollDelta();
        if (QwerkE::Input::MouseScrollDelta().x != 0.f || QwerkE::Input::MouseScrollDelta().y != 0.f)
        {
        }
    }
};