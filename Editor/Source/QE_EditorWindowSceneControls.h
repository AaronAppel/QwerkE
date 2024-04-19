#pragma once

#include "QC_StringHelpers.h"

#include "QF_PathDefines.h"
#include "QF_Scene.h"
#include "QF_Scenes.h"

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowSceneControls : public EditorWindow
		{
		public:
			EditorWindowSceneControls::EditorWindowSceneControls(GUID guid = GUID()) : EditorWindow("Scene Controls", EditorWindowTypes::SceneControls, guid) { }

		private:

			void DrawInternal() override
			{
                // #TODO Current scene from Scenes:: works, but maybe more than 1 scene can be active.
                // Might need to connect scene controls to a scene viewer, or combine windows
                Scene* currentScene = Scenes::GetCurrentScene();
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
                    Editor::ResetEditorWindowReferences();
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

                ImGui::SameLine();
                if (ImGui::Button("-"))
                {
                    Scenes::DestroyScene(currentScene);
                    Editor::ResetEditorWindowReferences();
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
                    if (!scenes.empty())
                    {
                        std::vector<const char*> sceneNames;
                        sceneNames.reserve(3);

                        for (size_t i = 0; i < scenes.size(); i++)
                        {
                            sceneNames.push_back(scenes[i]->GetSceneName().c_str());
                        }
                        int index = Scenes::GetCurrentSceneIndex();

                        const u32 sceneFileNameWidth = (u32)strlen(sceneNames[index]) * s_CharacterPixelSize;

                        ImGui::PushItemWidth((float)sceneFileNameWidth + (float)s_DropDownArrowSize);

                        char s_ScenesCombobuffer[] = "Scenes:    ";
                        snprintf(s_ScenesCombobuffer, strlen(s_ScenesCombobuffer), "Scenes: %i", (int)sceneNames.size());

                        // #TODO Use ImGui::SameLineEnd();
                        ImGui::SameLine(ImGui::GetWindowWidth() - sceneFileNameWidth - (strlen(s_ScenesCombobuffer) * s_CharacterPixelSize));
                        if (ImGui::Combo(s_ScenesCombobuffer, &index, sceneNames.data(), (s32)scenes.size()))
                        {
                            Scenes::SetCurrentScene(index);
                        }
                        ImGui::PopItemWidth();
                    }
                    else
                    {
                        ImGui::SameLineEnd("Scenes : 0");
                        ImGui::Text("Scenes : 0");
                    }
                }
			}
		};

	}

}