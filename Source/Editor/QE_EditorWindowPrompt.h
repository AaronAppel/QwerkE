#pragma once

#include "QF_PathDefines.h"

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		enum EditorWindowPromptTypes : uint8_t
		{
			PromptInvalid = 0,
			PromptCreateNewProject,
			// PromptUnsavedChanges
		};

		class EditorWindowPrompt : public EditorWindow
		{
		public:
			EditorWindowPrompt(GUID guid = GUID()) : EditorWindow("Prompt", EditorWindowTypes::Prompt, guid, (EditorWindowFlags)(AlignCentered | Singleton))
			{
				m_NewProjectNameBuffer.reserve(INT8_MAX);
				m_MinimumHeight = 50.f;
				m_MinimumWidth = 300.f;
				m_ImGuiFlags =	ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration |
								ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove |
								ImGuiWindowFlags_NoResize;
			}

		private:
			void DrawInternal() override
			{
				switch (m_PromptType)
				{
				case EditorWindowPromptTypes::PromptCreateNewProject:
					NewProjectPrompt();
					break;

				default:
					Close();
					break;
				}
			}

			void NewProjectPrompt()
			{
				ImGui::Text("New Project Name: ");
				ImGui::SameLine();
				if (ImGui::InputText("##InputTextNewProjectName", m_NewProjectNameBuffer.data(), m_NewProjectNameBuffer.capacity()))
				{
					m_NewProjectNameBuffer.resize(strlen(m_NewProjectNameBuffer.data()));
				}

				ImGui::Separator();
				if (ImGui::Button(("Create new project##" + std::string(m_WindowName)).c_str()))
				{
					if (m_NewProjectNameBuffer.empty())
					{
						LOG_ERROR("{0} Project name cannot be empty!", __FUNCTION__);
						return;
					}
					// #TODO Add more sanity checks

					Project& currentProject = Projects::CurrentProject();
					currentProject.projectName = m_NewProjectNameBuffer;

					std::string newProjectFileName = m_NewProjectNameBuffer + "." + projects_file_ext;
					currentProject.projectFileName = newProjectFileName;
					Projects::SaveProjectToFile(newProjectFileName);
					Projects::LoadProject(newProjectFileName);
					Close();
				}

				ImGui::SameLineEnd(3);
				if (ImGui::Button(("Cancel##" + std::string(m_WindowName)).c_str()))
				{
					Close();
				}
			}

			// #TODO Allow setting different prompt types
			EditorWindowPromptTypes m_PromptType = EditorWindowPromptTypes::PromptCreateNewProject;
			std::string m_NewProjectNameBuffer = "New Project Name";
		};

	}

}