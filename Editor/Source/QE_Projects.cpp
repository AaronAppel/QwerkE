#include "QE_Projects.h"

#include <string>
#include <vector>

#include "QF_Scenes.h"
#include "QF_Serialize.h"

namespace QwerkE {

    namespace Projects {

		constexpr char* ProjectsDataFileName = "ProjectsData.qdata";
		constexpr char* s_NullProjectSettings = "null_project.qproj";

		static ProjectsData s_ProjectsData;
        static Project s_CurrentProject;

		void local_TryAddUniqueProjectFileName();

		void Initialize()
		{
			const std::string lastOpenedDataFilePath = Paths::Setting(ProjectsDataFileName).c_str();
			if (!Files::Exists(lastOpenedDataFilePath.c_str()))
			{
				Serialize::ToFile(s_ProjectsData, Paths::Setting(ProjectsDataFileName).c_str());
			}
			Serialize::FromFile(lastOpenedDataFilePath.c_str(), s_ProjectsData);
		}

		void Shutdown()
		{
			Serialize::ToFile(s_ProjectsData, Paths::Setting(ProjectsDataFileName).c_str());
		}

		ProjectsData& GetProjectsData()
		{
			return s_ProjectsData;
		}

        Project& CurrentProject()
        {
            return s_CurrentProject;
        }

		void UnloadCurrentProject() // #TODO Expose in header?
		{
			// #TODO Prompt to save unsaved changes before unloading
			Scenes::Shutdown();
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

			s_CurrentProject.projectFileName = projectSettingsFileName;

			for (size_t i = 0; i < s_CurrentProject.sceneFileNames.size(); i++)
			{
				Scenes::CreateSceneFromFile(Paths::Scene(s_CurrentProject.sceneFileNames[i].c_str()), false);
			}
			s_ProjectsData.LastOpenedProjectFileName = s_CurrentProject.projectFileName;
			local_TryAddUniqueProjectFileName();
		}

		void SaveProject()
		{
			SaveProjectToFile(s_CurrentProject.projectFileName);
		}

		void SaveProjectToFile(const std::string& projectSettingsFileName)
		{
			// #TODO Review changing s_CurrentProject.projectFileName when creating a new file.
			// What values and editor state should change?
			if (s_CurrentProject.isLoaded)
			{
				LOG_WARN("{0} Project {1} is not loaded", __FUNCTION__, s_CurrentProject.projectName);
				return;
			}

			Path fileName = Files::FileName(projectSettingsFileName.c_str());
			std::string projectFilePath = Paths::Project(fileName.string().c_str());
			if (Files::Exists(projectFilePath.c_str()))
			{
				s_CurrentProject.projectFileName = fileName.string();
				Serialize::ToFile(s_CurrentProject, projectFilePath.c_str());
				s_CurrentProject.isDirty = false;
				local_TryAddUniqueProjectFileName();
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
