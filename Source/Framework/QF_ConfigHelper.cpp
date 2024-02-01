#include "QF_ConfigHelper.h"

#include "Libraries/cJSON/QC_cJSON.h"
#include "Libraries/Mirror/Source/Mirror.h"

#include "QF_Defines.h"
#include "QF_Input.h"
#include "QF_Log.h"
#include "QF_Serialization.h"

namespace QwerkE {

    ConfigData ConfigHelper::m_ConfigData = ConfigData();
    UserData ConfigHelper::m_UserData = UserData();
    ProjectData ConfigHelper::m_ProjectData = ProjectData();

    void ConfigHelper::LoadConfigData()
    {
        const char* configFilePath = ConfigsFolderPath(null_config); // #TODO How to handle file names?
        LoadConfigData(configFilePath);
    }

    void ConfigHelper::LoadConfigData(std::string configFilePath)
    {
        const cJSON* root = OpencJSONStream(configFilePath.c_str());
        if (root)
        {
            Serialization::DeserializeObjectFromJsonFile(root, m_ConfigData);
            ClosecJSONStream(root);
        }
    }

    void ConfigHelper::LoadUserData(std::string preferencesFilePath)
    {
        const cJSON* root = OpencJSONStream(preferencesFilePath.c_str());
        if (root)
        {
            Serialization::DeserializeObjectFromJsonFile(root, m_UserData);
            ClosecJSONStream(root);
        }
    }

    void ConfigHelper::SaveConfigData(ConfigData config)
    {
        m_ConfigData = config;
        SaveConfigData();
    }

    void ConfigHelper::SaveConfigData()
    {
        cJSON* jsonRootObject = cJSON_CreateObject();
        if (jsonRootObject)
        {
            Serialization::SerializeObject(jsonRootObject, m_ConfigData);
            const char* configFilePath = ConfigsFolderPath(null_config);
            PrintRootObjectToFile(configFilePath, jsonRootObject); // #TODO Take in config file path as arg
        }
    }

    void ConfigHelper::SaveUserData()
    {
        cJSON* root = cJSON_CreateObject();
        if (root)
        {
            Serialization::SerializeObject(root, m_UserData);
            const char* preferencesFilePath = PreferencesFolderPath(null_preferences);
            PrintRootObjectToFile(preferencesFilePath, root); // #TODO Take in config file path as arg
        }
    }

    void ConfigHelper::LoadProjectData(std::string preferencesFilePath)
    {
        const cJSON* root = OpencJSONStream(preferencesFilePath.c_str());
        if (root)
        {
            Serialization::DeserializeObjectFromJsonFile(root, m_ProjectData);
            ClosecJSONStream(root);
        }
    }

    void ConfigHelper::SaveProjectData()
    {
        cJSON* root = cJSON_CreateObject();
        if (root)
        {
            Serialization::SerializeObject(root, m_ProjectData);
            const char* projectFilePath = ProjectsFolderPath(null_project);
            PrintRootObjectToFile(projectFilePath, root); // #TODO Take in project file path as arg
        }
    }

}
