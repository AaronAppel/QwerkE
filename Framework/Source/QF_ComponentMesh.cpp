#include "QF_ComponentMesh.h"

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#include <bx/bx.h>
#ifdef _QDEBUG
#include <bgfxFramework/debugDraw/debugdraw.h>
#endif // _QDEBUG
#endif // _QBGFX

#include "QF_Assets.h"
#include "QF_ComponentTransform.h"
#include "QF_Renderer.h"
#include "QF_Window.h"

namespace QwerkE {

    ComponentMesh::ComponentMesh()
    {
        // #TODO Use GUIDs. Can take in as arguments, but serialization "CAN" set initial values.
        // Could assign data in memory, then emplace new to use empty/default constructor to get proper mesh data
        // from data member values.

        m_Mesh = Assets::Get<Mesh>(GUID::Invalid); // #TODO Get proper guid
        m_Shader = Assets::Get<Shader>(GUID::Invalid); // #TODO Get proper guid
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

        bgfx::setTransform(transform.GetMatrix());
        bgfx::setVertexBuffer(0, m_Mesh->m_vbh);
        bgfx::setIndexBuffer(m_Mesh->m_ibh);
        bgfx::submit(viewIdFbo1, m_Shader->m_Program);
#endif
    }

}
