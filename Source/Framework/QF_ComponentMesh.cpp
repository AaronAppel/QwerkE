#include "QF_ComponentMesh.h"

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#endif

#ifdef _QBGFXFRAMEWORK
#include <bgfxFramework/bgfx_utils.h>
#endif

#include "QC_Guid.h"

#include "QF_Assets.h"
#include "QF_ComponentTransform.h"

namespace QwerkE {

    void ComponentMesh::Initialize()
    {
        m_BgfxMesh = Assets::Get<bgfxFramework::Mesh>(m_MeshGuid);
        m_Mesh = Assets::Get<Mesh>(m_MeshGuid);
        m_Shader = Assets::Get<Shader>(m_ShaderGuid);
    }

    void ComponentMesh::Draw(u16 viewId, const ComponentTransform& transform)
    {
#ifdef _QDEBUG // #TESTING

        if (false)
        {
            bgfx::setTransform(transform.GetMatrix());
            bgfx::setVertexBuffer(0, m_Mesh->m_vbh);
            bgfx::setIndexBuffer(m_Mesh->m_ibh);

            bgfx::submit(viewId, m_Shader->m_Program);
        }
        else
        {
            // m_BgfxMesh->m_groups[0].m_vbh;
            // m_BgfxMesh->m_groups[0].m_ibh;
            m_BgfxMesh->submit(viewId, m_Shader->m_Program, transform.GetMatrix(), BGFX_STATE_DEFAULT);
            if (const bool pickingEnabled = true)
            {
                m_BgfxMesh->submit(3, m_Shader->m_Program, transform.GetMatrix(), BGFX_STATE_DEFAULT);
            }
        }
#endif
    }

}
