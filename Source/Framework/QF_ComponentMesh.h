#pragma once

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/Mirror.h"
#endif

#include "QC_Guid.h"

#include "QF_ComponentTransform.h"
#include "QF_Mesh.h"
#include "QF_Shader.h"

namespace bgfxFramework {
    struct Mesh;
}

namespace QwerkE {

    class ComponentMesh
    {
    public:
        void Initialize();
        void Draw(u16 viewId, const ComponentTransform& transform);

    private:
        MIRROR_PRIVATE_MEMBERS

        bgfxFramework::Mesh* m_BgfxMesh = nullptr;
        Mesh* m_Mesh = nullptr;
        Shader* m_Shader = nullptr;

        GUID m_MeshGuid = GUID::Invalid;
        GUID m_ShaderGuid = GUID::Invalid;
    };

}
