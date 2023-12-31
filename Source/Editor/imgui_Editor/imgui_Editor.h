#pragma once

#include "../Editor.h"

namespace QwerkE {

    class EntityEditor;
    class ResourceViewer;
    class SceneViewer;
    class ShaderEditor;

    // #TODO Consider deprecating header to use Editor.h
    class imgui_Editor : Editor
    {
    public:
        imgui_Editor();
        ~imgui_Editor();

        void Draw() override;

        EntityEditor* GetEntityEditor() { return m_EntityEditor; }

        void ToggleEditorUi() { m_ShowingEditorGUI = !m_ShowingEditorGUI; };

    private:
        void RenderDockingContext();
        void DrawMainMenuBar();

        bool m_ShowingExampleWindow = false;
        bool m_ShowingShaderEditor = false;
        bool m_ShowingEditorGUI = true;
        ResourceViewer* m_ResourceViewer = nullptr;
        ShaderEditor* m_ShaderEditor = nullptr;
        SceneViewer* m_SceneViewer = nullptr;
        EntityEditor* m_EntityEditor = nullptr;
    };

}
