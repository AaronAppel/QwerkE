#include "QF_Projects.h"

#include <string>

#include "QF_Serialization.h"

namespace QwerkE {

    namespace Projects {

		constexpr char* s_NullProjectSettings = "null_project.qproj";

        Project s_CurrentProject;

        Project& CurrentProject()
        {
            return s_CurrentProject;
        }

		void UnloadCurrentProject() // #TODO Expose in header?
		{
			// #TODO Unload s_CurrentProject
		}

		void LoadProject(const std::string& projectSettingsFilePath)
		{
			if (s_CurrentProject.isLoaded)
			{
				UnloadCurrentProject();
			}

			std::string projectFilePath = Paths::Project(projectSettingsFilePath.c_str());
			Serialization::NewDeserializeFromFile(projectFilePath.c_str(), s_CurrentProject);
		}

		void SaveProject()
		{
			std::string projectFilePath = Paths::Project(s_CurrentProject.projectName);
			s_CurrentProject.isDirty = false;
		}

    }

}
