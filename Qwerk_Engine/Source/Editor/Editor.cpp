#include "Editor.h"

// TODO: if (IMGUI_EDITOR)
#include "imgui_Editor/SceneGraph/imgui_SceneGraph.h"

namespace QwerkE {

    Editor::Editor()
    {
        m_SceneGraph = new SceneGraph(this);
    }

    Editor::~Editor()
    {
        delete m_SceneGraph;
        delete m_EntityEditor;
    }

}
