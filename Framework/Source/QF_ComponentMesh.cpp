#include "QF_ComponentMesh.h"

#ifdef _QDEBUG
#ifdef _QBGFX
#include <bgfx/bgfx.h>
#include <bx/timer.h>
#ifdef _QBGFXFRAMEWORK
#include <bgfxFramework/SampleRenderData.h>
#include <bgfxFramework/LoadShader.h>
#include <bgfxFramework/debugDraw/debugdraw.h>
#endif
#endif
#endif

#include "QF_ComponentTransform.h"
#include "QF_Renderer.h"
#include "QF_Window.h"

namespace QwerkE {

    namespace Renderer {

        const bgfx::ViewId s_ViewIdFbo1 = 2;

        extern bgfx::FrameBufferHandle s_FrameBufferHandle;
        extern const u8 s_FrameBufferTextureCount = 2;
        extern bgfx::TextureHandle s_FrameBufferTextures[s_FrameBufferTextureCount];
        extern bgfx::TextureHandle s_ReadBackTexture; // #TODO Destroy

    }

    void ComponentMesh::Create()
    {
        m_StartingTimeOffset = bx::getHPCounter();

        // #TESTING
        // Create static vertex buffer.
        m_vbh = bgfx::createVertexBuffer(
            // Static data can be passed with bgfx::makeRef
            bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices))
            , PosColorVertex::ms_layout
        );

        m_ibh = bgfx::createIndexBuffer(
            // Static data can be passed with bgfx::makeRef
            bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList))
        );

        // Create program from shaders.
        m_program = myLoadShaderProgram("vs_cubes.bin", "fs_cubes.bin");
    }

    void ComponentMesh::Destroy()
    {
        bgfx::destroy(m_ibh);
        bgfx::destroy(m_vbh);
        bgfx::destroy(m_program);

        // m_Registry.destroy(); // #TODO Deallocate entt
    }

    void ComponentMesh::Draw(const ComponentTransform& transform)
    {
#ifdef _QDEBUG // #TESTING

        const vec2f& windowSize = Window::GetSize();

        bgfx::setViewName(Renderer::s_ViewIdFbo1, "FBO1");
        bgfx::setViewClear(Renderer::s_ViewIdFbo1, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
        bgfx::setViewRect(Renderer::s_ViewIdFbo1, 0, 0, uint16_t(windowSize.x), uint16_t(windowSize.y));

        bgfx::setViewFrameBuffer(Renderer::s_ViewIdFbo1, Renderer::s_FrameBufferHandle);
        bgfx::touch(Renderer::s_ViewIdFbo1);

        {   // Debug drawer calls
            DebugDrawEncoder& debugDrawer = Renderer::DebugDrawer(); // #TESTING
            debugDrawer.begin(Renderer::s_ViewIdFbo1);

            const bx::Vec3 normal = { .0f,  1.f, .0f };
            const bx::Vec3 pos = { .0f, -2.f, .0f };

            bx::Plane plane(bx::InitNone);
            bx::calcPlane(plane, normal, pos);

            debugDrawer.drawGrid(Axis::Y, pos, 50, 1.0f);

            debugDrawer.end();
        }

        {
            const float time = (float)((bx::getHPCounter() - m_StartingTimeOffset) / double(bx::getHPFrequency()));

            const bx::Vec3 at = { .0f, .0f,   .0f };
            const bx::Vec3 eye = { .0f, .0f, -35.f };

            // Set view and projection matrix for view 0.
            {
                float view[16];
                bx::mtxLookAt(view, eye, at);

                float proj[16];
                bx::mtxProj(proj, 60.f, windowSize.x / windowSize.y, .1f, 100.f, bgfx::getCaps()->homogeneousDepth);
                bgfx::setViewTransform(Renderer::s_ViewIdFbo1, view, proj);

                // Set view 0 default viewport.
                // bgfx::setViewRect(Renderer::s_ViewIdFbo1, 0, 0, uint16_t(windowSize.x), uint16_t(windowSize.y));
            }

            // This dummy draw call is here to make sure that view 0 is cleared
            // if no other draw calls are submitted to view 0.

            uint64_t state = 0
                | BGFX_STATE_WRITE_R
                | BGFX_STATE_WRITE_G
                | BGFX_STATE_WRITE_B
                | BGFX_STATE_WRITE_A
                | BGFX_STATE_WRITE_Z
                | BGFX_STATE_DEPTH_TEST_LESS
                | BGFX_STATE_CULL_CW
                | BGFX_STATE_MSAA
                | 0 // #REVIEW
                ;

            // bx::mtxRotateXY(m_Matrix, time + m_Matrix[12] * 0.21f, time + m_Matrix[13] * 0.37f);

            //* Rotates
            // m_Matrix[12] = m_Position.x;
            // m_Matrix[13] = m_Position.y;
            // m_Matrix[14] = m_Position.z;

            /*/ // Doesn't rotate
            bx::mtxTranslate(m_Matrix, m_Position.x, m_Position.y, m_Position.z);
            //*/

            // Set model matrix for rendering.
            bgfx::setTransform(transform.GetMatrix());

            // Set vertex and index buffer.
            bgfx::setVertexBuffer(0, m_vbh);
            bgfx::setIndexBuffer(m_ibh);

            // Set render states.
            bgfx::setState(state);

            // Submit primitive for rendering to view 0.
            bgfx::submit(Renderer::s_ViewIdFbo1, m_program);
        }
#endif
    }

}
