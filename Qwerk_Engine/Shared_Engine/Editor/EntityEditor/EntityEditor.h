#ifndef _Entity_Editor_H_
#define _Entity_Editor_H_

// A window that shows data for an entity and gives a GUI
// to allow for data to easily be modified

#include <string>

namespace QwerkE {

    // TODO: Create an imgui_EntityEditor class
    class Scene;
    class GameObject;
    class Editor;
    class imgui_EditComponent;

    // TODO: Change this to be an abstract base class that gets inherited by library version like ImGui or BlockMenu
    class EntityEditor // abstract
    {
    public:
        EntityEditor(Editor* editor);
        ~EntityEditor();

        virtual void Draw();

        void SetCurrentEntity(GameObject* object) { m_CurrentEntity = object; };

    private:
        void DrawEntityEditor();

        Editor* m_Editor = nullptr;

        GameObject* m_CurrentEntity = nullptr;
        imgui_EditComponent* m_EditComponent = nullptr;
    };

}
#endif // !_Entity_Editor_H_
