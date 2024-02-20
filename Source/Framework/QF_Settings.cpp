#include "QF_Settings.h"

#include "QF_Defines.h"
#include "QF_Log.h"
#include "QF_Serialization.h"

namespace QwerkE {

	namespace Settings {

		EngineSettings s_engineSettings;
		ProjectSettings s_projectSettings;
		UserSettings s_userSettings;

		void LoadEngineSettings(/* #TODO Take in file path */)
		{
			const char* configFilePath = SettingsFolderPath(null_config);
			LoadEngineSettings(configFilePath);
		}

		void LoadEngineSettings(std::string configFilePath)
		{
			Serialization::DeserializeJsonFromFile(configFilePath.c_str(), s_engineSettings);
		}

		void SaveEngineSettings(/* #TODO Take in file path */)
		{
			Serialization::SerializeObjectToFile(s_engineSettings, SettingsFolderPath(null_config));
		}

		void LoadProjectSettings(std::string projectSettingsFilePath)
		{
			Serialization::DeserializeJsonFromFile(projectSettingsFilePath.c_str(), s_projectSettings);
		}

		void SaveProjectSettings()
		{
			SaveProjectSettings(ProjectsFolderPath(null_project));
		}

		void SaveProjectSettings(std::string projectSettingsFilePath)
		{
			Serialization::SerializeObjectToFile(s_projectSettings, projectSettingsFilePath.c_str());
		}

		void LoadUserSettings(std::string userSettingsFilePath)
		{
			Serialization::DeserializeJsonFromFile(userSettingsFilePath.c_str(), s_userSettings);
		}

		void SaveUserSettings()
		{
			Serialization::SerializeObjectToFile(s_userSettings, SettingsFolderPath(null_preferences));
		}

		EngineSettings& GetEngineSettings()
		{
			return s_engineSettings;
		}

		ProjectSettings& GetProjectSettings()
		{
			return s_projectSettings;
		}

		UserSettings& GetUserSettings()
		{
			return s_userSettings;
		}

	}

}
