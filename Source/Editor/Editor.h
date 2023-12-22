#pragma once

namespace QwerkE {

    class SceneGraph;
    class EntityEditor;

    // FEATURE: Add cross panel communication to support features such as
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

        SceneGraph* m_SceneGraph = nullptr;
        EntityEditor* m_EntityEditor = nullptr;

        void* m_ActionWindow = nullptr; // FEATURE: Game and Scene windows. Multi scene views
        void* m_MenuBar = nullptr;
        void* m_ResourcePanel = nullptr;
    };

}
