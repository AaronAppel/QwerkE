#include "QF_ComponentMesh.h"

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/math.h>
#include <bx/timer.h>
#ifdef _QDEBUG
#include <bgfxFramework/debugDraw/debugdraw.h>
#endif // _QDEBUG
#endif // _QBGFX

#include "QF_Assets.h"
#include "QF_ComponentTransform.h"
#include "QF_Renderer.h"
#include "QF_Window.h"

namespace QwerkE {

    void ComponentMesh::Initialize() // #TODO Move to Assets or a graphics data loader/creator
    {
        m_StartingTimeOffset = bx::getHPCounter();

        m_vbh = Assets::GetVbh();
        m_ibh = Assets::GetIbh();
        m_program = Assets::GetProgram();
    }

    void ComponentMesh::Draw(const ComponentTransform& transform)
    {
#ifdef _QDEBUG // #TESTING

        const bgfx::ViewId viewIdFbo1 = 2; // #TODO Fix hard coded value

        if (false)
        {   // Debug drawer calls
            DebugDrawEncoder& debugDrawer = Renderer::DebugDrawer(); // #TESTING
            debugDrawer.begin(viewIdFbo1);

            const bx::Vec3 normal = { .0f,  1.f, .0f };
            const bx::Vec3 pos = { .0f, -2.f, .0f };

            bx::Plane plane(bx::InitNone);
            bx::calcPlane(plane, normal, pos);

            debugDrawer.drawGrid(Axis::Y, pos, 50, 1.0f);

            debugDrawer.end();
        }

        {
            const float time = (float)((bx::getHPCounter() - m_StartingTimeOffset) / double(bx::getHPFrequency()));

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
            // bgfx::setState(state);

            // Submit primitive for rendering to view 0.
            bgfx::submit(viewIdFbo1, m_program);
        }
#endif
    }

}
