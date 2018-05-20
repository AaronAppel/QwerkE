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

extern int g_WindowWidth, g_WindowHeight; // TODO: Fix

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
	static bool isOpen = true;
	ImGui::Begin("Scene Window - Framework.cpp", &isOpen, ImGuiWindowFlags_NoScrollbar);

	// render scene to fbo
	m_FBO->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_SceneManager->Draw();
	m_FBO->UnBind();

	ImVec2 winSize = ImGui::GetWindowSize();

	ImVec2 imageSize = winSize;

	// TODO: Fix image width. A larger panel has more space on the right.
	// TODO: Consider centering image on panel.
	imageSize.x += winSize.x * 7.63f; // scale the width larger for upcoming divisions so window fits
	imageSize = ImVec2(imageSize.x / 9, imageSize.x / 16); // 16 x 9 resolution

	ImGui::SetWindowSize(ImVec2(winSize.x, imageSize.y + 35)); // snap window height to scale

	// render texture as image
	ImGui::Image(ImTextureID(m_FBO->GetTextureID()), imageSize, ImVec2(0, 1), ImVec2(1, 0));

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

		if (ImGui::Button(DispStrCombine(std::to_string(counter).c_str(), std::to_string(p.second->GetSceneID()).c_str()).c_str()) || m_Input->FrameKeyAction((eKeys)(eKeys::eKeys_0 + counter), eKeyState::eKeyState_Press))
		{
			m_SceneManager->SetCurrentScene(p.second->GetSceneID());
		}
		counter++;
	}

	ImGui::End();
}