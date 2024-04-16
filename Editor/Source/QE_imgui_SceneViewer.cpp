#include "QE_SceneViewer.h"

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QC_imgui.h"
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
        static bool isOpen = true;
        static bool isSceneTextureOpen = true;

        Scene* currentScene = Scenes::GetCurrentScene();
        if (!currentScene)
            return;

#ifdef _QDEARIMGUI
        if (ImGui::Begin("Scene Controls", &isOpen, ImGuiWindowFlags_NoScrollbar))
        {
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
                const char* const newFileDefaultName = StringAppend("NewScene", ".", scene_ext); // #TODO constexpr
                constexpr const char* emptyScenePrefabFileName = "Empty.qscene";

                if (Scene* newScene = Scenes::CreateScene(newFileDefaultName, true))
                {
                    newScene->LoadSceneFromFilePath(Paths::Scene(emptyScenePrefabFileName).c_str());
                    newScene->OnLoaded();
                    Scenes::SetCurrentScene(newScene);
                }
            }

            const char* buttonText = currentScene->GetIsPaused() ? ">" : "||";
            ImGui::SameLineCentered(buttonText);
            if (ImGui::Button(buttonText))
            {
                currentScene->SetIsPaused(!currentScene->GetIsPaused());
            }

            {
                constexpr u32 s_CharacterPixelSize = 10;
                constexpr u32 s_DropDownArrowSize = 20;

                const std::vector<Scene*>& scenes = Scenes::LookAtScenes();
                std::vector<const char*> sceneNames;
                sceneNames.reserve(3);

                for (size_t i = 0; i < scenes.size(); i++)
                {
                    sceneNames.push_back(scenes[i]->GetSceneName().c_str());
                }
                int index = Scenes::GetCurrentSceneIndex();

                const u32 sceneFileNameWidth = strlen(sceneNames[index]) * s_CharacterPixelSize;
#ifdef _QDEARIMGUI
                ImGui::PushItemWidth((float)sceneFileNameWidth + (float)s_DropDownArrowSize);

                char s_ScenesCombobuffer[] = "Scenes:    ";
                snprintf(s_ScenesCombobuffer, strlen(s_ScenesCombobuffer), "Scenes: %i", (int)sceneNames.size());

                ImGui::SameLine(ImGui::GetWindowWidth() - sceneFileNameWidth - (strlen(s_ScenesCombobuffer) * s_CharacterPixelSize));
                if (ImGui::Combo(s_ScenesCombobuffer, &index, sceneNames.data(), (s32)scenes.size()))
                {
                    Scenes::SetCurrentScene(index);
                }
                ImGui::PopItemWidth();
#endif
            }
        }
        ImGui::End();

#endif
    }

}
