#pragma once

#include "QF_EntityHandle.h"

namespace QwerkE {

    class EditComponent
    {
    public:
        void Draw(const EntityHandle& entity);

    private:
        unsigned int m_SelectedRenderableIndex = 0;

        bool m_ShowShaderList = true;
        bool m_ShowMaterialList = true;
        bool m_ShowMeshList = true;

        EntityHandle m_LastEntity;
    };

}
