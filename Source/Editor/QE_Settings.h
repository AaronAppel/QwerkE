#pragma once

#include <string>
#include <vector>

#include "QF_eKeys.h"

namespace QwerkE {

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

        void LoadRendererSettings(const std::string& rendererSettingsFilePath);
        void SaveRendererSettings();

        UserSettings& GetUserSettings();
        RendererSettings& GetRendererSettings();

    }

}
