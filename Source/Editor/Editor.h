#ifndef _Editor_H_
#define _Editor_H_

// Class Editor handles all code related to the engine editor.
// Components include GUI windows, scene graph, shader creator, etc.

namespace QwerkE {

    class SceneGraph;
    class EntityEditor;

    // TODO: Add cross panel communication to support features such as
    // drag and drop textures, trigger callbacks, input, etc.
    // Or just stop using editor subclasses and switch to base functions only.

    class Editor // Abstract
    {
    public:
        virtual ~Editor() {}

        virtual void NewFrame() = 0;
        virtual void Update() = 0;
        virtual void Draw() = 0;

        virtual void ToggleEditorUi() = 0;

    protected: // #TODO private
        Editor() {}

        SceneGraph* m_SceneGraph;
        EntityEditor* m_EntityEditor; // Change routine/component data for an entity

        void* m_ActionWindow = nullptr; // TODO: Game and Scene windows. Multi scene views
        void* m_MenuBar = nullptr; // TODO: Top menu bar with access to settings like save, preferences, etc
        void* m_ResourcePanel = nullptr; // TODO: Access to resources and folder structure
    };

}
#endif // !_Editor_H_
