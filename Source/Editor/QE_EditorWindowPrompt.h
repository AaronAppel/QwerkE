#pragma once

#include "QF_PathDefines.h"

#include "QE_EditorWindow.h"
#include "QE_Projects.h"

namespace QwerkE {

	namespace Editor {

		enum EditorWindowPromptTypes : uint8_t
		{
			PromptInvalid = 0,
			PromptCreateNewProject,
			PromptUnsavedChanges,  // #TODO Stop closing of window or editor if the user has not confirmed deleting unsaved changes
			PromptOpenNewEditorWindow
		};

		class EditorWindowPrompt : public EditorWindow
		{
		public:
			EditorWindowPrompt(GUID guid = GUID()) : EditorWindow("Prompt", EditorWindowTypes::Prompt, guid, (EditorWindowFlags)(AlignCentered | Singleton))
			{
				m_PromptStringBuffer.reserve(INT8_MAX);
				m_MinimumHeight = 150.f;
				m_MinimumWidth = 350.f;
				m_ImGuiFlags =	ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration |
								ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove |
								ImGuiWindowFlags_NoResize;
			}

			void SetPromptType(EditorWindowPromptTypes a_PromptType) { m_PromptType = a_PromptType; }
			EditorWindowPromptTypes GetPromptType() const { return m_PromptType; }

		private:
			void DrawInternal() override
			{
				switch (m_PromptType)
				{
				case EditorWindowPromptTypes::PromptCreateNewProject:
					NewProjectPrompt();
					break;

				case EditorWindowPromptTypes::PromptOpenNewEditorWindow:
					OpenNewEditorWindowPrompt();
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
				if (ImGui::InputText("##InputTextNewProjectName", m_PromptStringBuffer.data(), m_PromptStringBuffer.capacity()))
				{
					m_PromptStringBuffer.resize(strlen(m_PromptStringBuffer.data()));
				}

				ImGui::Separator();
				if (ImGui::Button(("Create new project##" + std::string(m_WindowName)).c_str()))
				{
					if (m_PromptStringBuffer.empty())
					{
						LOG_ERROR("{0} Project name cannot be empty!", __FUNCTION__);
						return;
					}
					// #TODO Add more sanity checks

					Project& currentProject = Projects::CurrentProject();
					currentProject.projectName = m_PromptStringBuffer;

					std::string newProjectFileName = m_PromptStringBuffer + "." + projects_file_ext;
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

			void OpenNewEditorWindowPrompt()
			{
				// #TODO Open up a text input (cursor focused) for user to enter the name of a window type
				// Can store recently opened window types, and maybe even favourites
				ImGui::Text("#TODO Open new window by type name");
			}

			EditorWindowPromptTypes m_PromptType = EditorWindowPromptTypes::PromptInvalid;
			std::string m_PromptStringBuffer = "New Project Name";
		};

	}

}