#ifndef _imgui_Editor_H_
#define _imgui_Editor_H_

#include "../Editor.h"

namespace QwerkE {

    class ResourceViewer;
    class ShaderEditor;
    class SceneViewer;
    class EntityEditor;

    // TODO: Consider deprecating header to use Editor.h
    class imgui_Editor : Editor
    {
    public:
        imgui_Editor();
        ~imgui_Editor();

        void NewFrame();
        void Update();
        void Draw();

        EntityEditor* GetEntityEditor() { return m_EntityEditor; }

        void ToggleEditorUi() { m_ShowingEditorGUI = !m_ShowingEditorGUI; };

    private:
        void RenderDockingContext();

        bool m_ShowingExampleWindow = false;
        bool m_ShowingShaderEditor = false;
        bool m_ShowingEditorGUI = true;
        ResourceViewer* m_ResourceViewer = nullptr;
        ShaderEditor* m_ShaderEditor = nullptr;
        SceneViewer* m_SceneViewer = nullptr;
        EntityEditor* m_EntityEditor = nullptr;
    };

}
#endif // _imgui_Editor_H_
