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

        const bgfx::ViewId viewIdFbo1 = 2; // #TODO Fix hard coded value

        bgfx::setTransform(transform.GetMatrix());
        bgfx::setVertexBuffer(0, m_Mesh->m_vbh);
        bgfx::setIndexBuffer(m_Mesh->m_ibh);
        bgfx::submit(viewIdFbo1, m_Shader->m_Program);
#endif
    }

}
