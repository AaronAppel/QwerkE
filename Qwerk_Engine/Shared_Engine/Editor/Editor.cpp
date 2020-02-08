#include "Editor.h"
#include "EntityEditor/EntityEditor.h"

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
