#pragma once

#include <string>
#include <vector>

#include "QF_Constants.h"
#include "QF_eKeys.h"

namespace QwerkE {

    struct FrameworkData
    {
        bool QuickLoad = true;
        int MaxConcurrentThreadCount = 10;
    };

    struct Libraries
    {
        std::string Rendering = gc_DefaultStringValue;
        std::string Audio = gc_DefaultStringValue;
        std::string Physics = gc_DefaultStringValue;
        std::string Networking = gc_DefaultStringValue;
        std::string Window = gc_DefaultStringValue;
    };

    struct ScenesList
    {
        std::vector<std::string> fileNames; // #TODO Mirror support for vectors
    };

    struct SceneSettings
    {
        int MaxEnabledScenes = 1;
    };

    struct Systems
    {
        bool AudioEnabled = false;
        bool PhysicsEnabled = false;
        bool NetworkingEnabled = false;
        bool ConsoleOutputWindowEnabled = true; // #TODO Prevent console from opening if ConsoleOutputWindowEnabled is false
    };

    struct EngineSettings
    {
        bool LimitFramerate = true;
        int MaxFramesPerSecond = 120;
    };

    struct Controls
    {
        eKeys Camera_MoveForward = eKeys::eKeys_W;
        eKeys Camera_MoveBackward = eKeys::eKeys_S;
        eKeys Camera_MoveLeft = eKeys::eKeys_A;
        eKeys Camera_MoveRight = eKeys::eKeys_D;
        eKeys Camera_MoveUp = eKeys::eKeys_E;
        eKeys Camera_MoveDown = eKeys::eKeys_Q;
        eKeys Camera_RotateLeft = eKeys::eKeys_R;
        eKeys Camera_RotateRight = eKeys::eKeys_T;
    };

    // #TODO Hide the data for safety. Find a better API for ConfigData
    struct ConfigData
    {
        FrameworkData frameworkData;
        Libraries libraries;
        SceneSettings sceneSettings;
        Systems systems;
        EngineSettings engineSettings;
    };

    struct ConfiguredGameKeys
    {
        eKeys action1 = eKeys::eKeys_MAX; // eKeys::eKeys_K;
        const char* resetScene = gc_DefaultStringValue;
    };

    // TODO Engine data shouldn't be in the framework
    struct SceneViewerData
    {
        int maxEnabledScenes = 1;
        int maxDisplayedScenes = 4;
        int viewDistance = 100;
    };

    struct UserData
    {
        Controls controls;
    };

    struct MenuBarData
    {
        bool showFps = true;
    };

    struct UiData
    {
        MenuBarData menuBarData;
    };

    struct RepeatedStruct // #TODO Remove after testing
    {
        RepeatedStruct(int argValue)
        {
            value = argValue;
        }
        int value = 1;
    };

    struct ProjectData
    {
        const char* projectName = gc_DefaultStringValue;
        const char* assetsRoot = gc_DefaultStringValue;
        FrameworkData frameworkData;
        UiData uiData;
        ScenesList scenes;
        SceneViewerData sceneViewerData;
        Systems systems;
        ConfiguredGameKeys configuredGameKeys;

        RepeatedStruct a = 1; // #TODO Remove after testing
        RepeatedStruct b = 2; // Testing multiple instances of an object.
        RepeatedStruct c = 3; // This will fail to de/serialize using type names instead of unique names
    };

    class ConfigHelper // The config helper is really an engine domain, so maybe move to engine and find another place for level loading logic
    {
    public:
        static void LoadConfigData(); // #TODO Load from file path. Do not rely on default paths, but instead generate the default path
        static void LoadConfigData(std::string configFilePath); // #TODO Write and chain with above overloaded method
        static void SaveConfigData();
        static void SaveConfigData(ConfigData config);

        static void LoadUserData(std::string preferencesFilePath);
        static void SaveUserData();

        static void LoadProjectData(std::string preferencesFilePath);
        static void SaveProjectData();

        static const ConfigData& GetConfigData() { return m_ConfigData; }
        static const UserData& GetUserData() { return m_UserData; }
        static const ProjectData& GetProjectData() { return m_ProjectData; }
        static void SetConfigData(ConfigData config) { m_ConfigData = config; }

    private:
        ConfigHelper() = default;

        static ConfigData m_ConfigData;
        static UserData m_UserData;
        static ProjectData m_ProjectData;
    };

}
