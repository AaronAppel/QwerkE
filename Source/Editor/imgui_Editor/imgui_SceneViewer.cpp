#include "../SceneViewer.h"

#include "../QwerkE_Framework/Libraries/imgui/imgui.h"

#include "../QwerkE_Framework/Source/Utilities/StringHelpers.h"
#include "../QwerkE_Framework/Source/FileSystem/FileIO/FileUtilities.h"
#include "../QwerkE_Framework/Source/Core/Input/Input.h"
#include "../QwerkE_Framework/Source/Core/Graphics/Renderer.h"
#include "../QwerkE_Framework/Source/Core/Scenes/Scene.h"
#include "../QwerkE_Framework/Source/Core/Scenes/Scenes.h"
#include "../QwerkE_Framework/Source/Core/Graphics/DataTypes/FrameBufferObject.h"
#include "../QwerkE_Framework/Source/Core/Window/Window.h"
#include "../QwerkE_Framework/Source/Core/Window/Windows.h"

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
        if (ImGui::Begin("Scene Viewer", &isOpen, ImGuiWindowFlags_NoScrollbar))
        {
            Scene* currentScene = Scenes::GetCurrentScene();

            m_currentEngineStateIndex = (char)currentScene->GetState();
            const char* states[] = { "Running", "Frozen", "Paused", "SlowMo", "Animating" }; // #TODO Implement engine states

            ImGui::PushItemWidth(150);
            if (ImGui::Combo("Scene State", &m_currentEngineStateIndex, states, sizeof(states)))
            {
                currentScene->SetState((eSceneState)m_currentEngineStateIndex);
            }
            ImGui::PopItemWidth();

            ImGui::SameLine();
            if (ImGui::Button("Save")) currentScene->SaveScene();

            ImGui::SameLine();
            if (ImGui::Button("Reload")) currentScene->ReloadScene();

            m_FBO->Bind();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Scenes::DrawCurrentScene();
            m_FBO->UnBind();

            const ImVec2 winSize = ImGui::GetWindowSize();

            ImVec2 imageSize = winSize;
            imageSize.x += winSize.x * 7.63f; // scale the width larger for upcoming divisions so window fits
            imageSize = ImVec2(imageSize.x / 9, imageSize.x / 16); // 16 x 9 resolution

            // const float scalar = 0.95f;
            // ImVec2 imageSize = ImVec2(winSize.x * scalar, winSize.y * scalar);

            // const vec2& aspectRatio = Windows::GetLastFocusedWindow()->GetAspectRatio();
            // imageSize = ImVec2(imageSize.x / aspectRatio.x, imageSize.y / aspectRatio.y);

            const float offset = 60.f;
            ImGui::SetWindowSize(ImVec2(winSize.x, imageSize.y + offset));

            ImGui::Image(ImTextureID(m_FBO->GetTextureID()), imageSize, ImVec2(0, 1), ImVec2(1, 0));
        }

        ImGui::End(); // #TODO Review if End() should be called when Begin() returns false
    }

    void SceneViewer::DrawSceneList()
    {
        if (ImGui::Begin("Scene List"))
        {
            const ImVec2 imageSize = ImVec2(100.0f, 100.0f);
            const float scalar = 1.5f;
            const int m_ItemsPerRow = 1 + (int)(ImGui::GetWindowSize().x / (imageSize.x * scalar));

            int counter = 1;
            for (const auto& p : *Scenes::LookAtScenes())
            {
                if (counter % m_ItemsPerRow)
                {
                    ImGui::SameLine();
                }

                if (ImGui::Button(DispStrCombine(std::to_string(counter).c_str(), std::to_string(p.second->GetSceneID()).c_str()).c_str()) || Input::FrameKeyAction((eKeys)(eKeys::eKeys_0 + counter), eKeyState::eKeyState_Press))
                {
                    Scenes::SetCurrentScene(p.second->GetSceneID());
                }
                counter++;
            }
        }

        ImGui::End();
    }

}
