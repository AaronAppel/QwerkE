#pragma once

#include <string>
#include <vector>

#include "QF_eKeys.h"

namespace QwerkE {

    struct EngineSettings // #TODO Rename to FrameworkSettings
    {
        bool isDirty = false; // #TODO Make editor state and remove from framework

        u16 windowWidthPixels = 1920;
        u16 windowHeightPixels = 1080;

        bool limitFramerate = true;
        u16 maxFramesPerSecond = 120;
        u16 defaultMaxFramesPerSecond = 360;

        u8 maxEnabledScenes = 1;
        u8 maxJobsAdditionalThreadCount = 1;

        bool consoleOutputWindowEnabled = true; // #TODO Prevent console from opening if ConsoleOutputWindowEnabled is false

        bool vSyncEnabled = false;
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

        const char* lastOpenedProjectFileName = Constants::gc_DefaultStringValue;

        // Interface: Window open, UI scaling, etc
    };

    struct RendererSettings
    {
        bool isDirty = false;

        u8 drawingPrimitiveType = 0; // GL_ZERO
    };

    namespace Settings
    {
        const char* GetStyleFileName();

        void LoadEngineSettings(const std::string& engineSettingsFilePath); // #TODO Write and chain with above overloaded method
        void SaveEngineSettings();

        void LoadUserSettings(const std::string& userSettingsFilePath);
        void SaveUserSettings();

        void LoadRendererSettings(const std::string& rendererSettingsFilePath);
        void SaveRendererSettings();

        EngineSettings& GetEngineSettings();
        UserSettings& GetUserSettings();
        RendererSettings& GetRendererSettings();

    }

}
