#pragma once

#include <string>
#include <vector>

#include "QF_eKeys.h"

namespace QwerkE {

    struct UserSettings
    {
        // Interface: Window open, UI layout and scaling, styles, etc
        bool isDirty = false; // #TODO Use Editor state flag
        bool startInPlayMode = true; // #TODO Serialize value

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
        // Interface: Window open, UI layout and scaling, etc
    };

    struct RendererSettings
    {
        bool isDirty = false; // #TODO Use Editor state flag

        u8 drawingPrimitiveType = 0; // GL_ZERO
    };

    namespace Settings
    {
        const char* GetStyleFileName();
        const char* GetStyleFileName2();

        void LoadUserSettings(const std::string& userSettingsFileName);
        void SaveUserSettings();

        void LoadRendererSettings(const std::string& rendererSettingsFileName);
        void SaveRendererSettings();

        UserSettings& GetUserSettings();
        RendererSettings& GetRendererSettings();

    }

}
