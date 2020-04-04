#ifndef _Entity_Editor_H_
#define _Entity_Editor_H_

// A window that shows data for an entity and gives a GUI
//     to allow for data to easily be modified.

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
#endif // _Entity_Editor_H_
