#pragma once

#include <string>
#include <vector>

#include "QF_QKey.h"
#include "QF_Framework.h"

namespace QwerkE {

    struct EditorSettings
    {
        bool isDirty = false; // #TODO Use Editor state flag

        bool showWelcomeWindow = true;

        Framework::StartUpData startUpData;

        bool limitFramerate = true;
        u16 maxFramesPerSecond = 120;
        u16 maxAllowedFramesPerSecond = 360;

        u8 maxEnabledScenes = 1;
        u8 maxJobsAdditionalThreadCount = 1;

        bool startInPlayMode = true;

        // #TODO Support
        // bool windowStartFullScreen = false;
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

        void LoadEditorSettings();
        void SaveEditorSettings();

        void LoadRendererSettings(const std::string& rendererSettingsFileName);
        void SaveRendererSettings();

        EditorSettings& GetEditorSettings();
        RendererSettings& GetRendererSettings();

    }

}
