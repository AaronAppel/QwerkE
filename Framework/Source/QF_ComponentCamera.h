#pragma once

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/math.h>
#ifdef _QBGFXFRAMEWORK
#include <bgfxFramework/debugDraw/debugdraw.h>
#endif
#endif

#include "QF_Input.h"
#include "QF_Log.h"
#include "QF_Renderer.h"
#include "QF_Settings.h"
#include "QF_Window.h"

// #TODO Move to data values and deprecate hard coding
static constexpr float gc_YAW = -90.f;	// #TODO Include the word "Default" in variable names?
static constexpr float gc_PITCH = 0.f;	// #TODO Load from data
static constexpr float gc_SPEED = 10.f;
static constexpr float gc_SENSITIVTY = 0.1f;
static constexpr float gc_ZOOM = 45.f;

namespace QwerkE {

    struct ComponentCamera
    {
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

            const vec2f& mouseScroll = QwerkE::Input::MouseScrollDelta();
            if (QwerkE::Input::MouseScrollDelta().x != 0.f || QwerkE::Input::MouseScrollDelta().y != 0.f)
            {
            }
        }

        void PreDrawSetup(const bgfx::ViewId viewId)
        {
            bx::mtxLookAt(m_View, m_Eye, m_At);

            const vec2f& windowSize = Window::GetSize();
            bx::mtxProj(m_Proj, m_Fov, windowSize.x / windowSize.y, m_Near, m_Far, bgfx::getCaps()->homogeneousDepth);
            bgfx::setViewTransform(viewId, m_View, m_Proj);

            {
                DebugDrawEncoder& debugDrawer = Renderer::DebugDrawer();
                debugDrawer.begin(viewId);
                debugDrawer.drawSphere(m_At.x, m_At.y, m_At.z, 3.f);
                debugDrawer.end();
            }
        }

        bx::Vec3 m_At = { .0f, .0f,   .0f };
        bx::Vec3 m_Eye = { .0f, .0f, -35.f };

        float m_Fov = 60.f;
        float m_Near = .1f;
        float m_Far = 100.f;

        float m_View[16];
        float m_Proj[16];
    };

}
