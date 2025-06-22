#include "QF_ComponentMesh.h"

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#endif

#include "QF_Assets.h"
#include "QF_Mesh.h"
#include "QF_Mirror.h" // #TODO Needed for Release build, but should it be?
#include "QF_Shader.h"

namespace QwerkE {

    void ComponentMesh::Initialize()
    {
        m_Mesh = Assets::Get<QwerkE::Mesh>(m_MeshGuid);
        m_Shader = Assets::Get<Shader>(m_ShaderGuid);
    }

    void ComponentMesh::Draw(u16 viewId, const ComponentTransform& transform)
    {
        // #TODO Instanced rendering. Refer to bgfx example project, instancing.cpp line 242
        // void bgfxFramework::Mesh::submit(const MeshState* const* _state, uint8_t _numPasses, const float* _mtx, uint16_t _numMatrices) const;

        bgfx::setTransform(transform.GetMatrix());
        constexpr uint8_t stream = 0;
        bgfx::setVertexBuffer(stream, m_Mesh->m_vbh);
        bgfx::setIndexBuffer(m_Mesh->m_ibh);

        // #TODO Review setting renderer state
        // bgfx::setState(BGFX_STATE_DEFAULT); // uint64_t

        bgfx::submit(viewId, m_Shader->Program());
    }

}
