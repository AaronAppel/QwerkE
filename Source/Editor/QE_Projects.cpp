#include "QE_Projects.h"

#include <string>
#include <vector>

#include "QF_Assets.h"
#include "QF_Paths.h"
#include "QF_Scenes.h" // #TODO Remove
#include "QF_Serialize.h"

#include "QE_Editor.h"
#include "QE_EditorWindowPrompt.h"
#include "QE_Settings.h"

namespace QwerkE {

    namespace Projects {

		constexpr char* ProjectsDataFileName = "ProjectsData.qdata";
		constexpr char* s_NullProjectSettings = "null_project.qproj";

		static ProjectsData s_ProjectsData;
        static Project s_CurrentProject;

		void local_TryAddUniqueProjectFileName();

		void Initialize()
		{
			// #TODO Check for user or default project settings
			const std::string lastOpenedDataFilePath = Paths::Setting(ProjectsDataFileName).c_str();
			if (!Files::Exists(lastOpenedDataFilePath.c_str()))
			{
				Serialize::ToFile(s_ProjectsData, Paths::Setting(ProjectsDataFileName).c_str());
			}
			Serialize::FromFile(lastOpenedDataFilePath.c_str(), s_ProjectsData);

			ProjectsData& projectsData = Projects::GetProjectsData();
			Projects::LoadProject(projectsData.LastOpenedProjectFileName);
			if (!Projects::CurrentProject().isLoaded)
			{
				// #TODO Handle load project error
				NewEditorWindow(Editor::EditorWindowTypes::Prompt);
			}

			const EditorSettings& editorSettings = Settings::GetEditorSettings();
			if (editorSettings.startInPlayMode)
			{
				const std::vector<Scene*>& scenes = Scenes::LookAtScenes();
				for (size_t i = 0; i < scenes.size(); i++)
				{
					scenes[i]->SetIsPaused(editorSettings.startInPlayMode);
				}
			}
		}

		void Shutdown()
		{
			Serialize::ToFile(s_ProjectsData, Paths::Setting(ProjectsDataFileName).c_str());
			if (s_CurrentProject.isLoaded)
			{
				Serialize::ToFile(s_CurrentProject, Paths::Project(s_CurrentProject.projectFileName.c_str()).c_str());
			}
		}

		ProjectsData& GetProjectsData()
		{
			return s_ProjectsData;
		}

        Project& CurrentProject()
        {
            return s_CurrentProject;
        }

		void LoadProject(const std::string& projectSettingsFileName)
		{
			if (s_CurrentProject.isLoaded)
			{
				LOG_WARN("{0} Project {1} already loaded", __FUNCTION__, s_CurrentProject.projectName);
				UnloadCurrentProject();
			}

			std::string projectFilePath = Paths::Project(projectSettingsFileName.c_str());
			if (!Files::Exists(projectFilePath.c_str()))
				return;

			Serialize::FromFile(projectFilePath.c_str(), s_CurrentProject);
			s_CurrentProject.isLoaded = true;
			s_CurrentProject.isDirty = false;

			s_CurrentProject.projectFileName = projectSettingsFileName;

			const u8 range = s_CurrentProject.scenesList.size();

			for (const auto& guidSceneNamePairs : s_CurrentProject.scenesList)
			{
				if (const Scene* newScene = Assets::Get<Scene>(guidSceneNamePairs.first))
				{
					// if (!Scenes::HasScene(newScene))
					{
						// #TODO Add scene? Scenes can be empty on reload but assets still exist in Assets::
					}

					if (newScene->GetSceneName() == s_CurrentProject.startUpSceneName)
					{
						Scenes::SetCurrentScene(newScene->GetGuid());
					}
				}
			}

			s_ProjectsData.LastOpenedProjectFileName = s_CurrentProject.projectFileName;
			local_TryAddUniqueProjectFileName();
		}

		void LoadProjectFromExplorer()
		{
			std::string result = Files::ExplorerOpen("QwerkE Project (*.qproj)\0*.qproj\0");
			if (!result.empty())
			{
				Path fileName = Files::FileName(result.c_str());
				Projects::LoadProject(fileName.string().c_str());
			}
		}

		void UnloadCurrentProject()
		{
			if (s_CurrentProject.isLoaded)
			{
				// #TODO Prompt to save unsaved changes before unloading
				Scenes::Shutdown();
				s_CurrentProject.isLoaded = false;
				s_CurrentProject.isDirty = false;
			}
		}

		void SaveProject()
		{
			SaveProjectToFile(s_CurrentProject.projectFileName);
		}

		void SaveProjectToFile(const std::string& projectSettingsFileName)
		{
			// #TODO Review changing s_CurrentProject.projectFileName when creating a new file.
			// What values and editor state should change?
			if (!s_CurrentProject.isLoaded)
			{
				LOG_WARN("{0} Project {1} is not loaded", __FUNCTION__, s_CurrentProject.projectName);
				// return;
			}

			Path fileName = Files::FileName(projectSettingsFileName.c_str());
			std::string projectFilePath = Paths::Project(fileName.string().c_str());

			s_CurrentProject.projectFileName = fileName.string();
			Serialize::ToFile(s_CurrentProject, projectFilePath.c_str());
			s_CurrentProject.isDirty = false;
			local_TryAddUniqueProjectFileName();
		}

		void SaveProjectFromExplorer()
		{
			// #TODO Change current project to point to new file?
			std::string result = Files::ExplorerSave("QwerkE Project (*.qproj)\0*.qproj\0");
			if (!result.empty())
			{
				Projects::SaveProjectToFile(result.c_str());
			}
		}

		void local_TryAddUniqueProjectFileName()
		{
			bool isUnique = true;
			for (size_t i = 0; i < s_ProjectsData.PreviousProjectFileNames.size(); i++)
			{
				if (strcmp(s_ProjectsData.PreviousProjectFileNames[i].c_str(), s_CurrentProject.projectFileName.c_str()) == 0)
				{
					isUnique = false;
					break;
				}
			}

			if (isUnique)
			{
				s_ProjectsData.PreviousProjectFileNames.push_back(s_CurrentProject.projectFileName);
			}
		}

    }

}
