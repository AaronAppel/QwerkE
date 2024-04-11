#pragma once

#ifdef _QBGFX
#include <bgfx/bgfx.h>
#endif

#include "QF_ComponentTransform.h"
#include "QF_Mesh.h"
#include "QF_Shader.h"

namespace QwerkE {

    class ComponentMesh
    {
    public:
        ComponentMesh();

        void Draw(const ComponentTransform& transform);

    private:
        Mesh* m_Mesh = nullptr;
        Shader* m_Shader = nullptr;
    };

}
