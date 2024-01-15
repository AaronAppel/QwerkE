#pragma once

namespace QwerkE {

    class SceneGraph;
    class EntityEditor;
    class ResourceViewer;
    class SceneViewer;
    class ShaderEditor;

    // FEATURE: Add cross panel communication to support features such as
    // drag and drop assets, trigger callbacks, input, etc.
    // Or just stop using editor subclasses and switch to base functions only.

    class Editor
    {
    public:
        virtual void NewFrame() {}
        virtual void Update() {}
        virtual void Draw() = 0;

        virtual void ToggleEditorUi() = 0;

        EntityEditor* GetEntityEditor() { return m_EntityEditor; }

    protected:
        Editor();
        virtual ~Editor();

        EntityEditor* m_EntityEditor = nullptr;
        SceneGraph* m_SceneGraph = nullptr;
        ShaderEditor* m_ShaderEditor = nullptr;
        ResourceViewer* m_ResourceViewer = nullptr;
        SceneViewer* m_SceneViewer = nullptr;

        void* m_ActionWindow = nullptr; // FEATURE: Game and Scene windows. Multi scene views
        void* m_MenuBar = nullptr;
        void* m_ResourcePanel = nullptr;
    };

}
