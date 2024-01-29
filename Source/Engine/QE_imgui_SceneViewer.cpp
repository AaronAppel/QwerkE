#include "QE_SceneViewer.h"

#include "imgui/imgui.h"

#include "QC_StringHelpers.h"

#include "QF_FileUtilities.h"
#include "QF_Input.h"
#include "QF_Scene.h"
#include "QF_Scenes.h"
#include "QF_FrameBufferObject.h"
#include "QF_Window.h"
#include "QF_Windows.h"

#include "QE_Engine.h"
#include "QE_Editor.h"
#include "QE_EntityEditor.h"

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

            m_currentEngineStateIndex = (char)currentScene->GetIsPaused();

            ImGui::PushItemWidth(150);
            const char* states[] = { "Running", "Paused" };
            if (ImGui::Combo("Scene State", &m_currentEngineStateIndex, states, sizeof(states)/sizeof(const char*)))
            {
                currentScene->SetIsPaused((bool)m_currentEngineStateIndex);
            }
            ImGui::PopItemWidth();

            ImGui::SameLine();
            if (ImGui::Button("Save")) currentScene->SaveScene();

            ImGui::SameLine();
            if (ImGui::Button("Reload"))
            {
                Editor::GetEntityEditor()->SetCurrentEntity(nullptr);
                currentScene->ReloadScene();
            }

            m_FBO->Bind();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Scenes::DrawCurrentScene();
            m_FBO->UnBind();

            const ImVec2 winSize = ImGui::GetWindowSize();

            ImVec2 imageSize = winSize;
            const float scalar = 7.63f;
            imageSize.x += winSize.x * scalar;
            const vec2& aspectRatio = Windows::GetWindow(0)->GetAspectRatio();
            imageSize = ImVec2(imageSize.x / aspectRatio.y, imageSize.x / aspectRatio.x);

            const float offset = 60.f;
            ImGui::SetWindowSize(ImVec2(winSize.x, imageSize.y + offset));

            ImGui::Image(ImTextureID(m_FBO->GetTextureID()), imageSize, ImVec2(0, 1), ImVec2(1, 0));
        }

        ImGui::End();
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

                if (ImGui::Button(p.second->GetSceneFileName().c_str()) ||
                    Input::FrameKeyAction((eKeys)(eKeys::eKeys_0 + counter), eKeyState::eKeyState_Press))
                {
                    Scenes::SetCurrentScene(p.second->GetSceneID());
                }
                counter++;
            }
        }

        ImGui::End();
    }

}
