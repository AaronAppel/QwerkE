#pragma once

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
#endif

#include "QF_Assets.h"
#include "QF_Mesh.h"
#include "QF_Shader.h"
#include "QF_System.h"
#include "QF_Texture.h"

#include "QE_Editor.h"
#include "QE_EditorWindow.h"
#include "QE_Paths.h"
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
                // #TODO Consider ImGui:: child tabs
                if (m_ViewingLoadedAssets)
                {
                    ViewLoadedAssets();
                }
                else
                {
                    ViewAssetsRegistry();
                }
			}

            void ViewLoadedAssets()
            {
                ImGui::Text("Loaded Assets");
                ImGui::SameLine();
                if (ImGui::Button("View Registry"))
                {
                    m_ViewingLoadedAssets = false;
                }

                ImGui::SameLine();
                if (ImGui::Button("Save Asset Registry"))
                {
                    Assets::SaveRegistry();
                }

                // #TODO Consider a button to unload an asset from RAM

                const float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
                const float buttonWidth = ImGui::GetContentRegionAvail().x - lineHeight * 0.5f;

                if (ImGui::CollapsingHeader("QwerkE::Mesh"), ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen)
                {
                    if (ImGui::SmallButton("Recompile All Meshes"))
                    {
                        System::StartProcess(Paths::Script("CompileMeshes.bat")); // #TODO move hard coded file name somewhere better
                    }

                    if (const std::unordered_map<GUID, Mesh*>* meshes = Assets::ViewAssets<Mesh>())
                    {
                        for (auto& guidMeshPair : *meshes)
                        {
                            ImGui::Text("GUID: ");
                            ImGui::SameLine();
                            ImGui::Text(std::to_string(guidMeshPair.first).c_str());
                            ImGui::SameLine();
                            ImGui::Text(std::to_string(guidMeshPair.second->m_vbh.idx).c_str());
                            ImGui::SameLine();
                            ImGui::Text(std::to_string(guidMeshPair.second->m_vbh.idx).c_str());
                            // #TODO Recompile and/or reload a single mesh
                        }
                    }
                }

                if (ImGui::CollapsingHeader("Shaders"), ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen)
                {
                    // ImGui::SameLine(buttonWidth);
                    // if (ImGui::Button("+##Shader", { lineHeight, lineHeight }))
                    {
                        // #TODO Create a new shader. Maybe open in shader editor
                    }

                    if (ImGui::SmallButton("Recompile All Shaders"))
                    {
                        System::StartProcess(Paths::Script("CompileShaders.bat")); // #TODO move hard coded file name somewhere better
                    }

                    if (const std::unordered_map<GUID, Shader*>* shaders = Assets::ViewAssets<Shader>())
                    {
                        for (auto& guidShaderPair : *shaders)
                        {
                            ImGui::Text("GUID: ");
                            ImGui::SameLine();
                            ImGui::Text(std::to_string(guidShaderPair.first).c_str());

                            // #TODO Draw names of all shader components
                            // std::vector<std::string>* shaderComponents = &guidShaderPair.second;
                            for (size_t i = 0; i < 1; i++)
                            {
                                ImGui::SameLine();
                                ImGui::Text(std::to_string(guidShaderPair.second->ProgramHandle().idx).c_str());
                            }
                            ImGui::SameLine();
                            if (ImGui::SmallButton("Reload"))
                            {
                                guidShaderPair.second->Reload();
                            }
                        }
                    }
                }

                static bool showImages = true;
                if (ImGui::CollapsingHeader("Textures"), ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen)
                {
                    if (const std::unordered_map<GUID, Texture*>* textures = Assets::ViewAssets<Texture>())
                    {
                        for (auto& guidTexturePair : *textures)
                        {
                            if (showImages)
                            {
                                const ImVec2 imageSize = { 50.f, 50.f };
                                ImGui::Image(ImTextureID(guidTexturePair.second->TextureHandle().idx), imageSize);
                                if (ImGui::IsItemHovered())
                                {
                                    ImGui::BeginTooltip();
                                    ImGui::Text("GUID: ");
                                    ImGui::SameLine();
                                    ImGui::Text(std::to_string(guidTexturePair.first).c_str());
                                    ImGui::SameLine();
                                    ImGui::Text(std::to_string(guidTexturePair.second->TextureHandle().idx).c_str());
                                    ImGui::EndTooltip();
                                }
                            }
                            ImGui::Text("GUID: ");
                            ImGui::SameLine();
                            ImGui::Text(std::to_string(guidTexturePair.first).c_str());

                            for (size_t i = 0; i < 1; i++)
                            {
                                ImGui::SameLine();
                                ImGui::Text(std::to_string(guidTexturePair.second->TextureHandle().idx).c_str());
                                // #TODO Option to load texture file to RAM via context menu
                            }
                        }
                    }
                }

                // #NOTE Using TreeNodeEx so buttons render in line with collapsible header
                const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
                const bool scenesTreeNodeOpen = ImGui::TreeNodeEx((void*)typeid(Scene).hash_code(), treeNodeFlags, "ScenesTree");

                ImGui::SameLine(buttonWidth - lineHeight * 1.5f);
                // #TODO Add guid to button labels
                if (ImGui::Button("+##Scene", ImVec2{ lineHeight, lineHeight }))
                {
                    Path newSceneFileName = Files::UniqueFilePath(Paths::Scene("NewScene.qscene").c_str()); // #TODO Use ".qscene" constant
                    Scenes::CreateScene(newSceneFileName.filename().string());
                }
                ImGui::SameLine(buttonWidth);
                if (ImGui::Button(":##Scene", { lineHeight, lineHeight }))
                {
                    // #TODO Delete scene
                }
                if (scenesTreeNodeOpen)
                {
                    ImGui::TreePop(); // Pop tree node before displaying content. Content not really in tree, only visually

                    if (ImGui::Button("-##Scene", { lineHeight, lineHeight }))
                    {
                        // #TODO Delete scene
                    }

                    if (const std::unordered_map<GUID, Scene*>* scenes = Assets::ViewAssets<Scene>())
                    {
                        for (auto& guidScenePair : *scenes)
                        {
                            ImGui::Text("GUID:");
                            ImGui::SameLine();
                            ImGui::Text(std::to_string(guidScenePair.first).c_str());
                            ImGui::SameLine();
                            ImGui::Text(guidScenePair.second->GetSceneName().c_str());
                        }
                    }
                }
            }

            void ViewAssetsRegistry()
            {
                ImGui::Text("Assets Registry");
                ImGui::SameLine();
                if (ImGui::Button("View Loaded Assets"))
                {
                    m_ViewingLoadedAssets = true;
                }

                ImGui::SameLine();
                if (ImGui::Button("Save Asset Registry"))
                {
                    Assets::SaveRegistry();
                }

                // #TODO Consider a button to remove an asset entry from the registry

                // auto materialAssetRegistry = Assets::ViewRegistry<Material>();
                auto assetRegistry = Assets::ViewRegistry();

                for (auto& pairMirrorTypesVector : assetRegistry)
                {
                    std::string message = "Type: ";
                    message += std::to_string(pairMirrorTypesVector.first);
                    ImGui::Text(message.c_str());

                    for (size_t i = 0; i < pairMirrorTypesVector.second.size(); i++)
                    {
                        auto pairGuidString = pairMirrorTypesVector.second[i];

                        if (Assets::Has(pairMirrorTypesVector.first, pairGuidString.first))
                        {
                            ImGui::Text("L"); // #TODO Is loaded into ram or not. Add a button to load
                        }
                        else
                        {
                            ImGui::Text("X"); // #TODO Is loaded into ram or not. Add a button to load
                            if (ImGui::IsItemClicked(ImGui::MouseRight) ||
                                ImGui::IsItemClicked(ImGui::MouseLeft))
                            {
                                Assets::Load(pairMirrorTypesVector.first, pairGuidString.first);
                            }
                        }
                        ImGui::SameLine();
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
                    ImGui::Dummy({ 0.f, 5.f });
                }
            }

            MIRROR_PRIVATE_MEMBERS

            // #TODO bool m_AutoSaveRegistryOnFileDrop = true;
            bool m_ViewingLoadedAssets = true;
		};

	}

}