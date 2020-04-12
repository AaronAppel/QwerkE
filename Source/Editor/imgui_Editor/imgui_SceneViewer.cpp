#include "../SceneViewer.h"

#include "../QwerkE_Framework/Libraries/imgui/imgui.h"
#include "../QwerkE_Framework/Source/Utilities/StringHelpers.h"
#include "../QwerkE_Framework/Source/FileSystem/FileIO/FileUtilities.h"
#include "../QwerkE_Framework/Source/Core/Input/Input.h"
#include "../QwerkE_Framework/Source/Core/Graphics/Renderer.h"
#include "../QwerkE_Framework/Source/Core/Scenes/Scene.h"
#include "../QwerkE_Framework/Source/Core/Graphics/DataTypes/FrameBufferObject.h"
#include "../QwerkE_Framework/Source/Core/Scenes/Scene.h"

namespace QwerkE {

    SceneViewer::SceneViewer()
    {
        m_FBO = new FrameBufferObject();
        m_FBO->Init();
    }

    SceneViewer::~SceneViewer()
    {
        delete m_FBO;
    }

    void SceneViewer::NewFrame()
    {
    }

    void SceneViewer::Update()
    {
    }

    void SceneViewer::Draw()
    {
        DrawSceneView();
        DrawSceneList();
    }

    void SceneViewer::DrawSceneView()
    {
        static bool isOpen = true;
        if (ImGui::Begin("Scene Window - Framework.cpp", &isOpen, ImGuiWindowFlags_NoScrollbar))
        {
            Scene* currentScene = Scenes::GetCurrentScene();

            // save/load + state
            static int selection = 0;
            selection = (char)currentScene->GetState();
            const char* states[] = { "Running", "Frozen", "Paused", "SlowMo", "Animating" };
            ImGui::PushItemWidth(150);
            if (ImGui::Combo("Scene State", &selection, states, 5))
            {
                currentScene->SetState((eSceneState)selection);
            }
            ImGui::SameLine();
            ImGui::PopItemWidth();
            ImGui::SameLine();
            if (ImGui::Button("Save")) currentScene->SaveScene();
            ImGui::SameLine();
            if (ImGui::Button("Reload")) currentScene->ReloadScene();

            // Render scene to FBO
            m_FBO->Bind();
            // Renderer::NewFrame();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Scenes::DrawCurrentScene();
            m_FBO->UnBind();

            ImVec2 winSize = ImGui::GetWindowSize();

            ImVec2 imageSize = winSize;

            // TODO: Fix image width. A larger panel has more space on the right.
            // TODO: Consider centering image on panel.
            imageSize.x += winSize.x * 7.63f; // scale the width larger for upcoming divisions so window fits
            imageSize = ImVec2(imageSize.x / 9, imageSize.x / 16); // 16 x 9 resolution

            ImGui::SetWindowSize(ImVec2(winSize.x, imageSize.y + 60)); // snap window height to scale

            // render texture as image
            ImGui::Image(ImTextureID(m_FBO->GetTextureID()), imageSize, ImVec2(0, 1), ImVec2(1, 0));

            ImGui::End();
        }
        else
            ImGui::End();
    }

    void SceneViewer::DrawSceneList()
    {
        ImGui::Begin("Scene List");

        // TODO: Think of adding multi window support for viewing more than 1
        // enabled scene at a time. Going to have to look at how input would
        // work for that.

        int counter = 1;
        ImVec2 winSize = ImGui::GetWindowSize();
        ImVec2 imageSize = ImVec2(100.0f, 100.0f);
        int m_ItemsPerRow = (int)(winSize.x / (imageSize.x * 1.5f) + 1.0f); // (* up the image size for feel), + avoid dividing by 0

        for (const auto& p : *Scenes::LookAtScenes())
        {
            if (counter % m_ItemsPerRow)
                ImGui::SameLine();

            if (ImGui::Button(DispStrCombine(std::to_string(counter).c_str(), std::to_string(p.second->GetSceneID()).c_str()).c_str()) || Input::FrameKeyAction((eKeys)(eKeys::eKeys_0 + counter), eKeyState::eKeyState_Press))
            {
                Scenes::SetCurrentScene(p.second->GetSceneID());
            }
            counter++;
        }

        ImGui::End();
    }

}
