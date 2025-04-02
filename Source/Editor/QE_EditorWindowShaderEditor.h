#pragma once

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
			EditorWindowShaderEditor::EditorWindowShaderEditor(GUID guid = GUID()) :
				EditorWindow("Shader Editor", EditorWindowTypes::ShaderEditor, guid)
			{
				m_FileBuffer.Allocate(m_StartingBufferSize);
				m_FileBuffer.Fill('\0');
			}

		private:
			void DrawInternal() override
			{
				// #TODO Path validation
				ImGui::Text("Shader file path:");
				ImGui::SameLine();
				ImGui::Text(m_CurrentShaderFilePath.string().c_str());

				if (ImGui::Button("Load file from explorer"))
				{
					const std::string returnValue = Files::ExplorerOpen("Text file (*.txt)\0*.txt\0");
					if (m_LatestFilePathExists = Files::Exists(returnValue.c_str()))
					{
						m_CurrentShaderFilePath = returnValue;
						LoadFile(m_CurrentShaderFilePath);
					}
				}

				if (m_LatestFilePathExists && ImGui::Button("Reload file from disk"))
				{
					LoadFile(m_CurrentShaderFilePath);
				}

				ImGui::InputTextMultiline("##ShaderFileTextBuffer", m_FileBuffer.As<char>(), m_FileBuffer.SizeInBytes(), ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_AllowTabInput);
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

			const u16 m_StartingBufferSize = 10000;
			Path m_CurrentShaderFilePath = Paths::ShadersDir();
			Buffer m_FileBuffer;
			bool m_LatestFilePathExists = false;
		};

	}

}