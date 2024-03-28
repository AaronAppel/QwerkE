#pragma once

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#endif

namespace QwerkE {

    class MeshComponent
    {
    public:
        MeshComponent() = default;

        void Create();
        void Destroy();

        void Draw();

    private:
        bgfx::VertexBufferHandle m_vbh;
        bgfx::IndexBufferHandle m_ibh;
        bgfx::ProgramHandle m_program;

        int64_t m_StartingTimeOffset = INT64_MIN;
    };

}
