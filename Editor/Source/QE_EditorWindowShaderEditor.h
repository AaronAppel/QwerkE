#pragma once

#include "QF_Paths.h"

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowShaderEditor : public EditorWindow
		{
		public:
			EditorWindowShaderEditor::EditorWindowShaderEditor(GUID guid = GUID()) : EditorWindow("Shader Editor", EditorWindowTypes::ShaderEditor, guid) { }

		private:
			void DrawInternal() override
			{
				ImGui::Text("Shader file path: ");
				ImGui::SameLine();
				ImGui::Text(m_CurrentFilePath.string().c_str());

				if (ImGui::Button("Load"))
				{
					LoadFile();
				}

				ImGui::InputText("##TextBuffer", m_FileBuffer.data(), m_FileBuffer.capacity());
			}

			void LoadFile()
			{
				m_FileBuffer.reserve(m_MaxBufferSize);
			}

			const u16 m_MaxBufferSize = 10000;
			Path m_CurrentFilePath = Paths::ShadersDir();
			std::string m_FileBuffer;
		};

	}

}