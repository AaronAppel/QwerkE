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

		void LoadProject(const std::string& projectSettingsFilePath)
		{
			std::string projectFilePath = Paths::Project(projectSettingsFilePath.c_str());
			Serialization::DeserializeObjectFromFile(projectFilePath.c_str(), s_CurrentProject);
			Serialization::NewDeserializeFromToFile("NewSerialization", s_CurrentProject);
		}

		void SaveProject()
		{
			std::string projectFilePath = Paths::Project(s_CurrentProject.projectName);
			s_CurrentProject.isDirty = false;
		}

    }

}
