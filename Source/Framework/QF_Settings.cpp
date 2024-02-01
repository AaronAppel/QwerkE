#include "QF_Settings.h"

#include "Libraries/cJSON/QC_cJSON.h"
#include "Libraries/Mirror/Source/Mirror.h"

#include "QF_Defines.h"
#include "QF_Input.h"
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
        const cJSON* root = OpencJSONStream(configFilePath.c_str());
        if (root)
        {
            Serialization::DeserializeObjectFromJsonFile(root, m_engineSettings);
            ClosecJSONStream(root);
        }
    }

    void Settings::SaveEngineSettings(/* #TODO Take in file path */)
    {
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
        const cJSON* root = OpencJSONStream(projectSettingsFilePath.c_str());
        if (root)
        {
            Serialization::DeserializeObjectFromJsonFile(root, m_projectSettings);
            ClosecJSONStream(root);
        }
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
        const cJSON* root = OpencJSONStream(userSettingsFilePath.c_str());
        if (root)
        {
            Serialization::DeserializeObjectFromJsonFile(root, m_userSettings);
            ClosecJSONStream(root);
        }
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
