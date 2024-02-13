#include "QE_SceneViewer.h"

#include "imgui/imgui.h"

#include "QC_StringHelpers.h"

#include "QF_FileUtilities.h"
#include "QF_Input.h"
#include "QF_Renderer.h"
#include "QF_Scene.h"
#include "QF_Scenes.h"
#include "QF_FrameBufferObject.h"
#include "QF_Window.h"

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
    }

    void SceneViewer::DrawSceneView()
    {
        static bool isOpen = true;
        if (ImGui::Begin("Scene Viewer", &isOpen, ImGuiWindowFlags_NoScrollbar))
        {
            Scene* currentScene = Scenes::GetCurrentScene();

            ImGui::PushItemWidth(100);
            const char* runningStates[] = { "Running", "Paused" };
            m_currentSceneStateIndex = (char)currentScene->GetIsPaused();
            if (ImGui::Combo("Running", &m_currentSceneStateIndex, runningStates, sizeof(runningStates)/sizeof(const char*)))
            {
                currentScene->SetIsPaused((bool)m_currentSceneStateIndex);
            }
            ImGui::PopItemWidth();

            ImGui::SameLine();
            ImGui::PushItemWidth(100);
            const char* activeStates[] = { "Inactive", "Active" };
            int sceneIsActive = (char)currentScene->GetIsActive();
            if (ImGui::Combo("Active", &sceneIsActive, activeStates, sizeof(activeStates) / sizeof(const char*)))
            {
                currentScene->SetIsActive((bool)sceneIsActive);
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

            DrawSceneList();

            m_FBO->Bind();
            Renderer::ClearScreen();
            Scenes::DrawCurrentScene();
            m_FBO->UnBind();

            const ImVec2 winSize = ImGui::GetWindowSize();

            ImVec2 imageSize = winSize;
            const float scalar = 7.63f;
            imageSize.x += winSize.x * scalar;
            const vec2& aspectRatio = Window::GetAspectRatio();
            imageSize = ImVec2(imageSize.x / aspectRatio.y, imageSize.x / aspectRatio.x);

            const float offset = 60.f;
            ImGui::SetWindowSize(ImVec2(winSize.x, imageSize.y + offset));

            ImGui::Image(ImTextureID(m_FBO->GetTextureID()), imageSize, ImVec2(0, 1), ImVec2(1, 0));
        }

        ImGui::End();
    }

    const char* s_ScenesComboText = "Scene ";
    const int s_CharacterPixelSize = 10;
    void SceneViewer::DrawSceneList()
    {
        const std::vector<Scene*>& scenes = Scenes::LookAtScenes();
        std::vector<const char*> sceneNames;
        sceneNames.reserve(3);

        for (size_t i = 0; i < scenes.size(); i++)
        {
            sceneNames.push_back(scenes[i]->GetSceneName().c_str());
        }
        int index = Scenes::GetCurrentSceneIndex();

        const int sceneFileNameWidth = strlen(sceneNames[index]) * s_CharacterPixelSize;
        ImGui::PushItemWidth(sceneFileNameWidth);

        ImGui::SameLine(ImGui::GetWindowWidth() - sceneFileNameWidth - (strlen(s_ScenesComboText) * s_CharacterPixelSize));
        if (ImGui::Combo(s_ScenesComboText, &index, sceneNames.data(), scenes.size()))
        {
            Scenes::SetCurrentScene(index);
        }
        ImGui::PopItemWidth();
    }

}
