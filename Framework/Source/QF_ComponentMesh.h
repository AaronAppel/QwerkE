#pragma once

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#endif

#include "QF_ComponentTransform.h"

namespace QwerkE {

    class ComponentMesh
    {
    public:
        ComponentMesh() = default;

        void Initialize();

        void Draw(const ComponentTransform& transform);

    private:
        bgfx::VertexBufferHandle m_vbh;
        bgfx::IndexBufferHandle m_ibh;
        bgfx::ProgramHandle m_program;

        int64_t m_StartingTimeOffset = INT64_MIN;
    };

}
