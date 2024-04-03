#pragma once

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/math.h>
#ifdef _QBGFXFRAMEWORK
#include <bgfxFramework/debugDraw/debugdraw.h>
#endif
#endif

#include "QF_Renderer.h"
#include "QF_Window.h"

namespace QwerkE {

    struct ComponentCamera
    {
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

        float m_MoveSpeed = 1.f;

        bx::Vec3 m_At = { .0f, .0f,   .0f };
        bx::Vec3 m_Eye = { .0f, .0f, -35.f };

        float m_Fov = 60.f;
        float m_Near = .1f;
        float m_Far = 100.f;

        float m_View[16];
        float m_Proj[16];
    };

}
