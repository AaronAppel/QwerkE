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

        void SetDimensions(u8 rows, u8 columns) { m_Rows = rows; m_Columns = columns; }

    private:
        bgfx::VertexBufferHandle m_vbh;
        bgfx::IndexBufferHandle m_ibh;
        bgfx::ProgramHandle m_program;

        int64_t m_StartingTimeOffset = INT64_MIN;

        u8 m_Rows = 11;
        u8 m_Columns = 11;
    };

}
