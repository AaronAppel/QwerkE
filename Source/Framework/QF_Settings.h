#pragma once

namespace QwerkE {

    struct EngineSettings // #TODO Rename to FrameworkSettings
    {
        bool isDirty = false; // #TODO Use Editor state flag

        u16 windowWidthPixels = 1920;
        u16 windowHeightPixels = 1080;

        u16 windowOpenPositionX = 200;
        u16 windowOpenPositionY = 200;

        bool limitFramerate = true;
        u16 maxFramesPerSecond = 120;
        u16 maxAllowedFramesPerSecond = 360;

        u8 maxEnabledScenes = 1;
        u8 maxJobsAdditionalThreadCount = 1;

        bool consoleOutputWindowEnabled = true; // #TODO Prevent console from opening if ConsoleOutputWindowEnabled is false

        bool vSyncEnabled = false;
        bool windowAutoFocusOnStart = false;

        // #TODO Support
        // bool windowStartFullScreen = false;
    };

    namespace Settings {

        void LoadEngineSettings(const std::string& engineSettingsFilePath); // #TODO Write and chain with above overloaded method
        void SaveEngineSettings(const std::string& engineSettingsFileName);
        void SaveEngineSettings();

        EngineSettings& GetEngineSettings();

    }

}
