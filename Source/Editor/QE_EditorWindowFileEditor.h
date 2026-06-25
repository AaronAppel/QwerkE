#pragma once

#ifdef _QCJSON
#include "Libraries/cJSON/cJSON.h"
#endif

#include "QE_EditorWindow.h"

#include "QE_EditorInspector.h"
#include "QF_Files.h"
#include "QF_Paths.h"

// Dear ImGui Color Text Edit
#include "../Libraries/ImGuiColorTextEdit/TextEditor.h"

// struct cJSON;

namespace QwerkE {

	namespace Editor {

		class EditorWindowFileEditor : public EditorWindow
		{
		public:
			EditorWindowFileEditor(GUID guid) :
				EditorWindow("File Editor",
					EditorWindowTypes::FileEditor,
					guid,
					(EditorWindowFlags)(0))
			{
				m_JsonFilePath = Paths::Scene("GameScene1.qscene");
				// m_JsonFilePath = "B:\\QwerkE\\Test.json";
				LoadFileData();
			}

			~EditorWindowFileEditor()
			{
				if (m_JsonRootObject)
				{
					cJSON_Delete(m_JsonRootObject);
				}
			}

		private:
			void DrawInternal() override
			{
				const float fontScaling = 1.25f;
				ImGui::SetWindowFontScale(fontScaling);
				if (ImGui::BeginChild("##JsonFilePath", { ImGui::GetContentRegionAvail().x, 32.f * fontScaling }))
				{
					if (ImGui::Button("Load File")) // #TODO Launch explorer
					{
						LoadFileData();
					}
					ImGui::SameLine();
					ImGui::Text("File:");
					ImGui::SameLine();
					ImGui::Text(m_JsonFilePath.c_str());
					ImGui::Separator();
				}
				ImGui::EndChild();

				// Split area: left = text editor, right = inspector
				ImGui::BeginChild("##FileEditorSplit", ImGui::GetContentRegionAvail());
				{
					float fullW = ImGui::GetContentRegionAvail().x;
					float halfW = fullW * 0.6f; // editor gets 60%

					ImGui::BeginChild("##TextEditorPane", { halfW, 0 }, true);
					{
						// Render the TextEditor instance
						m_TextEditor.Render("TextEditor");
					}
					ImGui::EndChild();

					ImGui::SameLine();

					ImGui::BeginChild("##InspectorPane", { 0, 0 }, true);
					{
						// Show JSON inspector on the right
						Inspector::InspectJsonFile(m_JsonRootObject, true);
					}
					ImGui::EndChild();
				}
				ImGui::EndChild();
			}

			void LoadFileData()
			{
				ASSERT(!m_JsonRootObject, "json structure already instantiated");

				if (Buffer jsonFileBuffer = Files::LoadFile(m_JsonFilePath.c_str()))
				{
					m_JsonRootObject = cJSON_Parse(jsonFileBuffer.As<char>());
					if (!m_JsonRootObject)
					{
						LOG_ERROR("{0} Could not parse JSON file {1}! Possible compile error. Check file for typos", __FUNCTION__, m_JsonFilePath.c_str());
					}
					// Also set the text editor buffer to the file contents
					m_TextEditor.SetText(std::string(jsonFileBuffer.As<char>()));
				}
				else
				{
					LOG_ERROR("{0} Error loading JSON file {1}!", __FUNCTION__, m_JsonFilePath.c_str());
				}
			}

			std::string m_JsonFilePath;
			cJSON* m_JsonRootObject = nullptr;

			// ImGui Color Text Editor instance
			TextEditor m_TextEditor;
		};

	}

}