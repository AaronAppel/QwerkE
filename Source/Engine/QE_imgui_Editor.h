#pragma once

#include "QE_Editor.h"

namespace QwerkE {

    // #TODO Consider deprecating header to use Editor.h
    class imgui_Editor : Editor
    {
    public:
        void Draw() override;

        EntityEditor* GetEntityEditor() { return m_EntityEditor; }

        void ToggleEditorUi() { m_ShowingEditorGUI = !m_ShowingEditorGUI; };

    private:
        void RenderDockingContext();
        void DrawMainMenuBar();

        bool m_ShowingExampleWindow = false;
        bool m_ShowingShaderEditor = false;
        bool m_ShowingEditorGUI = true;
    };

}
