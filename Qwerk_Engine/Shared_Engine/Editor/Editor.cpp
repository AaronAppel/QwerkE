#include "Editor.h"
#include "../QwerkE_Framework/Systems/Services.h"
#include "../QwerkE_Framework/Systems/Input/Input.h"
#include "EntityEditor/EntityEditor.h"

namespace QwerkE {

    Editor::Editor()
    {
        m_SceneGraph = new SceneGraph(this);
        m_Input = (Input*)QwerkE::Services::GetService(eEngineServices::Input_Manager);
    }

    Editor::~Editor()
    {
        delete m_SceneGraph;
        delete m_EntityEditor;
    }

}
