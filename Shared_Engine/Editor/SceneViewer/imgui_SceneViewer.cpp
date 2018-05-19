#include "imgui_SceneViewer.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/ServiceLocator.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Graphics/ShaderProgram/ShaderProgram.h"
#include "../QwerkE_Framework/QwerkE_Common/Utilities/FileIO/FileUtilities.h"
#include "../QwerkE_Framework/QwerkE_Common/Libraries/imgui/imgui.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Input/InputManager.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Renderer.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/SceneManager.h"
#include "../QwerkE_Framework/QwerkE_Framework/Components/Time.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Graphics/Gfx_Classes/FrameBufferObject.h"
#include "../QwerkE_Framework/QwerkE_Framework/Scenes/Scene.h"
#include "../QwerkE_Framework/QwerkE_Common/Utilities/StringHelpers.h"

imgui_SceneViewer::imgui_SceneViewer()
{
	m_SceneManager = (SceneManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Scene_Manager);
	m_Scenes = m_SceneManager->LookAtScenes();
	m_Input = (InputManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Input_Manager);
	m_FBO = new FrameBufferObject();
	m_FBO->Init();
}

imgui_SceneViewer::~imgui_SceneViewer()
{
	delete m_FBO;
}

void imgui_SceneViewer::NewFrame()
{
}

void imgui_SceneViewer::Update()
{

}

void imgui_SceneViewer::Draw()
{
	DrawSceneView();
	DrawSceneList();
}

void imgui_SceneViewer::DrawSceneView()
{
	// render scene to fbo
	m_FBO->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_SceneManager->Draw();
	m_FBO->UnBind();

	ImGui::Begin("Scene Window - Framework.cpp");
	ImGui::Image(ImTextureID(m_FBO->GetTextureID()), ImVec2(1600 / 3, 900 / 3), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
}

void imgui_SceneViewer::DrawSceneList()
{
	ImGui::Begin("Scene List");

	// TODO: Think of adding multi window support for viewing more than 1
	// enabled scene at a time. Going to have to look at how input would
	// work for that.
	int counter = 0;
	ImVec2 winSize = ImGui::GetWindowSize();
	ImVec2 imageSize = ImVec2(100.0f, 100.0f);
	int m_ItemsPerRow = winSize.x / (imageSize.x * 1.5f) + 1; // (* up the image size for feel), + avoid dividing by 0

	for (const auto &p : *m_SceneManager->LookAtScenes()) // save pointer?
	{
		if (counter % m_ItemsPerRow)
			ImGui::SameLine();

		if (ImGui::Button(DispStrCombine(std::to_string(counter).c_str(), std::to_string(p.second->GetSceneID()).c_str()).c_str()) || m_Input->GetIsKeyDown((eKeys)(eKeys::eKeys_0 + counter)))
		{
			m_SceneManager->SetCurrentScene(p.second->GetSceneID());
		}
		counter++;
	}

	ImGui::End();
}