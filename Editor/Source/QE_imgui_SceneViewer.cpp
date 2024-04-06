#include "QE_SceneViewer.h"

#ifdef _QDEARIMGUI
#include "imgui/imgui.h"
#endif

#include "QC_StringHelpers.h"

#include "QF_Files.h"
#include "QF_Input.h"
#include "QF_Scene.h"
#include "QF_Scenes.h"
#include "QF_Window.h"

#include "QE_Editor.h"
#include "QE_EntityEditor.h"

#include "QF_ComponentMesh.h"
#include "QF_ComponentTransform.h"

namespace QwerkE {

    void SceneViewer::Draw()
    {
        DrawSceneView();
    }

    void SceneViewer::DrawSceneView()
    {
        static bool isOpen = true;
        static bool isSceneTextureOpen = true;

        Scene* currentScene = Scenes::GetCurrentScene();
        if (!currentScene)
            return;

#ifdef _QDEARIMGUI
        if (ImGui::Begin("Scene Viewer", &isOpen, ImGuiWindowFlags_NoScrollbar))
        {
            // ImGui::GetWindowSize();

            // ImGui::PushItemWidth(100);
            // const char* runningStates[] = { "Running", "Paused" };
            // m_currentSceneStateIndex = (char)currentScene->GetIsPaused();
            // if (ImGui::Combo("", &m_currentSceneStateIndex, runningStates, sizeof(runningStates)/sizeof(const char*)))
            // {
            //     currentScene->SetIsPaused((bool)m_currentSceneStateIndex);
            // }
            // ImGui::PopItemWidth();
            // ImGui::SameLine();

            // #TODO Color
            bool sceneWasDirty = currentScene->IsDirty();
            if (sceneWasDirty)
            {
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.f, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1.f, 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1.f, 0.6f, 0.6f));
            }
            if (ImGui::Button("Save")) currentScene->SaveScene();
            if (sceneWasDirty)
            {
                ImGui::PopStyleColor(3);
            }

            ImGui::SameLine();
            if (ImGui::Button("Reload"))
            {
                Editor::OnReset();
                currentScene->ReloadScene();
            }

            ImGui::SameLine();
            if (ImGui::Button("+"))
            {
                // #TODO Ask Scenes:: for a new scene and remove ability to
                // create Scene objects externally.
                constexpr const char* newFileDefaultName = "NewScene";
                constexpr const char* emptyScenePrefabFileName = "Empty.qscene";

                // #NOTE Previous function UniqueFileNameNumberAppend(ScenesFolderPath(""), newFileDefaultName, scene_ext);
                uPtr<char[]> newFilePath = Files::UniqueFileName(Paths::Scene(StringAppend(newFileDefaultName, ".", scene_ext)).c_str());

                Scene* newScene = Scenes::CreateSceneFromFile(newFilePath.get());
                newScene->LoadSceneFromFilePath(emptyScenePrefabFileName);
                newScene->OnLoaded();
                Scenes::SetCurrentScene(newFilePath.get());
            }

            ImGui::SameLine(ImGui::GetWindowWidth() * 0.5f - 20.f);
            const char* buttonText = currentScene->GetIsPaused() ? ">" : "||";
            if (ImGui::Button(buttonText))
            {
                currentScene->SetIsPaused(!currentScene->GetIsPaused());
            }

            if (currentScene)
            {
                static bool isOpen = false;
                ImGui::Begin("MeshPositionWindow", &isOpen);

                auto viewTransforms = currentScene->ViewComponents<ComponentTransform>();
                int i = 0;
                for (auto entity : viewTransforms)
                {
                    ComponentTransform& transform = viewTransforms.get<ComponentTransform>(entity);

                    vec3f meshPosition = transform.GetPosition();

                    std::string meshName = "MeshPosition";
                    if (ImGui::DragFloat3((meshName + std::to_string(i)).c_str(), &meshPosition.x, .1f))
                    {
                        transform.SetPosition(meshPosition);
                    }

                    // mesh.Draw();
                    ++i;
                }

                ImGui::End();
            }

            DrawSceneList();
        }
        ImGui::End();

        if (ImGui::Begin("Scene Texture", &isSceneTextureOpen, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar))
        {
            ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

            // #TODO Replace hard coded texture ID
            ImGui::Image(ImTextureID(4), ImVec2(contentRegionAvailable.x, contentRegionAvailable.y), ImVec2(0, 1), ImVec2(1, 0));
        }
        ImGui::End();
#endif
    }

    const int s_CharacterPixelSize = 10;
    const int s_DropDownArrowSize = 20;
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
#ifdef _QDEARIMGUI
        ImGui::PushItemWidth((float)sceneFileNameWidth + s_DropDownArrowSize);

        char s_ScenesCombobuffer [] = "Scenes:    ";
        snprintf(s_ScenesCombobuffer, strlen(s_ScenesCombobuffer), "Scenes: %i", sceneNames.size());

        ImGui::SameLine(ImGui::GetWindowWidth() - sceneFileNameWidth - (strlen(s_ScenesCombobuffer) * s_CharacterPixelSize));
        if (ImGui::Combo(s_ScenesCombobuffer, &index, sceneNames.data(), scenes.size()))
        {
            Scenes::SetCurrentScene(index);
        }
        ImGui::PopItemWidth();
#endif
    }

}
