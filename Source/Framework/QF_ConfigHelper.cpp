#include "QF_ConfigHelper.h"

#include <iostream>
#include <limits.h>
#include <stdlib.h>

#include "Libraries/cJSON/QC_cJSON.h"

#include "QC_Reflection.h"

#include "QF_Defines.h"
#include "QF_Log.h"
#include "QF_Input.h"

namespace QwerkE {

    ConfigData ConfigHelper::m_ConfigData = ConfigData();

    void ConfigHelper::LoadConfigData()
    {
        const char* configFilePath = ConfigsFolderPath(null_config); // #TODO How to handle file names?
        LoadConfigData(configFilePath);
    }

    void ConfigHelper::LoadConfigData(std::string configFilePath)
    {
        // #TODO Dependencies on proper data is fragile. At least have exception handling with more info, or find another way of ensuring proper data
        const cJSON* root = OpencJSONStream(configFilePath.c_str());
        if (root == nullptr)
        {
            LOG_ERROR("Unable to load json for file {0}", configFilePath);
            return;
        }

        if (const cJSON* framework = GetItemFromRootByKey(root, "Framework"))
        {
            m_ConfigData.framework.QuickLoad = (bool)GetItemFromArrayByKey(framework, "QuickLoad")->valueint;
            m_ConfigData.framework.MaxConcurrentThreadCount = GetItemFromArrayByKey(framework, "MaxConcurrentThreadCount")->valueint;
        }

        if (const cJSON* libraries = GetItemFromRootByKey(root, "Libraries"))
        {
            m_ConfigData.libraries.Audio = GetItemFromArrayByKey(libraries, "Audio")->valuestring;
            m_ConfigData.libraries.Networking = GetItemFromArrayByKey(libraries, "Networking")->valuestring;
            m_ConfigData.libraries.Physics = GetItemFromArrayByKey(libraries, "Physics")->valuestring;
            m_ConfigData.libraries.Rendering = GetItemFromArrayByKey(libraries, "Rendering")->valuestring;
            m_ConfigData.libraries.Window = GetItemFromArrayByKey(libraries, "Window")->valuestring;
        }

        if (const cJSON* scenes = GetItemFromRootByKey(root, "Scenes"))
        {
            const std::vector<cJSON*> sceneList = GetAllItemsFromArray(scenes);
            for (unsigned int i = 0; i < sceneList.size(); i++)
            {
                m_ConfigData.scenes.fileNames.push_back(sceneList.at(i)->valuestring);
            }
        }

        if (const cJSON* sceneSettings = GetItemFromRootByKey(root, "SceneSettings"))
        {
            m_ConfigData.sceneSettings.MaxEnabledScenes = GetItemFromArrayByKey(sceneSettings, "MaxEnabledScenes")->valueint;
        }

        if (const cJSON* systems = GetItemFromRootByKey(root, "Systems"))
        {
            m_ConfigData.systems.AudioEnabled = (bool)GetItemFromArrayByKey(systems, "AudioEnabled")->valueint;
            m_ConfigData.systems.NetworkingEnabled = (bool)GetItemFromArrayByKey(systems, "NetworkingEnabled")->valueint;
            m_ConfigData.systems.PhysicsEnabled = (bool)GetItemFromArrayByKey(systems, "PhysicsEnabled")->valueint;
            m_ConfigData.systems.ConsoleOutputWindowEnabled = (bool)GetItemFromArrayByKey(systems, "ConsoleOutputWindowEnabled")->valueint;
        }

        if (const cJSON* engineSettings = GetItemFromRootByKey(root, "EngineSettings"))
        {
            m_ConfigData.engineSettings.LimitFramerate = (bool)GetItemFromArrayByKey(engineSettings, "LimitFramerate")->valueint;
            m_ConfigData.engineSettings.MaxFramesPerSecond = GetItemFromArrayByKey(engineSettings, "MaxFramesPerSecond")->valueint;
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

        if (const cJSON* controls = GetItemFromRootByKey(root, "Controls"))
        {
            // #TODO Avoid string to char to eKeys casting
            // #TODO Remember case insensitivity as well with input keys
            m_ConfigData.controls.Camera_MoveForward = (eKeys)GetItemFromArrayByKey(controls, FULL_VARIABLE_TO_STRING(m_ConfigData.controls.Camera_MoveForward))->valuestring[0];
            m_ConfigData.controls.Camera_MoveBackward = (eKeys)GetItemFromArrayByKey(controls, FULL_VARIABLE_TO_STRING(m_ConfigData.controls.Camera_MoveBackward))->valuestring[0];
            m_ConfigData.controls.Camera_MoveLeft = (eKeys)GetItemFromArrayByKey(controls, FULL_VARIABLE_TO_STRING(m_ConfigData.controls.Camera_MoveLeft))->valuestring[0];
            m_ConfigData.controls.Camera_MoveRight = (eKeys)GetItemFromArrayByKey(controls, FULL_VARIABLE_TO_STRING(m_ConfigData.controls.Camera_MoveRight))->valuestring[0];
            m_ConfigData.controls.Camera_MoveUp = (eKeys)GetItemFromArrayByKey(controls, FULL_VARIABLE_TO_STRING(m_ConfigData.controls.Camera_MoveUp))->valuestring[0];
            m_ConfigData.controls.Camera_MoveDown = (eKeys)GetItemFromArrayByKey(controls, FULL_VARIABLE_TO_STRING(m_ConfigData.controls.Camera_MoveDown))->valuestring[0];
            m_ConfigData.controls.Camera_RotateLeft = (eKeys)GetItemFromArrayByKey(controls, FULL_VARIABLE_TO_STRING(m_ConfigData.controls.Camera_RotateLeft))->valuestring[0];
            m_ConfigData.controls.Camera_RotateRight = (eKeys)GetItemFromArrayByKey(controls, FULL_VARIABLE_TO_STRING(m_ConfigData.controls.Camera_RotateRight))->valuestring[0];
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
        cJSON* root = CreateObject();

        if (cJSON* framework = CreateArray("Framework"))
        {
            AddItemToArray(framework, CreateNumber("QuickLoad", (int)m_ConfigData.framework.QuickLoad));
            AddItemToArray(framework, CreateNumber("MaxConcurrentThreadCount", m_ConfigData.framework.MaxConcurrentThreadCount));
            AddItemToRoot(root, framework);
        }

        if (cJSON* libraries = CreateArray("Libraries"))
        {
            AddItemToArray(libraries, CreateString("Audio", m_ConfigData.libraries.Audio.c_str()));
            AddItemToArray(libraries, CreateString("Networking", m_ConfigData.libraries.Networking.c_str()));
            AddItemToArray(libraries, CreateString("Physics", m_ConfigData.libraries.Physics.c_str()));
            AddItemToArray(libraries, CreateString("Rendering", m_ConfigData.libraries.Rendering.c_str()));
            AddItemToArray(libraries, CreateString("Window", m_ConfigData.libraries.Window.c_str()));
            AddItemToRoot(root, libraries);
        }

        if (cJSON* scenes = CreateArray("Scenes"))
        {
            for (unsigned int i = 0; i < m_ConfigData.scenes.fileNames.size(); i++)
            {
                AddItemToArray(scenes, CreateString(std::to_string(i).c_str(), m_ConfigData.scenes.fileNames[i].c_str()));
            }
            AddItemToRoot(root, scenes);
        }

        if (cJSON* sceneSettings = CreateArray("SceneSettings"))
        {
            AddItemToArray(sceneSettings, CreateNumber("MaxEnabledScenes", m_ConfigData.sceneSettings.MaxEnabledScenes));
            AddItemToRoot(root, sceneSettings);
        }

        if (cJSON* systems = CreateArray("Systems"))
        {
            AddItemToArray(systems, CreateBool("AudioEnabled", m_ConfigData.systems.AudioEnabled));
            AddItemToArray(systems, CreateBool("NetworkingEnabled", m_ConfigData.systems.NetworkingEnabled));
            AddItemToArray(systems, CreateBool("PhysicsEnabled", m_ConfigData.systems.PhysicsEnabled));
            AddItemToArray(systems, CreateBool("ConsoleOutputWindowEnabled", m_ConfigData.systems.ConsoleOutputWindowEnabled));
            AddItemToRoot(root, systems);
        }

        if (cJSON* engineSettings = CreateArray("EngineSettings"))
        {
            AddItemToArray(engineSettings, CreateBool("LimitFramerate", m_ConfigData.engineSettings.LimitFramerate));
            AddItemToArray(engineSettings, CreateNumber("MaxFramesPerSecond", (int)m_ConfigData.engineSettings.MaxFramesPerSecond));
            AddItemToRoot(root, engineSettings);
        }

        const char* configFilePath = ConfigsFolderPath(null_config);
        PrintRootObjectToFile(configFilePath, root); // #TODO Take in config file path as arg
    }

    void ConfigHelper::SaveUsersData()
    {
        cJSON* root = CreateObject();

        if (cJSON* controls = CreateArray("Controls"))
        {
            AddItemToArray(controls, CreateString(FULL_VARIABLE_TO_STRING(m_ConfigData.controls.Camera_MoveForward), (char*)&m_ConfigData.controls.Camera_MoveForward));
            AddItemToArray(controls, CreateString(FULL_VARIABLE_TO_STRING(m_ConfigData.controls.Camera_MoveBackward), (char*)&m_ConfigData.controls.Camera_MoveBackward));
            AddItemToArray(controls, CreateString(FULL_VARIABLE_TO_STRING(m_ConfigData.controls.Camera_MoveLeft), (char*)&m_ConfigData.controls.Camera_MoveLeft));
            AddItemToArray(controls, CreateString(FULL_VARIABLE_TO_STRING(m_ConfigData.controls.Camera_MoveRight), (char*)&m_ConfigData.controls.Camera_MoveRight));
            AddItemToArray(controls, CreateString(FULL_VARIABLE_TO_STRING(m_ConfigData.controls.Camera_MoveUp), (char*)&m_ConfigData.controls.Camera_MoveUp));
            AddItemToArray(controls, CreateString(FULL_VARIABLE_TO_STRING(m_ConfigData.controls.Camera_MoveDown), (char*)&m_ConfigData.controls.Camera_MoveDown));
            AddItemToArray(controls, CreateString(FULL_VARIABLE_TO_STRING(m_ConfigData.controls.Camera_RotateLeft), (char*)&m_ConfigData.controls.Camera_RotateLeft));
            AddItemToArray(controls, CreateString(FULL_VARIABLE_TO_STRING(m_ConfigData.controls.Camera_RotateRight), (char*)&m_ConfigData.controls.Camera_RotateRight));
            AddItemToRoot(root, controls);
        }

        const char* preferencesFilePath = PreferencesFolderPath(null_preferences);
        PrintRootObjectToFile(preferencesFilePath, root); // #TODO Take in config file path as arg
    }

}
