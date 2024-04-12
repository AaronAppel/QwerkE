#include "QF_ComponentMesh.h"

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#include <bx/bx.h>
#endif // _QBGFX

#include "QC_Guid.h"

#include "QF_Assets.h"
#include "QF_ComponentTransform.h"

namespace QwerkE {

    ComponentMesh::ComponentMesh()
    {
        m_Mesh = Assets::Get<Mesh>(GUID::Invalid); // #TODO Get proper guid
        m_Shader = Assets::Get<Shader>(GUID::Invalid); // #TODO Get proper guid
    }

    void ComponentMesh::Draw(const ComponentTransform& transform)
    {
#ifdef _QDEBUG // #TESTING

        uint64_t state = 0 // #TESTING
            | BGFX_STATE_WRITE_R
            | BGFX_STATE_WRITE_G
            | BGFX_STATE_WRITE_B
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | BGFX_STATE_CULL_CW
            | BGFX_STATE_MSAA
            | 0
            ;
        // bgfx::setState(state);

        bgfx::setTransform(transform.GetMatrix());
        bgfx::setVertexBuffer(0, m_Mesh->m_vbh);
        bgfx::setIndexBuffer(m_Mesh->m_ibh);

        const bgfx::ViewId viewIdFbo1 = 2; // #TODO Fix hard coded value
        bgfx::submit(viewIdFbo1, m_Shader->m_Program);
#endif
    }

}
