
#include "QE_EditorHotKeys.h"

namespace QwerkE {

	namespace Editor {

        // #TODO Use code as default values but load from data at startup and user load hotkeys request
        std::unordered_map<EditorHotKeys, Input::InputMapping> s_EditorHotkeys = {

            { e_CloseApplication,           Input::InputMapping(std::vector<QKey>{ QKey::e_Escape }) },
            { e_RestartApplication,         Input::InputMapping(std::vector<QKey>{ QKey::e_CtrlAny, QKey::e_R }) },
            { e_HideEditorUi,               Input::InputMapping(std::vector<QKey>{ QKey::e_CtrlAny, QKey::e_U }) },

            { e_WindowStackPanelOpen,       Input::InputMapping(std::vector<QKey>{ QKey::e_CtrlAny, QKey::e_Tab }) },

            { e_Scene_Select1,              Input::InputMapping(std::vector<QKey>{ QKey::e_F1 }) },
            { e_Scene_Select2,              Input::InputMapping(std::vector<QKey>{ QKey::e_F2 }) },
            { e_Scene_Select3,              Input::InputMapping(std::vector<QKey>{ QKey::e_F3 }) },
            { e_Scene_Select4,              Input::InputMapping(std::vector<QKey>{ QKey::e_F4 }) },
            { e_Scene_Select5,              Input::InputMapping(std::vector<QKey>{ QKey::e_F5 }) },
            { e_Scene_Select6,              Input::InputMapping(std::vector<QKey>{ QKey::e_F6 }) },
            { e_Scene_Select7,              Input::InputMapping(std::vector<QKey>{ QKey::e_F7 }) },
            { e_Scene_Select8,              Input::InputMapping(std::vector<QKey>{ QKey::e_F8 }) },
            { e_Scene_Select9,              Input::InputMapping(std::vector<QKey>{ QKey::e_F9 }) },
            { e_Scene_Select10,             Input::InputMapping(std::vector<QKey>{ QKey::e_F10 }) },
            { e_Scene_Select11,             Input::InputMapping(std::vector<QKey>{ QKey::e_F11 }) },
            { e_Scene_Select12,             Input::InputMapping(std::vector<QKey>{ QKey::e_F12 }) },

            { e_SceneGraphToggleActive,     Input::InputMapping(std::vector<QKey>{ QKey::e_CtrlAny, QKey::e_D }) },
        };

        void PollHotkeys()
        {
            // Global application commands
            u8 start = 0;
            u16 end = EditorHotKeys::e_EditorHotKeysMax; // #TODO Values are not sequential/contiguous

            for (size_t i = start; i < end - start; i++)
            {
                // s_EditorHotkeys[(EditorHotKeys)i].Poll();
            }

            s_EditorHotkeys[e_CloseApplication].Poll();
            s_EditorHotkeys[e_RestartApplication].Poll();
            s_EditorHotkeys[e_HideEditorUi].Poll();

            // Windows
            s_EditorHotkeys[e_WindowStackPanelOpen].Poll();
            // e_WindowStackPanelCycleNext,
            // e_WindowStackPanelCyclePrevious,

            // Scenes
            constexpr size_t numberOfHotkeyedScenes = EditorHotKeys::e_Scene_SelectMax - EditorHotKeys::e_Scene_Select1;
            for (u8 i = 0; i < numberOfHotkeyedScenes; i++)
            {
                EditorHotKeys hotKey = static_cast<EditorHotKeys>(EditorHotKeys::e_Scene_Select1 + i);
                s_EditorHotkeys[hotKey].Poll();
            }

            // Scene graph window // #TODO Should each window check its own hotkeys?
            s_EditorHotkeys[e_SceneGraphToggleActive].Poll();
        }

        void HotkeyPoll(const EditorHotKeys a_EditorHotKey)
        {
            if (s_EditorHotkeys.find(a_EditorHotKey) != s_EditorHotkeys.end())
            {
                s_EditorHotkeys[a_EditorHotKey].Poll();
            }
            LOG_WARN("Unsupported EditorHotKeys value: {0}", a_EditorHotKey);
        }

        bool IsHotkeyActive(const EditorHotKeys a_EditorHotKey)
        {
            if (s_EditorHotkeys.find(a_EditorHotKey) != s_EditorHotkeys.end())
            {
                return s_EditorHotkeys[a_EditorHotKey].IsActive();
            }
            LOG_WARN("Unsupported EditorHotKeys value: {0}", a_EditorHotKey);
            return false;
        }

        bool IsHotkeyPressed(const EditorHotKeys a_EditorHotKey)
        {
            if (s_EditorHotkeys.find(a_EditorHotKey) != s_EditorHotkeys.end())
            {
                return s_EditorHotkeys[a_EditorHotKey].Pressed();
            }
            LOG_WARN("Unsupported EditorHotKeys value: {0}", a_EditorHotKey);
            return false;
        }

        bool IsHotkeyReleased(const EditorHotKeys a_EditorHotKey)
        {
            if (s_EditorHotkeys.find(a_EditorHotKey) != s_EditorHotkeys.end())
            {
                return s_EditorHotkeys[a_EditorHotKey].Released();
            }
            LOG_WARN("Unsupported EditorHotKeys value: {0}", a_EditorHotKey);
            return false;
        }

	}

}
