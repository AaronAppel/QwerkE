#include "QF_ComponentMesh.h"

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#endif

#include "QF_Assets.h"
#include "QF_Mesh.h"
#include "QF_Mirror.h" // #TODO Needed for Release build, but should it be?
#include "QF_Shader.h"
#include "QF_Texture.h"

namespace QwerkE {

    bgfx::TextureHandle s_TextureHandle;
    bgfx::UniformHandle s_UniformHandleTexture;
    bgfx::UniformHandle s_UniformHandleUvOffset;
    vec4f uvOffset = vec4f(1, 1, 0, 0);

    void ComponentMesh::Initialize()
    {
        m_Mesh = Assets::Get<QwerkE::Mesh>(m_MeshGuid);
        m_Shader = Assets::Get<Shader>(m_ShaderGuid);
        if (m_TextureGuid && m_Shader && m_Shader->HasTextureUniform())
        {
            m_Texture = Assets::Get<Texture>(m_TextureGuid);
            s_TextureHandle.idx = m_Texture->TextureHandle().idx;
            s_UniformHandleTexture = bgfx::createUniform("u_TextureColor", bgfx::UniformType::Sampler);

            // if (7235877850588061835 == m_ShaderGuid) // #TODO Remove hard coded shader guid
            if (m_Shader->HasUniform("u_UVScaleOffset"))
            {
                s_UniformHandleUvOffset = bgfx::createUniform("u_UVScaleOffset", bgfx::UniformType::Vec4);
                if (bgfx::isValid(s_UniformHandleUvOffset))
                {
                    bgfx::setUniform(s_UniformHandleUvOffset, &uvOffset); // #NOTE Currently static so can submit once at init time
                    m_SetUvOffsetUniform = true;
                }
            }
        }

        // #TODO Destroy uniform and texture handles
    }

    void ComponentMesh::Draw(u16 viewId, const ComponentTransform& transform)
    {
        // #TODO Instanced rendering. Refer to bgfx example project, instancing.cpp line 242
        // void bgfxFramework::Mesh::submit(const MeshState* const* _state, uint8_t _numPasses, const float* _mtx, uint16_t _numMatrices) const;

        if (m_TextureGuid && m_Shader->HasTextureUniform())
        {
            // #TODO Set uniform;
            // u_TextureColor
            // uniform vec4 u_Time;
            // uniform vec4 u_UVScaleOffset;
            // bgfx::setTexture(0, s_UniformHandle, s_TextureHandle);

            if (m_SetUvOffsetUniform) // #TODO Remove hard coded shader guid
            {
                // #TODO Create Material class to handle setting multiple textures
                // #TODO Review adding uniform helpers to Texture
                // bgfx::setUniform(s_UniformHandleUvOffset, &uvOffset);
                bgfx::setTexture(0, s_UniformHandleTexture, s_TextureHandle); // #NOTE Needs to be set every frame, even if other textures are not submitted
            }
        }

        // m_Shader->PreDraw();

        // LOG_ONCE_ERROR();

        bgfx::setTransform(transform.GetMatrix());
        constexpr uint8_t stream = 0;
        bgfx::setVertexBuffer(stream, m_Mesh->m_vbh); // #TODO Mesh->PreDraw(); abstract members
        bgfx::setIndexBuffer(m_Mesh->m_ibh);

        bgfx::setState(0
            | BGFX_STATE_WRITE_RGB
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | BGFX_STATE_CULL_CCW
            | BGFX_STATE_MSAA
        );

        bgfx::submit(viewId, m_Shader->ProgramHandle());
    }

}
