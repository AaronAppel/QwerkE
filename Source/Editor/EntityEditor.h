#pragma once

#include <string>

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
    };

}
