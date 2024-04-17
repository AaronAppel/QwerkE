#pragma once

#include "QF_Assets.h"
#include "QF_Mesh.h"
#include "QF_Settings.h"
#include "QF_Shader.h"

#include "QE_Editor.h"
#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowAssets : public EditorWindow
		{
		public:
            EditorWindowAssets() : EditorWindow("Assets") { }

		private:
			void DrawInternal() override
			{
                {
                    ImGui::Text("Meshes:");
                    const std::unordered_map<GUID, Mesh*>& meshes = Assets::ViewAssets<Mesh>();
                    for (auto& guidMeshPair : meshes)
                    {
                        Mesh* mesh = guidMeshPair.second;
                        ImGui::Text("GUID: ");
                        ImGui::SameLine();
                        ImGui::Text(std::to_string(mesh->m_GUID).c_str());
                    }
                }

                {
                    ImGui::Text("Shaders:");
                    const std::unordered_map<GUID, Shader*>& shaders = Assets::ViewAssets<Shader>();
                    for (auto& guidShaderPair : shaders)
                    {
                        Shader* shader = guidShaderPair.second;
                        ImGui::Text("GUID: ");
                        ImGui::SameLine();
                        ImGui::Text(std::to_string(shader->m_GUID).c_str());
                    }
                }

                {
                    ImGui::Text("Textures:");
                }

                {
                    ImGui::Text("Materials:");
                }

                {
                    ImGui::Text("Assets Registry:");
                    auto assetRegistry = Assets::ViewRegistry();
                    for (size_t i = 0; i < assetRegistry.size(); i++)
                    {
                        ImGui::Text("GUID: ");
                        ImGui::SameLine();
                        ImGui::Text(std::to_string(assetRegistry[i].first).c_str());
                        ImGui::SameLine();
                        ImGui::Text(" File: ");
                        ImGui::SameLine();
                        ImGui::Text(assetRegistry[i].second.c_str());
                    }
                }
			}
		};

	}

}