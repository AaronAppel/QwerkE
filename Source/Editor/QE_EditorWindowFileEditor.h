#pragma once

#ifdef _QCJSON
#include "Libraries/cJSON/cJSON.h"
#endif

#include "QE_EditorWindow.h"

#include "QE_EditorInspector.h"
#include "QF_Files.h"
#include "QF_Paths.h"

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

				if (ImGui::BeginChild("##JsonFileContents"), ImGui::GetContentRegionAvail().x - 10.f)
				{
					// #TODO Save file data to send to inspector instead of loading the file every frame

					// Inspector::InspectJsonFile(Paths::Scene("NewScene1.qscene").c_str(), true);
					Inspector::InspectJsonFile(m_JsonRootObject, true);
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
				}
				else
				{
					LOG_ERROR("{0} Error loading JSON file {1}!", __FUNCTION__, m_JsonFilePath.c_str());
				}
			}

			std::string m_JsonFilePath;
			cJSON* m_JsonRootObject = nullptr;
		};

	}

}