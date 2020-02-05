#include "Editor.h"
#include "../QwerkE_Framework/QwerkE_Common/Utilities/StringHelpers.h"
#include "../QwerkE_Framework/QwerkE_Common/Utilities/FileIO/FileUtilities.h"
#include "../QwerkE_Framework/QwerkE_Common/Libraries/imgui/imgui.h"
#include "../QwerkE_Framework/Systems/Services.h"
#include "../QwerkE_Framework/Systems/Input/InputManager.h"
#include "../QwerkE_Framework/Systems/Renderer/Renderer.h"
#include "../QwerkE_Framework/Systems/SceneManager.h"
#include "EntityEditor/EntityEditor.h"

namespace QwerkE {

    Editor::Editor()
    {
        m_SceneGraph = new SceneGraph(this);
        m_SceneManager = (SceneManager*)QwerkE::Services::GetService(eEngineServices::Scene_Manager);
        m_Input = (InputManager*)QwerkE::Services::GetService(eEngineServices::Input_Manager);
    }

    Editor::~Editor()
    {
        delete m_SceneGraph;
        delete m_EntityEditor;
    }

}
