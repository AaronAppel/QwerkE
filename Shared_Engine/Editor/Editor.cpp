#include "Editor.h"
#include "../Systems/Graphics/ShaderProgram/ShaderProgram.h"
#include "../../Shared_Generic/Utilities/FileIO/FileUtilities.h"
#include "../../Shared_Generic/Libraries/imgui/imgui.h"
#include "../Systems/ServiceLocator.h"
#include "../Systems/Input/InputManager.h"
#include "../Systems/Renderer.h"
#include "../Scene/SceneManager.h"

Editor::Editor()
{
	m_SceneGraph = new SceneGraph();
	m_SceneManager = (SceneManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Scene_Manager);
	m_Input = (InputManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Input_Manager);
}

Editor::~Editor()
{
	delete m_SceneGraph;
}