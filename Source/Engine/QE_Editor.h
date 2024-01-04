#pragma once

namespace QwerkE {

    class SceneGraph;
    class EntityEditor;

    // FEATURE: Add cross panel communication to support features such as
    // drag and drop textures, trigger callbacks, input, etc.
    // Or just stop using editor subclasses and switch to base functions only.

    class Editor
    {
    public:
        virtual ~Editor() {}

        virtual void NewFrame() {}
        virtual void Update() {}
        virtual void Draw() = 0;

        virtual void ToggleEditorUi() = 0;

    protected: // #TODO What should be private?
        Editor() {}

        SceneGraph* m_SceneGraph = nullptr;
        EntityEditor* m_EntityEditor = nullptr;

        void* m_ActionWindow = nullptr; // FEATURE: Game and Scene windows. Multi scene views
        void* m_MenuBar = nullptr;
        void* m_ResourcePanel = nullptr;
    };

}
