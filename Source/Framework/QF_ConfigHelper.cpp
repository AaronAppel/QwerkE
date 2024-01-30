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
        if (root == nullptr)
        {
            LOG_ERROR("Unable to load json for file {0}", configFilePath);
            return;
        }

        const Mirror::ClassInfo* configDataClassInfo = Mirror::InfoForClass<ConfigData>();
        for (size_t i = 0; i < configDataClassInfo->fields.size(); i++)
        {
            Mirror::Field configDataField = configDataClassInfo->fields.at(i);
            if (!configDataField.type)
                break; // #NOTE Future fields will also have null types

            switch (configDataField.type->enumType)
            {
            case MirrorTypes::FrameworkData:
                Serialization::DeserializeJsonObject<FrameworkData>(GetItemFromRootByKey(root, MIRROR_TO_STR(FrameworkData)), m_ConfigData.frameworkData);
                break;

            case MirrorTypes::Libraries:
                Serialization::DeserializeObjectByTypeKey<Libraries>(root, m_ConfigData.libraries);
                break;

            case MirrorTypes::ScenesData:
                Serialization::DeserializeObjectByTypeKey<ScenesData>(root, m_ConfigData.scenesData);
                break;

            case MirrorTypes::SceneSettings:
                Serialization::DeserializeObjectByTypeKey<SceneSettings>(root, m_ConfigData.sceneSettings);
                break;

            case MirrorTypes::Systems:
                Serialization::DeserializeObjectByTypeKey<Systems>(root, m_ConfigData.systems);
                break;

            case MirrorTypes::EngineSettings:
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

        const Mirror::ClassInfo* userDataClassInfo = Mirror::InfoForClass<UserData>();
        for (size_t i = 0; i < userDataClassInfo->fields.size(); i++)
        {
            Mirror::Field userDataField = userDataClassInfo->fields.at(i);
            if (!userDataField.type)
                break;

            switch (userDataField.type->enumType)
            {
            case MirrorTypes::Controls:
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
        cJSON* jsonRootObject = cJSON_CreateObject();

        const Mirror::ClassInfo* configDataClassInfo = Mirror::InfoForClass<ConfigData>();
        for (size_t i = 0; i < configDataClassInfo->fields.size(); i++)
        {
            Mirror::Field userDataField = configDataClassInfo->fields.at(i);
            if (!userDataField.type)
                break;

            switch (userDataField.type->enumType)
            {
            case MirrorTypes::FrameworkData:
                Serialization::SerializeObject<FrameworkData>(jsonRootObject, m_ConfigData.frameworkData);
                break;

            case MirrorTypes::Libraries:
                Serialization::SerializeObject<Libraries>(jsonRootObject, m_ConfigData.libraries);
                break;

            case MirrorTypes::ScenesData:
                Serialization::SerializeObject<ScenesData>(jsonRootObject, m_ConfigData.scenesData);
                break;

            case MirrorTypes::SceneSettings:
                Serialization::SerializeObject<SceneSettings>(jsonRootObject, m_ConfigData.sceneSettings);
                break;

            case MirrorTypes::Systems:
                Serialization::SerializeObject<Systems>(jsonRootObject, m_ConfigData.systems);
                break;

            case MirrorTypes::EngineSettings:
                Serialization::SerializeObject<EngineSettings>(jsonRootObject, m_ConfigData.engineSettings);
                break;

            default:
                LOG_ERROR("{0} Unhandled field type {1}(2) for serialization!", __FUNCTION__, userDataField.type->stringName, (int)userDataField.type->enumType);
                break;
            }
        }

        const char* configFilePath = ConfigsFolderPath(null_config);
        PrintRootObjectToFile(configFilePath, jsonRootObject); // #TODO Take in config file path as arg
    }

    void ConfigHelper::SaveUserData()
    {
        cJSON* jsonRoot = cJSON_CreateObject();

        const Mirror::ClassInfo* userDataClassInfo = Mirror::InfoForClass<UserData>();
        for (size_t i = 0; i < userDataClassInfo->fields.size(); i++)
        {
            Mirror::Field userDataField = userDataClassInfo->fields.at(i);
            if (!userDataField.type)
                break;

            switch (userDataField.type->enumType)
            {
            case MirrorTypes::Controls:
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

    void ConfigHelper::LoadProjectData(std::string preferencesFilePath)
    {
        m_ProjectData;
    }

    void ConfigHelper::SaveProjectData()
    {
        cJSON* jsonRootObject = cJSON_CreateObject();

        if (true)
        {
            Serialization::SerializeObject<ProjectData>(jsonRootObject, m_ProjectData);
        }
        else
        {
            const Mirror::ClassInfo* projectDataClassInfo = Mirror::InfoForClass<ProjectData>();
            for (size_t i = 0; i < projectDataClassInfo->fields.size(); i++)
            {
                Mirror::Field projectDataField = projectDataClassInfo->fields.at(i);
                if (!projectDataField.type)
                    break;

                switch (projectDataField.type->enumType)
                {
                case MirrorTypes::m_constCharPtr:
                   /* if (strcmp(projectDataField.name.c_str(), "projectName") == 0)
                    {
                        Serialization::SerializeJsonField(jsonRootObject, projectDataField, (void*)m_ProjectData.projectName);
                    }
                    else if (strcmp(projectDataField.name.c_str(), "assetsRoot") == 0)
                    {
                        Serialization::SerializeJsonField(jsonRootObject, projectDataField, (void*)m_ProjectData.assetsRoot);
                   }*/
                    break;

                case MirrorTypes::FrameworkData:
                    Serialization::SerializeObject<FrameworkData>(jsonRootObject, m_ProjectData.frameworkData);
                    break;

                case MirrorTypes::UiData:
                    Serialization::SerializeObject<UiData>(jsonRootObject, m_ProjectData.uiData);
                    break;

                case MirrorTypes::ScenesData:
                    Serialization::SerializeObject<ScenesData>(jsonRootObject, m_ProjectData.scenes);
                    break;

                case MirrorTypes::SceneViewerData:
                    Serialization::SerializeObject<SceneViewerData>(jsonRootObject, m_ProjectData.sceneViewerData);
                    break;

                case MirrorTypes::Systems:
                    Serialization::SerializeObject<Systems>(jsonRootObject, m_ProjectData.systems);
                    break;

                case MirrorTypes::ConfiguredGameKeys:
                    Serialization::SerializeObject<ConfiguredGameKeys>(jsonRootObject, m_ProjectData.configuredGameKeys);
                    break;

                default:
                    LOG_ERROR("{0} Unhandled field type {1}(2) for serialization!", __FUNCTION__, projectDataField.type->stringName, (int)projectDataField.type->enumType);
                    break;
                }
            }
        }

        const char* preferencesFilePath = ProjectsFolderPath(null_project);
        PrintRootObjectToFile(preferencesFilePath, jsonRootObject); // #TODO Take in config file path as arg
    }

}
