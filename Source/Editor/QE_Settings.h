#pragma once

#include <string>
#include <vector>

#include "QF_eKeys.h"

namespace QwerkE {

    struct EngineSettings // #TODO Rename to FrameworkSettings
    {
        bool isDirty = false; // #TODO Use Editor state flag

        u16 windowWidthPixels = 1920;
        u16 windowHeightPixels = 1080;

        bool limitFramerate = true;
        u16 maxFramesPerSecond = 120;
        u16 defaultMaxFramesPerSecond = 360;

        u8 maxEnabledScenes = 1;
        u8 maxJobsAdditionalThreadCount = 1;

        bool consoleOutputWindowStartOpen = true;

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

        void LoadEngineSettings(const std::string& engineSettingsFilePath); // #TODO Write and chain with above overloaded method
        void SaveEngineSettings(const std::string& engineSettingsFileName);
        void SaveEngineSettings();

        void LoadRendererSettings(const std::string& rendererSettingsFilePath);
        void SaveRendererSettings();

        EngineSettings& GetEngineSettings();
        UserSettings& GetUserSettings();
        RendererSettings& GetRendererSettings();

    }

}
