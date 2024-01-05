#include "QF_ConfigHelper.h"

#include <iostream>
#include <limits.h>
#include <stdlib.h>

#include "../../../Libraries/cJSON_Interface/cJSONInterface.h"

#include "QC_ReflectionMacros.h"

#include "QF_Log.h"
#include "QF_QwerkE_Defines.h"

namespace QwerkE {

    ConfigData ConfigHelper::m_ConfigData = ConfigData();

    void ConfigHelper::LoadConfigData()
    {
        std::string configFilePath = ConfigsFolderPath("preferences.qpref"); // #TODO How to handle file names?
        LoadConfigData(configFilePath);
    }

    void ConfigHelper::LoadConfigData(std::string configFilePath)
    {
        cJSON* root = OpencJSONStream(configFilePath.c_str());
        if (root == nullptr)
        {
            LOG_ERROR("Unable to load json for file {0}", configFilePath);
            return;
        }

        cJSON* framework = GetItemFromRootByKey(root, "Framework");
        m_ConfigData.framework.QuickLoad = (bool)GetItemFromArrayByKey(framework, "QuickLoad")->valueint;
        m_ConfigData.framework.MaxConcurrentThreadCount = GetItemFromArrayByKey(framework, "MaxConcurrentThreadCount")->valueint;

        cJSON* libraries = GetItemFromRootByKey(root, "Libraries");
        m_ConfigData.libraries.Audio = GetItemFromArrayByKey(libraries, "Audio")->valuestring;
        m_ConfigData.libraries.Networking = GetItemFromArrayByKey(libraries, "Networking")->valuestring;
        m_ConfigData.libraries.Physics = GetItemFromArrayByKey(libraries, "Physics")->valuestring;
        m_ConfigData.libraries.Rendering = GetItemFromArrayByKey(libraries, "Rendering")->valuestring;
        m_ConfigData.libraries.Window = GetItemFromArrayByKey(libraries, "Window")->valuestring;

        cJSON* scenes = GetItemFromRootByKey(root, "Scenes");
        std::vector<cJSON*> sceneList = GetAllItemsFromArray(scenes);

        for (unsigned int i = 0; i < sceneList.size(); i++)
        {
            m_ConfigData.scenes.fileNames.push_back(sceneList.at(i)->valuestring);
        }

        cJSON* sceneSettings = GetItemFromRootByKey(root, "SceneSettings");
        m_ConfigData.sceneSettings.MaxEnabledScenes = GetItemFromArrayByKey(sceneSettings, "MaxEnabledScenes")->valueint;

        cJSON* systems = GetItemFromRootByKey(root, "Systems");
        m_ConfigData.systems.AudioEnabled = (bool)GetItemFromArrayByKey(systems, "AudioEnabled")->valueint;
        m_ConfigData.systems.NetworkingEnabled = (bool)GetItemFromArrayByKey(systems, "NetworkingEnabled")->valueint;
        m_ConfigData.systems.PhysicsEnabled = (bool)GetItemFromArrayByKey(systems, "PhysicsEnabled")->valueint;
        m_ConfigData.systems.ConsoleOutputWindowEnabled = (bool)GetItemFromArrayByKey(systems, "ConsoleOutputWindowEnabled")->valueint;

        cJSON* engineSettings = GetItemFromRootByKey(root, "EngineSettings");
        m_ConfigData.engineSettings.LimitFramerate = (bool)GetItemFromArrayByKey(engineSettings, "LimitFramerate")->valueint;
        m_ConfigData.engineSettings.MaxFramesPerSecond = GetItemFromArrayByKey(engineSettings, "MaxFramesPerSecond")->valueint;

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

        cJSON* framework = CreateArray("Framework");
        AddItemToArray(framework, CreateNumber("QuickLoad", (int)m_ConfigData.framework.QuickLoad));
        AddItemToArray(framework, CreateNumber("MaxConcurrentThreadCount", m_ConfigData.framework.MaxConcurrentThreadCount));
        AddItemToRoot(root, framework);

        cJSON* libraries = CreateArray("Libraries");
        AddItemToArray(libraries, CreateString("Audio", m_ConfigData.libraries.Audio.c_str()));
        AddItemToArray(libraries, CreateString("Networking", m_ConfigData.libraries.Networking.c_str()));
        AddItemToArray(libraries, CreateString("Physics", m_ConfigData.libraries.Physics.c_str()));
        AddItemToArray(libraries, CreateString("Rendering", m_ConfigData.libraries.Rendering.c_str()));
        AddItemToArray(libraries, CreateString("Window", m_ConfigData.libraries.Window.c_str()));
        AddItemToRoot(root, libraries);

        cJSON* scenes = CreateArray("Scenes");
        for (unsigned int i = 0; i < m_ConfigData.scenes.fileNames.size(); i++)
        {
            AddItemToArray(scenes, CreateString(std::to_string(i).c_str(), m_ConfigData.scenes.fileNames[i].c_str()));
        }
        AddItemToRoot(root, scenes);

        cJSON* sceneSettings = CreateArray("SceneSettings");
        AddItemToArray(sceneSettings, CreateNumber("MaxEnabledScenes", m_ConfigData.sceneSettings.MaxEnabledScenes));
        AddItemToRoot(root, sceneSettings);

        cJSON* systems = CreateArray("Systems");
        AddItemToArray(systems, CreateBool("AudioEnabled", m_ConfigData.systems.AudioEnabled));
        AddItemToArray(systems, CreateBool("NetworkingEnabled", m_ConfigData.systems.NetworkingEnabled));
        AddItemToArray(systems, CreateBool("PhysicsEnabled", m_ConfigData.systems.PhysicsEnabled));
        AddItemToArray(systems, CreateBool("ConsoleOutputWindowEnabled", m_ConfigData.systems.ConsoleOutputWindowEnabled));
        AddItemToRoot(root, systems);

        cJSON* engineSettings = CreateArray("EngineSettings");
        AddItemToArray(engineSettings, CreateBool("LimitFramerate", m_ConfigData.engineSettings.LimitFramerate));
        AddItemToArray(engineSettings, CreateNumber("MaxFramesPerSecond", (int)m_ConfigData.engineSettings.MaxFramesPerSecond));

        std::string path = ConfigsFolderPath("preferences.qpref");
        PrintRootObjectToFile(path.c_str(), root);
    }

}
