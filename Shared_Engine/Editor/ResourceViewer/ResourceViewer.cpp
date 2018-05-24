#include "ResourceViewer.h"
// TODO: Why can't I include imgui.h? include order?
//#include "../../QwerkE_Framework/QwerkE_Common/Libraries/imgui/imgui.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/ServiceLocator.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/ResourceManager/ResourceManager.h"
#include "../QwerkE_Framework/QwerkE_Framework/Graphics/FrameBufferObject.h"
#include "../QwerkE_Framework/QwerkE_Framework/Graphics/GraphicsUtilities/GraphicsHelpers.h"
#include "../QwerkE_Framework/QwerkE_Framework/Entities/GameObject.h"
#include "../QwerkE_Framework/QwerkE_Framework/Entities/Components/Camera/CameraComponent.h"
#include "../QwerkE_Framework/QwerkE_Common/Libraries/glew/GL/glew.h"
#include "../QwerkE_Framework/QwerkE_Framework/Scenes/Scene.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/SceneManager.h"
#include "../QwerkE_Framework/QwerkE_Framework/Scenes/ViewerScene.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/ServiceLocator.h"
#include "../QwerkE_Framework/QwerkE_Framework/Systems/Factory/Factory.h"

#include <string>

ResourceViewer::ResourceViewer()
{
	m_ResourceManager = (ResourceManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Resource_Manager);
	m_Textures = m_ResourceManager->LookAtTextures();
	m_Shaders = m_ResourceManager->LookAtShaderProgram();
	m_Meshes = m_ResourceManager->LookAtMeshes();
	m_FBO = new FrameBufferObject();
	m_FBO->Init();

	m_ViewerScene = new ViewerScene();
	m_Subject = ((Factory*)QwerkE::ServiceLocator::GetService(eEngineServices::Factory_Entity))->CreateTestModel(m_ViewerScene, vec3(0, -4, 40));
	m_TagPlane = ((Factory*)QwerkE::ServiceLocator::GetService(eEngineServices::Factory_Entity))->CreatePlane(m_ViewerScene, vec3(2, -2, 10));
	m_TagPlane->SetRotation(vec3(90,0,0));
	m_TagPlane->SetScale(vec3(0.3f, 0.3f, 0.3f));

	m_ViewerScene->AddObjectToScene(m_Subject);
	m_ViewerScene->AddObjectToScene(m_TagPlane);

	m_ViewerScene->Initialize();
	m_ViewerScene->SetIsEnabled(true);
	((CameraComponent*)m_ViewerScene->GetCameraList().at(0)->GetComponent(Component_Camera))->SetViewportSize(vec2(1, 1));

	((SceneManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Scene_Manager))->AddScene(m_ViewerScene);

	DrawModelThumbnails();
}

ResourceViewer::~ResourceViewer()
{
	delete m_FBO;
}

void ResourceViewer::Draw()
{
	if(ImGui::Begin("ResourceViewer"))
	{
		if (ImGui::Button("Refresh"))
		{
			DrawModelThumbnails();
		}
		ImGui::SameLine();

		// select what resource to view
		if(ImGui::Button("Images"))
			m_CurrentResource = 0;
		ImGui::SameLine();
		if (ImGui::Button("Models"))
			m_CurrentResource = 1;
		ImGui::SameLine();
		if (ImGui::Button("Shaders"))
			m_CurrentResource = 2;
		ImGui::SameLine();
		if (ImGui::Button("Fonts"))
			m_CurrentResource = 3;
		ImGui::SameLine();
		if (ImGui::Button("Levels"))
			m_CurrentResource = 4;
		ImGui::SameLine();
		if (ImGui::Button("Sounds"))
			m_CurrentResource = 5;

		// draw list of resources
		static ImTextureID nullImage = ImTextureID(1);
		ImVec2 winSize = ImGui::GetWindowSize();
		m_ItemsPerRow = winSize.x / (m_ImageSize.x * 1.5f) + 1; // (* up the image size for feel), + avoid dividing by 0
		unsigned int counter = 0;
		ImGui::Separator();
		// TODO: Consider using imgui groups for easier hover support
		switch (m_CurrentResource)
		{
		case 0:
			// draw texture thumbnails
			for (const auto &p : *m_Textures)
			{
				if (counter % m_ItemsPerRow)
					ImGui::SameLine();

				ImGui::ImageButton((ImTextureID)p.second, m_ImageSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), 1);
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					// image name or something might be better. use newly create asset tags
					ImGui::Text(std::to_string(p.second).c_str());
					ImGui::Text("TagName");
					ImGui::EndTooltip();
				}
				counter++;
			}
			break;
		case 1:
			if (counter % m_ItemsPerRow)
				ImGui::SameLine();

			for (int i = 0; i < m_ModelImageHandles.size(); i++)
			{
				if (counter % m_ItemsPerRow)
					ImGui::SameLine();

				ImGui::ImageButton((ImTextureID)m_ModelImageHandles.at(i), m_ImageSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), 1);
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					// image name or something might be better. use newly create asset tags
					ImGui::Text(std::to_string(m_ModelImageHandles[0]).c_str());
					ImGui::EndTooltip();
				}
				counter++;
			}
			break;
		}

		ImGui::End();
	}
}

void ResourceViewer::DrawModelThumbnails()
{
	// dump old values. maybe calculate what changed in the future
	m_ModelImageHandles.clear();

	for (const auto &p : *m_Meshes)
	{
		m_FBO->Bind();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// ((RenderComponent*)m_Subject->GetComponent(Component_Render))->SetMesh(m_ResourceManager->GetMesh("Circle"));

		// TODO: RenderRoutine needs to update its uniform functions properly
		//((RenderComponent*)m_Subject->GetComponent(Component_Render))->SetModel(p.second);
		//((RenderComponent*)m_TagPlane->GetComponent(Component_Render))->SetColour(vec4(128, 128, 128, 255)); // TODO: use model asset tag color

		// draw scene
		m_ViewerScene->Draw();

		GLuint tempTexture;
		glGenTextures(1, &tempTexture);
		glBindTexture(GL_TEXTURE_2D, tempTexture);
		// glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1280, 720, 0, 0, 1);
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, 1280, 720);

		m_FBO->UnBind();

		// m_ImageHandles.push_back(CopyFBOToTexture(*m_FBO, 1280, 720));
		// m_ModelImageHandles.push_back(tempTexture);
	}
	m_ModelImageHandles.push_back(m_FBO->GetTextureID());
}