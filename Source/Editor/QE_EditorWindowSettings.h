#pragma once

#ifdef _QENUM
#include "Libraries/enum/QC_enum.h"
#endif

#ifdef _QMIRROR
#include "Libraries/Mirror/Source/MIR_Mirror.h"
#endif

#include "QE_EditorWindow.h"

namespace QwerkE {

	namespace Editor {

        QC_ENUM(eSettingsOptions, u8, Null, EditorSettings, GameActions, Renderer, Project); // #TODO Project isn't settings anymore. Could be reviewed to move
        // #TODO Physics can be another add. Editing collision between layers, global gravity, etc.

        class EditorWindowSettings : public EditorWindow
        {
        public:
            EditorWindowSettings(GUID guid = GUID()) : EditorWindow("Settings", EditorWindowTypes::Settings, guid) { }

        private:
            void DrawInternal() override;

        private:
            MIRROR_PRIVATE_MEMBERS
            eSettingsOptions m_SettingsEditorOption = eSettingsOptions::Null;
            u8 m_LastPopUpIndex = 0;
		};

	}

}