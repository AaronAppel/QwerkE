#pragma once

#include <string>
#include <vector>

#include "QF_Enums.h"

namespace QwerkE {

    class GameObject;
    class EditComponent;

    class EntityEditor
    {
    public:
        EntityEditor();
        ~EntityEditor();

        virtual void Draw();

        void SetCurrentEntity(GameObject* object) { m_CurrentEntity = object; };

    private:
        void DrawEntityEditor();

        GameObject* m_CurrentEntity = nullptr;
        EditComponent* m_EditComponent = nullptr;

        std::vector<const char*> m_ListboxItemStrings = { "Render" };
        std::vector<eComponentTags> m_ListboxItemTypes = { eComponentTags::Component_Render };
    };

}
