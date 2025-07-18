#pragma once

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
#endif

#include "QC_Guid.h"

#include "QF_ComponentTransform.h"

namespace bgfxFramework {
    struct Mesh;
}

namespace QwerkE {

    class Mesh;
    class Shader;
    class Texture;

    class ComponentMesh
    {
    public:
        void Initialize();
        void Draw(u16 viewId, const ComponentTransform& transform);

        void SetMeshGuid(const GUID& newMeshGuid) { m_MeshGuid = newMeshGuid; }
        void SetShaderGuid(const GUID& newShaderGuid) { m_ShaderGuid = newShaderGuid; }
        void SetTextureGuid(const GUID& newShaderGuid) { m_TextureGuid = newShaderGuid; }

        const GUID& GetMeshGuid() { return m_MeshGuid; }
        const GUID& GetShaderGuid() { return m_ShaderGuid; }
        const GUID& GetTextureGuid() { return m_TextureGuid; }

    private:
        MIRROR_PRIVATE_MEMBERS

        Mesh* m_Mesh = nullptr;
        Shader* m_Shader = nullptr;
        Texture* m_Texture = nullptr;

        GUID m_MeshGuid = GUID::Invalid;
        GUID m_ShaderGuid = GUID::Invalid;
        GUID m_TextureGuid = GUID::Invalid;

        bool m_SetUvOffsetUniform = false; // #TODO Improve
    };

}
