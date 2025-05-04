#pragma once

#include <string>
#include <vector>

#include "QF_QKey.h"

namespace QwerkE {

    struct UserSettings
    {
        // Interface: Window open, UI layout and scaling, styles, etc
        bool isDirty = false; // #TODO Use Editor state flag
        bool startInPlayMode = true; // #TODO Serialize value

        // #TODO Key bindings
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
