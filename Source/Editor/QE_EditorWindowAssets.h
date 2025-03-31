#pragma once

#ifdef _QBGFXFRAMEWORK
namespace bgfxFramework { struct Mesh; }
#endif

#include "QF_Assets.h"
#include "QF_Mesh.h"
#include "QF_Shader.h"

#include "QE_Editor.h"
#include "QE_EditorWindow.h"
#include "QE_Settings.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowAssets : public EditorWindow
		{
		public:
            EditorWindowAssets(GUID guid = GUID()) : EditorWindow("Assets", EditorWindowTypes::Assets, guid) { }

		private:
			void DrawInternal() override
			{
                static bool viewLoadedAssets = true;

                if (viewLoadedAssets)
                {
                    ImGui::Text("Loaded Assets");
                    ImGui::SameLine();
                    if (ImGui::Button("View Registry"))
                    {
                        viewLoadedAssets = false;
                    }

                    // #TODO Consider a button to unload an asset from RAM

                    if (ImGui::CollapsingHeader("bgfxFramework::Mesh"))
                    {
                        if (const std::unordered_map<GUID, bgfxFramework::Mesh*>* meshes = Assets::ViewAssets<bgfxFramework::Mesh>())
                        {
                            for (auto& guidMeshPair : *meshes)
                            {
                                ImGui::Text("GUID: ");
                                ImGui::SameLine();
                                ImGui::Text(std::to_string(guidMeshPair.first).c_str());
                            }
                        }
                    }

                    if (ImGui::CollapsingHeader("QwerkE::Mesh"))
                    {
                        if (const std::unordered_map<GUID, Mesh*>* meshes = Assets::ViewAssets<Mesh>())
                        {
                            for (auto& guidMeshPair : *meshes)
                            {
                                ImGui::Text("GUID: ");
                                ImGui::SameLine();
                                ImGui::Text(std::to_string(guidMeshPair.first).c_str());
                            }
                        }
                    }

                    if (ImGui::CollapsingHeader("Shaders"))
                    {
                        if (const std::unordered_map<GUID, Shader*>* shaders = Assets::ViewAssets<Shader>())
                        {
                            for (auto& guidShaderPair : *shaders)
                            {
                                ImGui::Text("GUID: ");
                                ImGui::SameLine();
                                ImGui::Text(std::to_string(guidShaderPair.first).c_str());
                            }
                        }
                    }

                    if (ImGui::CollapsingHeader("Scenes"))
                    {
                        if (const std::unordered_map<GUID, Scene*>* scenes = Assets::ViewAssets<Scene>())
                        {
                            for (auto& guidScenePair : *scenes)
                            {
                                ImGui::Text("GUID: ");
                                ImGui::SameLine();
                                ImGui::Text(std::to_string(guidScenePair.first).c_str());
                                ImGui::SameLine();
                                ImGui::Text(guidScenePair.second->GetSceneName().c_str());
                            }
                        }

                        // #NOTE Scene transition change
                        // std::vector<Scene*> scenes = Scenes::LookAtScenes();
                        // for (size_t i = 0; i < scenes.size(); i++)
                        // {
                        //     ImGui::Text("GUID: ");
                        //     ImGui::SameLine();
                        //     ImGui::Text(std::to_string(scenes[i]->GetGuid()).c_str());
                        //     ImGui::SameLine();
                        //     ImGui::Text(scenes[i]->GetSceneName().c_str());
                        // }
                    }
                }
                else
                {
                    ImGui::Text("Assets Registry");
                    ImGui::SameLine();
                    if (ImGui::Button("View Loaded Assets"))
                    {
                        viewLoadedAssets = true;
                    }

                    if (ImGui::Button("Save Asset Registry"))
                    {
                        Assets::SaveRegistry();
                    }

                    // #TODO Consider a button to remove an asset entry from the registry

                    // auto materialAssetRegistry = Assets::ViewRegistry<Material>();
                    auto assetRegistry = Assets::ViewRegistry();

                    for (auto& pairMirrorTypesVector : assetRegistry)
                    {
                        size_t type = pairMirrorTypesVector.first;
                        for (size_t i = 0; i < pairMirrorTypesVector.second.size(); i++)
                        {
                            auto pairGuidString = pairMirrorTypesVector.second[i];
                            ImGui::Text("GUID: ");
                            ImGui::SameLine();
                            std::string guidString = std::to_string(pairGuidString.first).c_str();
                            ImGui::Text(guidString.c_str());

                            std::string popUpName = "Context GUID" + guidString;
                            if (ImGui::IsItemClicked(ImGui::MouseRight))
                            {
                                ImGui::OpenPopup(popUpName.c_str());
                            }

                            if (ImGui::BeginPopup(popUpName.c_str()))
                            {
                                if (ImGui::Button("Copy To Clipboard"))
                                {
                                    ImGui::SetClipboardText(guidString.c_str());
                                    LOG_INFO("{0} Copied to clipboard: {1}", __FUNCTION__, guidString.c_str());
                                }
                                ImGui::EndPopup();
                            }

                            ImGui::SameLine();
                            ImGui::Text(" File: ");
                            ImGui::SameLine();
                            // #TODO Decide how to search for shader and materials that have more than 1 string in vector
                            ImGui::Text(pairGuidString.second[0].c_str());
                        }
                    }
                }
			}

            bool m_AutoSaveRegistryOnFileDrop = true;
		};

	}

}