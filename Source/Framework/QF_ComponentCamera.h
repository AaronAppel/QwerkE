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

            bgfx::setViewRect(viewId, 0, 0, uint16_t(windowSize.x), uint16_t(windowSize.y));

            bx::Vec3 eye = { position.x, position.y, position.z };
            bx::Vec3 at = { m_LookAtPosition.x, m_LookAtPosition.y, m_LookAtPosition.z };
            bx::mtxLookAt(m_View, eye, at);

            bx::mtxProj(m_Proj, m_Fov, windowSize.x / windowSize.y, m_Near, m_Far, bgfx::getCaps()->homogeneousDepth);
            bgfx::setViewTransform(viewId, m_View, m_Proj);

#ifdef _QDEBUG
            if (m_ShowSphere)
            {
                DebugDrawEncoder& debugDrawer = Renderer::DebugDrawer();
                debugDrawer.begin(viewId);
                debugDrawer.drawSphere(m_LookAtPosition.x, m_LookAtPosition.y, m_LookAtPosition.z, 3.f);
                debugDrawer.end();
            }
#endif
        }

        // #TODO How will Mirror handle undefined types? May need to handle with warning or special #define macro
        bool m_ShowSphere = true;

        float m_MoveSpeed = 5.f;

        vec3f m_LookAtPosition = { .0f, .0f, .0f };

        float m_Fov = 60.f;
        float m_Near = .1f;
        float m_Far = 100.f;

        float m_View[16];
        float m_Proj[16];
    };

}
