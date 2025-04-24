#pragma once

#ifdef _QENUM
#include "Libraries/enum/QC_enum.h"
#endif

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

        QC_ENUM(eSettingsOptions, u8, Null, Engine, GameActions, UserSettings, Renderer, Project); // #TODO Project isn't settings anymore. Could be reviewed to move
        // #TODO Physics can be another add. Editing collision between layers, global gravity, etc.

        class EditorWindowSettings : public EditorWindow
        {
        public:
            EditorWindowSettings(GUID guid = GUID()) : EditorWindow("Settings", EditorWindowTypes::Settings, guid) { }

        private:
            void DrawInternal() override;

        private:
            eSettingsOptions m_SettingsEditorOption = eSettingsOptions::Null;
            s8 m_LastPopUpIndex = -1;
		};

	}

}