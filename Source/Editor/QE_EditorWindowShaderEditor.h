#pragma once

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
#endif

#include "QF_Buffer.h"
#include "QF_Directory.h"
#include "QF_Files.h"
#include "QF_Paths.h"

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowShaderEditor : public EditorWindow
		{
		public:
			EditorWindowShaderEditor(GUID guid = GUID()) :
				EditorWindow("Shader Editor", EditorWindowTypes::ShaderEditor, guid)
			{
				m_FileBuffer.Allocate(m_StartingBufferSize);
				m_FileBuffer.Fill('\0');
			}

		private:
			void DirectoryPathSetterButton(std::filesystem::path& path)
			{
				ImGui::SetNextItemWidth(10.f);
				if (ImGui::Button(":##ShaderEditor")) // #TODO Add window guid
				{
					path = QwerkE::Files::ExplorerOpen("All files (*.*)\0*.*\0", path.string().c_str());
					if (path.has_parent_path())
					{
						path = path.parent_path(); // #TODO Can't select directories, so return parent path of selected file
					}
				}
			}

			void DrawInternal() override
			{
				// #TODO Path validation
				ImGui::Text("Shader file path:");

				DirectoryPathSetterButton(m_CurrentShaderFilePath);
				if (ImGui::IsAtStartOfNewLine())
				{
					ImGui::SameLine(0, 3.f); // #NOTE Reduce space between buttons
				}
				const std::string result = ImGui::DirectoryPathAsSameLineButtons(m_CurrentShaderFilePath.string());
				if (!result.empty())
				{
					m_CurrentShaderFilePath = result;
				}

				if (ImGui::Button("Load file from explorer"))
				{
					std::string returnValue = "";
					const char* shaderFilesFilter =
						"All files (*.*)\0*.*\0"
						"Vertex file (*.vert)\0*.vert\0"
						"Fragment file (*.frag)\0*.frag\0";

					if (Directory::Exists(m_CurrentShaderFilePath.string().c_str()))
					{
						returnValue = Files::ExplorerOpen(shaderFilesFilter, m_CurrentShaderFilePath.string().c_str());
					}
					else if (Files::Exists(m_CurrentShaderFilePath.string().c_str()))
					{
						returnValue = Files::ExplorerOpen(shaderFilesFilter, m_CurrentShaderFilePath.parent_path().string().c_str());
					}
					else
					{
						returnValue = Files::ExplorerOpen(shaderFilesFilter, Paths::ShadersDir().c_str());
					}

					if (m_LatestFilePathExists = Files::Exists(returnValue.c_str()))
					{
						m_CurrentShaderFilePath = returnValue;
						LoadFile(m_CurrentShaderFilePath);
					}
				}

				ImGui::SameLineIfSpace(150.f);
				if (ImGui::Button("Save"))
				{
					// #TODO Save over existing file data
					LOG_WARN("Unimplemented save button");
				}
				ImGui::SameLineIfSpace(150.f);
				if (ImGui::Button("Save As"))
				{
					// #TODO Save using file explorer
					LOG_WARN("Unimplemented save as button");
				}
				ImGui::SameLineIfSpace(150.f);
				if (ImGui::Button("Recompile binary"))
				{
					// #TODO Recompile binary
					LOG_WARN("Unimplemented recompile button");
				}
				ImGui::SameLineIfSpace(150.f);
				if (ImGui::Button("Reload shader program"))
				{
					// #TODO Reload Assets shader program using (presumably newly generated) shader binaries
					LOG_WARN("Unimplemented reload button");
				}

				if (m_LatestFilePathExists)
				{
					ImGui::SameLineIfSpace(150.f);
					if (ImGui::Button("Reload file from disk"))
					{
						LoadFile(m_CurrentShaderFilePath);
					}
				}

				if (m_FileContentsDirty)
				{
					// #TODO Look at ImGui example to mark tab or window as "*" unchanged settings state
				}

				if (ImGui::InputTextMultiline("##ShaderFileTextBuffer", m_FileBuffer.As<char>(), m_FileBuffer.SizeInBytes(), ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_AllowTabInput))
				{
					m_FileContentsDirty = true;
					// #TODO Look at ImGui example to mark tab or window as "*" unchanged settings state
				}
			}

			void LoadFile(const Path& shaderFilePath)
			{
				if (Files::Exists(shaderFilePath.string().c_str()))
				{
					Buffer result = Files::LoadFile(shaderFilePath.string().c_str());
					if (result.SizeInBytes() > 0)
					{
						m_FileBuffer.Write(result);
					}
				}
			}

			MIRROR_PRIVATE_MEMBERS

			const u16 m_StartingBufferSize = 10000;
			Path m_CurrentShaderFilePath = Paths::ShadersDir();
			Buffer m_FileBuffer;
			bool m_LatestFilePathExists = false;
			bool m_FileContentsDirty = false;
		};

	}

}