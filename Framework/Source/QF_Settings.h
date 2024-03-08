#pragma once

#include <string>
#include <vector>

#include "QF_Constants.h"
#include "QF_eKeys.h"

namespace QwerkE {

    struct ProjectSettings
    {
        bool isDirty = false;

        const char* projectName = Constants::gc_DefaultStringValue;
        const char* assetsDirPath = Constants::gc_DefaultStringValue;

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
        bool isDirty = false;

        u16 windowWidthPixels = 1600;
        u16 windowHeightPixels = 900;

        bool limitFramerate = true;
        u16 maxFramesPerSecond = 120;

        u8 maxEnabledScenes = 1;
        u8 maxJobsAdditionalThreadCount = 1;

        bool audioEnabled = false;
        bool consoleOutputWindowEnabled = true; // #TODO Prevent console from opening if ConsoleOutputWindowEnabled is false

        // Editor UI
        bool showingSchematicsEditor = false;
        bool showingExampleWindow = false;
        bool showingShaderEditor = false;
        bool showingSettingsEditor = false;
        bool showingFPS = false;
        bool showingRendererSettings = false;
    };

    struct UserSettings
    {
        bool isDirty = false;

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

    struct RendererSettings
    {
        bool isDirty = false;

        u8 drawingPrimitiveType = 0; // GL_ZERO
    };

    namespace Settings
    {
        void LoadEngineSettings(); // #TODO Load from file path. Do not rely on default paths, but instead generate the default path
        void LoadEngineSettings(std::string engineSettingsFilePath); // #TODO Write and chain with above overloaded method
        void SaveEngineSettings();

        void LoadProjectSettings(std::string projectSettingsFilePath);
        void SaveProjectSettings();
        void SaveProjectSettings(std::string projectSettingsFilePath);

        void LoadUserSettings(std::string userSettingsFilePath);
        void SaveUserSettings();

        void LoadRendererSettings(std::string rendererSettingsFilePath);
        void SaveRendererSettings();

        EngineSettings& GetEngineSettings();
        ProjectSettings& GetProjectSettings();
        UserSettings& GetUserSettings();
        RendererSettings& GetRendererSettings();
    }

}
