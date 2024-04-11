#pragma once

#include <string>
#include <vector>

#include "QE_EditComponent.h"
#include "QF_EntityHandle.h"
#include "QF_Enums.h"

namespace QwerkE {

    class EditComponent;

    class EntityEditor final
    {
    public:
        EntityEditor();

        virtual void Draw();

        void OnReset() { m_CurrentEntity.Invalidate(); }
        void SetCurrentEntity(const EntityHandle& newEntity) { m_CurrentEntity = newEntity; }

    private:
        void DrawEntityEditor();

        EntityHandle m_CurrentEntity;
        uPtr<EditComponent> m_EditComponent = nullptr;

        std::vector<const char*> m_ListboxItemStrings = { "Render" };
        std::vector<eComponentTags> m_ListboxItemTypes = { eComponentTags::Component_Render };

        // #TODO View Cherno video on improving imgui in Hazel : https://www.youtube.com/watch?v=oESRecjuLNY&ab_channel=TheCherno
        // Look at transform X, Y, X, colored buttons, better font usage in imgui, and templated draw components methods
    };

}
