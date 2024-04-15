#include "QF_ComponentMesh.h"

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#include <bx/bx.h>
#endif // _QBGFX

#include "QC_Guid.h"

#include "QF_Assets.h"
#include "QF_ComponentTransform.h"

namespace QwerkE {

    void ComponentMesh::Initialize()
    {
        m_Mesh = Assets::Get<Mesh>(m_MeshGuid);
        m_Shader = Assets::Get<Shader>(m_ShaderGuid);
    }

    void ComponentMesh::Draw(const ComponentTransform& transform)
    {
#ifdef _QDEBUG // #TESTING

        bgfx::setTransform(transform.GetMatrix());
        bgfx::setVertexBuffer(0, m_Mesh->m_vbh);
        bgfx::setIndexBuffer(m_Mesh->m_ibh);

        const bgfx::ViewId viewIdFbo1 = 2; // #TODO Fix hard coded value
        bgfx::submit(viewIdFbo1, m_Shader->m_Program);
#endif
    }

}
