#pragma once

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

		class EditorWindowPrompt : public EditorWindow
		{
		public:
			EditorWindowPrompt(GUID guid = GUID()) : EditorWindow("Prompt", EditorWindowTypes::Prompt, guid, AlignCentered)
			{
				m_NewProjectNameBuffer.reserve(INT8_MAX);
				m_MinimumHeight = 200.f;
				m_MinimumWidth = 200.f;
				m_ImGuiFlags =	ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration |
								ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove |
								ImGuiWindowFlags_NoResize;
			}

		private:
			void DrawInternal() override
			{
				if (!Projects::CurrentProject().isLoaded)
				{
					ImGui::Text("New Project Name: ");
					if (ImGui::InputText("##InputTextNewProjectName", m_NewProjectNameBuffer.data(), m_NewProjectNameBuffer.capacity()))
					{
						m_NewProjectNameBuffer.resize(strlen(m_NewProjectNameBuffer.data()));
					}

					if (ImGui::Button("Create new project"))
					{
						// #TODO Sanity checks
						if (m_NewProjectNameBuffer.empty())
						{
							// LOG_ERROR("{0} Project name cannot be empty!", __FUNCTION__);
							return;
						}

						Project& currentProject = Projects::CurrentProject();
						currentProject.projectName = m_NewProjectNameBuffer;

						std::string newProjectFileName = m_NewProjectNameBuffer + "." + projects_file_ext;
						currentProject.projectFileName = newProjectFileName;
						Projects::SaveProjectToFile(newProjectFileName);
					}
				}
				else
				{
					Close();
				}
			}

			std::string m_NewProjectNameBuffer = "New Project Name";
		};

	}

}