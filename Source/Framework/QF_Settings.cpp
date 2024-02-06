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
        Serialization::SerializeObjectToFile(SettingsFolderPath(null_config), m_engineSettings);
    }

    void Settings::SaveEngineSettings(const EngineSettings& engineSettings)
    {
        m_engineSettings = engineSettings;
        SaveEngineSettings();
    }

    void Settings::LoadProjectSettings(std::string projectSettingsFilePath)
    {
        Serialization::DeserializeJsonFromFile(projectSettingsFilePath.c_str(), m_projectSettings);
    }

    void Settings::SaveProjectSettings()
    {
        Serialization::SerializeObjectToFile(ProjectsFolderPath(null_project), m_projectSettings);
    }

    void Settings::LoadUserSettings(std::string userSettingsFilePath)
    {
        Serialization::DeserializeJsonFromFile(userSettingsFilePath.c_str(), m_userSettings);
    }

    void Settings::SaveUserSettings()
    {
        Serialization::SerializeObjectToFile(SettingsFolderPath(null_preferences), m_userSettings);
    }

}
