#include "QF_Settings.h"

#include "Libraries/cJSON/QC_cJSON.h" // #TODO Deprecate reference to library

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
        Serialization::DeserializeObjectFromFile(configFilePath.c_str(), m_engineSettings);
    }

    void Settings::SaveEngineSettings(/* #TODO Take in file path */)
    {
        // Serialization::SerializeObjectFromFile(configFilePath.c_str(), m_engineSettings);
        cJSON* jsonRootObject = cJSON_CreateObject();
        if (jsonRootObject)
        {
            Serialization::SerializeObject(jsonRootObject, m_engineSettings);
            const char* configFilePath = SettingsFolderPath(null_config);
            PrintRootObjectToFile(configFilePath, jsonRootObject);
        }
    }

    void Settings::SaveEngineSettings(const EngineSettings& engineSettings)
    {
        m_engineSettings = engineSettings;
        SaveEngineSettings();
    }

    void Settings::LoadProjectSettings(std::string projectSettingsFilePath)
    {
        Serialization::DeserializeObjectFromFile(projectSettingsFilePath.c_str(), m_projectSettings);
    }

    void Settings::SaveProjectSettings()
    {
        cJSON* root = cJSON_CreateObject(/* #TODO Take in file path */);
        if (root)
        {
            Serialization::SerializeObject(root, m_projectSettings);
            const char* preferencesFilePath = ProjectsFolderPath(null_project);
            PrintRootObjectToFile(preferencesFilePath, root);
        }
    }

    void Settings::LoadUserSettings(std::string userSettingsFilePath)
    {
        Serialization::DeserializeObjectFromFile(userSettingsFilePath.c_str(), m_userSettings);
    }

    void Settings::SaveUserSettings()
    {
        cJSON* root = cJSON_CreateObject(/* #TODO Take in file path */);
        if (root)
        {
            Serialization::SerializeObject(root, m_userSettings);
            const char* projectFilePath = SettingsFolderPath(null_preferences);
            PrintRootObjectToFile(projectFilePath, root);
        }
    }

}
