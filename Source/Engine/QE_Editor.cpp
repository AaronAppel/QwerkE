#include "QE_Editor.h"

#include "QE_EntityEditor.h"
#include "QE_ResourceViewer.h"
#include "QE_ShaderEditor.h"
#include "QE_SceneGraph.h"
#include "QE_SceneViewer.h"

namespace QwerkE {

    Editor::Editor()
    {
        m_EntityEditor = new EntityEditor();
        m_SceneGraph = new SceneGraph(m_EntityEditor);
        m_ResourceViewer = new ResourceViewer();
        m_ShaderEditor = new ShaderEditor();
        m_SceneViewer = new SceneViewer();
    }

    Editor::~Editor()
    {
        delete m_EntityEditor;
        delete m_SceneGraph;
        delete m_ResourceViewer;
        delete m_ShaderEditor;
        delete m_SceneViewer;
    }

}
