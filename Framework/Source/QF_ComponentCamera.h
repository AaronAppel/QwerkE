#pragma once

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/math.h>
#ifdef _QBGFXFRAMEWORK
#include <bgfxFramework/debugDraw/debugdraw.h>
#endif
#endif

#include "QF_ComponentTransform.h"
#include "QF_Renderer.h"
#include "QF_Window.h"

namespace QwerkE {

    namespace Renderer {

        extern bgfx::FrameBufferHandle s_FrameBufferHandle; // #TODO Move to better place (Renderer?)

    }

    struct ComponentCamera
    {
        void PreDrawSetup(const bgfx::ViewId viewId, const vec3f& position)
        {
            const vec2f& windowSize = Window::GetSize();

            bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
            bgfx::setViewRect(viewId, 0, 0, uint16_t(windowSize.x), uint16_t(windowSize.y));

            bgfx::setViewFrameBuffer(viewId, Renderer::s_FrameBufferHandle);
            bgfx::touch(viewId);

            bx::Vec3 eye = { position.x, position.y, position.z };
            bx::Vec3 at = { m_LookAtPosition.x, m_LookAtPosition.y, m_LookAtPosition.z };
            bx::mtxLookAt(m_View, eye, at);

            bx::mtxProj(m_Proj, m_Fov, windowSize.x / windowSize.y, m_Near, m_Far, bgfx::getCaps()->homogeneousDepth);
            bgfx::setViewTransform(viewId, m_View, m_Proj);

            if (m_ShowSphere)
            {
                DebugDrawEncoder& debugDrawer = Renderer::DebugDrawer();
                debugDrawer.begin(viewId);
                debugDrawer.drawSphere(m_LookAtPosition.x, m_LookAtPosition.y, m_LookAtPosition.z, 3.f);
                debugDrawer.end();
            }
        }

        bool m_ShowSphere = true;

        float m_MoveSpeed = 5.f;

        vec3f m_LookAtPosition = { .0f, .0f, .0f };

        float m_Fov = 60.f;
        float m_Near = .1f;
        float m_Far = 100.f;

        float m_View[16];
        float m_Proj[16];

        // #TESTING New camera movement
        bx::Vec3 m_up = bx::InitZero;
        float m_horizontalAngle;
        float m_verticalAngle;
    };

}
