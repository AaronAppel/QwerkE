#include "QF_Settings.h"

#include "QF_Defines.h"
#include "QF_Log.h"
#include "QF_Serialization.h"

namespace QwerkE {

    EngineSettings Settings::m_engineSettings = EngineSettings();
    ProjectSettings Settings::m_projectSettings = ProjectSettings();
    UserSettings Settings::m_userSettings = UserSettings();

    void Settings::LoadEngineSettings(/* #TODO Take in file path */)
    {
        const char* configFilePath = SettingsFolderPath(null_config);
        LoadEngineSettings(configFilePath);
    }

    void Settings::LoadEngineSettings(std::string configFilePath)
    {
        Serialization::DeserializeJsonFromFile(configFilePath.c_str(), m_engineSettings);
    }

    void Settings::SaveEngineSettings(/* #TODO Take in file path */)
    {
        Serialization::SerializeObjectToFile(m_engineSettings, SettingsFolderPath(null_config));
    }

    void Settings::LoadProjectSettings(std::string projectSettingsFilePath)
    {
        Serialization::DeserializeJsonFromFile(projectSettingsFilePath.c_str(), m_projectSettings);
    }

    void Settings::SaveProjectSettings()
    {
        SaveProjectSettings(ProjectsFolderPath(null_project));
    }

    void Settings::SaveProjectSettings(std::string projectSettingsFilePath)
    {
        Serialization::SerializeObjectToFile(m_projectSettings, projectSettingsFilePath.c_str());
    }

    void Settings::LoadUserSettings(std::string userSettingsFilePath)
    {
        Serialization::DeserializeJsonFromFile(userSettingsFilePath.c_str(), m_userSettings);
    }

    void Settings::SaveUserSettings()
    {
        Serialization::SerializeObjectToFile(m_userSettings, SettingsFolderPath(null_preferences));
    }

}
