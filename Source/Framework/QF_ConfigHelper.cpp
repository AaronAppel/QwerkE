#include "QF_ConfigHelper.h"

#include "Libraries/cJSON/QC_cJSON.h"

#include "QC_Reflection.h"

#include "QF_Defines.h"
#include "QF_Input.h"
#include "QF_Log.h"
#include "QF_Serialization.h"

namespace QwerkE {

    ConfigData ConfigHelper::m_ConfigData = ConfigData();
    UserData ConfigHelper::m_UserData = UserData();

    void ConfigHelper::LoadConfigData()
    {
        const char* configFilePath = ConfigsFolderPath(null_config); // #TODO How to handle file names?
        LoadConfigData(configFilePath);
    }

    void ConfigHelper::LoadConfigData(std::string configFilePath)
    {
        const cJSON* root = OpencJSONStream(configFilePath.c_str());
        if (root == nullptr)
        {
            LOG_ERROR("Unable to load json for file {0}", configFilePath);
            return;
        }

        const Reflection::ClassInfo* configDataClassInfo = Reflection::GetClass<ConfigData>();
        for (size_t i = 0; i < configDataClassInfo->fields.size(); i++)
        {
            Reflection::Field configDataField = configDataClassInfo->fields.at(i);
            if (!configDataField.type)
                break; // #NOTE Future fields will also have null types

            switch (configDataField.type->enumType)
            {
            case ReflectionType::FrameworkData:
                Serialization::DeserializeJsonObject<FrameworkData>(GetItemFromRootByKey(root, TYPENAME_TO_STR(FrameworkData)), m_ConfigData.frameworkData);
                break;

            case ReflectionType::Libraries:
                Serialization::DeserializeObjectByTypeKey<Libraries>(root, m_ConfigData.libraries);
                break;

            case ReflectionType::ScenesData:
                Serialization::DeserializeObjectByTypeKey<ScenesData>(root, m_ConfigData.scenesData);
                break;

            case ReflectionType::SceneSettings:
                Serialization::DeserializeObjectByTypeKey<SceneSettings>(root, m_ConfigData.sceneSettings);
                break;

            case ReflectionType::Systems:
                Serialization::DeserializeObjectByTypeKey<Systems>(root, m_ConfigData.systems);
                break;

            case ReflectionType::EngineSettings:
                Serialization::DeserializeObjectByTypeKey<EngineSettings>(root, m_ConfigData.engineSettings);
                break;

            default:
                LOG_ERROR("{0} Unhandled field type {1}(2) for serialization!", __FUNCTION__, configDataField.type->stringName, (int)configDataField.type->enumType);
                break;
            }
        }

        ClosecJSONStream(root);
    }

    void ConfigHelper::LoadUserData(std::string preferencesFilePath)
    {
        const cJSON* root = OpencJSONStream(preferencesFilePath.c_str());
        if (root == nullptr)
        {
            LOG_ERROR("Unable to load json for file {0}", preferencesFilePath);
            return;
        }

        const Reflection::ClassInfo* userDataClassInfo = Reflection::GetClass<UserData>();
        for (size_t i = 0; i < userDataClassInfo->fields.size(); i++)
        {
            Reflection::Field userDataField = userDataClassInfo->fields.at(i);
            if (!userDataField.type)
                break;

            switch (userDataField.type->enumType)
            {
            case ReflectionType::Controls:
                Serialization::DeserializeObjectByTypeKey<Controls>(root, m_UserData.controls);
                break;

            default:
                LOG_ERROR("{0} Unhandled field type {1}(2) for serialization!", __FUNCTION__, userDataField.type->stringName, (int)userDataField.type->enumType);
                break;
            }
        }

        ClosecJSONStream(root);
    }

    void ConfigHelper::SaveConfigData(ConfigData config)
    {
        m_ConfigData = config;
        SaveConfigData();
    }

    void ConfigHelper::SaveConfigData()
    {
        cJSON* jsonRoot = CreateObject();

        const Reflection::ClassInfo* configDataClassInfo = Reflection::GetClass<ConfigData>();
        for (size_t i = 0; i < configDataClassInfo->fields.size(); i++)
        {
            Reflection::Field userDataField = configDataClassInfo->fields.at(i);
            if (!userDataField.type)
                break;

            switch (userDataField.type->enumType)
            {
            case ReflectionType::FrameworkData:
                Serialization::SerializeObject<FrameworkData>(jsonRoot, m_ConfigData.frameworkData);
                break;

            case ReflectionType::Libraries:
                Serialization::SerializeObject<Libraries>(jsonRoot, m_ConfigData.libraries);
                break;

            case ReflectionType::ScenesData:
                Serialization::SerializeObject<ScenesData>(jsonRoot, m_ConfigData.scenesData);
                break;

            case ReflectionType::SceneSettings:
                Serialization::SerializeObject<SceneSettings>(jsonRoot, m_ConfigData.sceneSettings);
                break;

            case ReflectionType::Systems:
                Serialization::SerializeObject<Systems>(jsonRoot, m_ConfigData.systems);
                break;

            case ReflectionType::EngineSettings:
                Serialization::SerializeObject<EngineSettings>(jsonRoot, m_ConfigData.engineSettings);
                break;

            default:
                LOG_ERROR("{0} Unhandled field type {1}(2) for serialization!", __FUNCTION__, userDataField.type->stringName, (int)userDataField.type->enumType);
                break;
            }
        }

        const char* configFilePath = ConfigsFolderPath(null_config);
        PrintRootObjectToFile(configFilePath, jsonRoot); // #TODO Take in config file path as arg
    }

    void ConfigHelper::SaveUserData()
    {
        cJSON* jsonRoot = CreateObject();

        const Reflection::ClassInfo* userDataClassInfo = Reflection::GetClass<UserData>();
        for (size_t i = 0; i < userDataClassInfo->fields.size(); i++)
        {
            Reflection::Field userDataField = userDataClassInfo->fields.at(i);
            if (!userDataField.type)
                break;

            switch (userDataField.type->enumType)
            {
            case ReflectionType::Controls:
                Serialization::SerializeObject<Controls>(jsonRoot, m_UserData.controls);
                break;

            default:
                LOG_ERROR("{0} Unhandled field type {1}(2) for serialization!", __FUNCTION__, userDataField.type->stringName, (int)userDataField.type->enumType);
                break;
            }
        }

        const char* preferencesFilePath = PreferencesFolderPath(null_preferences);
        PrintRootObjectToFile(preferencesFilePath, jsonRoot); // #TODO Take in config file path as arg
    }

}
