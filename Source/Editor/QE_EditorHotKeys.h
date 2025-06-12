#pragma once

#include <unordered_map>
#include <vector>

#include "QF_Input.h"
#include "QF_InputMapping.h"

namespace QwerkE {

	namespace Editor {

        // #TODO Hotkeys, or HotKeys
        enum EditorHotKeys : uint16_t // Max 65535
        {
            None = 0,

            // Global application commands
            e_CloseApplication = 1,
            e_RestartApplication,
            e_HideEditorUi,

            // Windows
            e_WindowStackPanelOpen = 1000,
            e_WindowStackPanelCycleNext,
            e_WindowStackPanelCyclePrevious,

            // Scenes
            e_Scene_Select1 = 2000,
            e_Scene_Select2,
            e_Scene_Select3,
            e_Scene_Select4,
            e_Scene_Select5,
            e_Scene_Select6,
            e_Scene_Select7,
            e_Scene_Select8,
            e_Scene_Select9,
            e_Scene_Select10,
            e_Scene_Select11,
            e_Scene_Select12,
            e_Scene_SelectMax,

            // Scene graph window // #TODO Should each window check its own hotkeys?
            e_SceneGraphToggleActive = 3000,

            e_EditorHotKeysMax
        };

        void HotkeyPoll(const EditorHotKeys a_EditorHotKey); // #TODO Shouldn't be exposed externally in the future

        bool IsHotkeyActive(const EditorHotKeys a_EditorHotKey);
        bool IsHotkeyPressed(const EditorHotKeys a_EditorHotKey);
        bool IsHotkeyReleased(const EditorHotKeys a_EditorHotKey);

	}

}