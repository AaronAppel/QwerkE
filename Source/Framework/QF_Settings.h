#pragma once

#include <string>
#include <vector>

#include "QF_Constants.h"
#include "QF_eKeys.h"

namespace QwerkE {

    struct ProjectSettings
    {
        const char* projectName = gc_DefaultStringValue;
        const char* assetsDirPath = gc_DefaultStringValue;

        // Referenced Scenes
        std::vector<std::string> sceneFileNames;

        // struct ConfiguredGameKeys
        // {
        //     eKeys action1 = eKeys::eKeys_MAX; // eKeys::eKeys_K;
        //     const char* resetScene = gc_DefaultStringValue;
        // };
    };

    struct EngineSettings
    {
        int windowWidthPixels = 1600; // #TODO Support unsigned short
        int windowHeightPixels = 900;

        bool showFps = true;
        bool limitFramerate = true;
        int maxFramesPerSecond = 120;

        int maxEnabledScenes = 1;
        int maxConcurrentThreadCount = 1;

        bool audioEnabled = false;
        bool physicsEnabled = false;
        bool networkingEnabled = false;
        bool consoleOutputWindowEnabled = true; // #TODO Prevent console from opening if ConsoleOutputWindowEnabled is false
    };

    struct UserSettings
    {
        eKeys key_camera_MoveForward = eKeys::eKeys_W;
        eKeys key_camera_MoveBackward = eKeys::eKeys_S;
        eKeys key_camera_MoveLeft = eKeys::eKeys_A;
        eKeys key_camera_MoveRight = eKeys::eKeys_D;
        eKeys key_camera_MoveUp = eKeys::eKeys_E;
        eKeys key_camera_MoveDown = eKeys::eKeys_Q;
        eKeys key_camera_RotateLeft = eKeys::eKeys_R;
        eKeys key_camera_RotateRight = eKeys::eKeys_T;

        // Interface: Window open, UI scaling, etc
    };

    // TODO Engine data shouldn't be in the framework
    struct SceneViewerData
    {
        int maxEnabledScenes = 1;
        int maxDisplayedScenes = 4;
        int viewDistance = 100;
    };

    class Settings // The config helper is really an engine domain, so maybe move to engine and find another place for level loading logic
    {
    public:
        static void LoadEngineSettings(); // #TODO Load from file path. Do not rely on default paths, but instead generate the default path
        static void LoadEngineSettings(std::string engineSettingsFilePath); // #TODO Write and chain with above overloaded method
        static void SaveEngineSettings();
        static void SaveEngineSettings(const EngineSettings& engineSettings);

        static void LoadProjectSettings(std::string projectSettingsFilePath);
        static void SaveProjectSettings();

        static void LoadUserSettings(std::string userSettingsFilePath);
        static void SaveUserSettings();

        static const EngineSettings& GetEngineSettings() { return m_engineSettings; }
        static const ProjectSettings& GetProjectSettings() { return m_projectSettings; }
        static const UserSettings& GetUserSettings() { return m_userSettings; }

    private:
        Settings() = default;

        static EngineSettings m_engineSettings;
        static ProjectSettings m_projectSettings;
        static UserSettings m_userSettings;
    };

}
