#pragma once

#include <string>
#include <vector>

#include "QF_eKeys.h"

namespace QwerkE {

    struct FrameworkData
    {
        bool QuickLoad = true;
        int MaxConcurrentThreadCount = 10;
    };

    struct Libraries
    {
        std::string Rendering = "OpenGL";
        std::string Audio = "OpenAL";
        std::string Physics = "Bullet3D";
        std::string Networking = "RakNet";
        std::string Window = "GLFW3";
    };

    struct ScenesData
    {
        std::vector<std::string> fileNames;
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
        bool ConsoleOutputWindowEnabled = true;
    };

    struct EngineSettings
    {
        bool LimitFramerate = true;
        int MaxFramesPerSecond = 120;
    };

    struct Controls
    {
        eKeys Camera_MoveForward = eKeys::eKeys_MAX;
        eKeys Camera_MoveBackward = eKeys::eKeys_MAX;
        eKeys Camera_MoveLeft = eKeys::eKeys_MAX;
        eKeys Camera_MoveRight = eKeys::eKeys_MAX;
        eKeys Camera_MoveUp = eKeys::eKeys_MAX;
        eKeys Camera_MoveDown = eKeys::eKeys_MAX;
        eKeys Camera_RotateLeft = eKeys::eKeys_MAX;
        eKeys Camera_RotateRight = eKeys::eKeys_MAX;
    };

    // #TODO Hide the data for safety. Find a better API for ConfigData

    struct ConfigData
    {
        FrameworkData frameworkData;
        Libraries libraries;
        ScenesData scenesData;
        SceneSettings sceneSettings;
        Systems systems;
        EngineSettings engineSettings;
    };

    struct UserData
    {
        Controls controls;
    };

    class ConfigHelper
    {
    public:
        static void LoadConfigData(); // #TODO Load from file path. Do not rely on default paths, but instead generate the default path
        static void LoadConfigData(std::string configFilePath); // #TODO Write and chain with above overloaded method
        static void SaveConfigData();
        static void SaveConfigData(ConfigData config);
        static void LoadUserData(std::string preferencesFilePath);
        static void SaveUserData();

        static const ConfigData& GetConfigData() { return m_ConfigData; }
        static const UserData& GetUserData() { return m_UserData; }
        static void SetConfigData(ConfigData config) { m_ConfigData = config; }

    private:
        ConfigHelper() = default;

        static ConfigData m_ConfigData;
        static UserData m_UserData;
    };

}
