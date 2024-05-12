#include "QF_Settings.h"

#include "QF_Defines.h"
#include "QF_Log.h"
#include "QF_Serialization.h"

namespace QwerkE {

	namespace Settings {

		EngineSettings s_engineSettings;
		ProjectSettings s_projectSettings;
		UserSettings s_userSettings;
		RendererSettings s_rendererSettings;

		void LoadEngineSettings(/* #TODO Take in file path */)
		{
			const char* configFilePath = SettingsFolderPath(null_config);
			LoadEngineSettings(configFilePath);
		}

		void LoadEngineSettings(std::string configFilePath)
		{
			Serialization::DeserializeJsonFromFile(configFilePath.c_str(), s_engineSettings);
			s_engineSettings.isDirty = false;
		}

		void SaveEngineSettings(/* #TODO Take in file path */)
		{
			Serialization::SerializeObjectToFile(s_engineSettings, SettingsFolderPath(null_config));
			s_engineSettings.isDirty = false;
		}

		void LoadProjectSettings(std::string projectSettingsFilePath)
		{
			Serialization::DeserializeJsonFromFile(projectSettingsFilePath.c_str(), s_projectSettings);
			s_projectSettings.isDirty = false;
		}

		void SaveProjectSettings()
		{
			SaveProjectSettings(ProjectsFolderPath(null_project));
		}

		void SaveProjectSettings(std::string projectSettingsFilePath)
		{
			Serialization::SerializeObjectToFile(s_projectSettings, projectSettingsFilePath.c_str());
			s_projectSettings.isDirty = false;
		}

		void LoadUserSettings(std::string userSettingsFilePath)
		{
			Serialization::DeserializeJsonFromFile(userSettingsFilePath.c_str(), s_userSettings);
			s_userSettings.isDirty = false;
		}

		void SaveUserSettings()
		{
			Serialization::SerializeObjectToFile(s_userSettings, SettingsFolderPath(null_preferences));
			s_userSettings.isDirty = false;
		}

		void LoadRendererSettings(std::string rendererSettingsFilePath)
		{
			Serialization::DeserializeJsonFromFile(rendererSettingsFilePath.c_str(), s_rendererSettings);
			s_rendererSettings.isDirty = false;
		}

		void SaveRendererSettings()
		{
			Serialization::SerializeObjectToFile(s_rendererSettings, SettingsFolderPath("RendererSettings1.qren"));
			s_rendererSettings.isDirty = false;
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

		RendererSettings& GetRendererSettings()
		{
			return s_rendererSettings;
		}

	}

}
