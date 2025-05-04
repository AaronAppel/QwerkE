#pragma once

namespace QwerkE {

    enum QKeyState : u8
    {
        e_KeyStateUp = 0,
        e_KeyStateDown,
        e_KeyStateAny
    };

    enum QMouseButton : u8
    {
		// #TODO Force use for mouse related calls?
    };

    enum QGamepadButton : u8
    {
		// #TODO Force use for gamepad related calls?
    };

    enum QKey : u8
    {
        // #NOTE
        // Standard English US/UK keyboard layout assumed.
        // Comments added for IDE hover context info.
        // Macro keys currently unsupported.
        // Entries assigned using char/ASCII values are order dependent.

        e_MouseButton1 = 0,             // Mouse device button 1
        e_MouseButton2,                 // Mouse device button 2
        e_MouseButton3,                 // Mouse device button 3
        e_MouseButton4,                 // Mouse device button 4
        e_MouseButton5,                 // Mouse device button 5
        e_MouseButton6,                 // Mouse device button 6
        e_MouseButton7,                 // Mouse device button 7
        e_MouseButton8,                 // Mouse device button 8

        e_Gamepad0,                     // Gamepad device button 0
        e_Gamepad1,                     // Gamepad device button 1
        e_Gamepad2,                     // Gamepad device button 2
        e_Gamepad3,                     // Gamepad device button 3
        e_Gamepad4,                     // Gamepad device button 4
        e_Gamepad5,                     // Gamepad device button 5
        e_Gamepad6,                     // Gamepad device button 6
        e_Gamepad7,                     // Gamepad device button 7
        e_Gamepad8,                     // Gamepad device button 8
        e_Gamepad9,                     // Gamepad device button 9
        e_Gamepad10,                    // Gamepad device button 10
        e_Gamepad11,                    // Gamepad device button 11
        e_Gamepad12,                    // Gamepad device button 12
        e_Gamepad13,                    // Gamepad device button 13
        e_Gamepad14,                    // Gamepad device button 14
        e_Gamepad15,                    // Gamepad device button 15

        e_ShiftL,                       // Left Shift key
        e_CtrlL,                        // Left CTRL or Control key
        e_AltL,                         // Left ALT or Alt key
        e_SystemL,                      // Left System key, like Windows or Win (Microsoft), command or cmd (Apple)
        e_ShiftR,                       // Right Shift key
        e_CtrlR,                        // Right CTRL or Control key
        e_AltR,                         // Right ALT or Alt key
        e_SystemR,                      // Right System key, like Windows or Win (Microsoft), command or cmd (Apple)

        e_Space = ' ',                  // Spacebar key

        e_MediaPlay,                    // Play or pause media key
        e_MediaNext,                    // Next or skip forward media key
        e_MediaPrevious,                // Previous or skip back media key
        e_MediaVolumeUp,                // Volume up media key
        e_MediaVolumeDown,              // Volume down media key
        e_MediaVolumeMute,              // Volume mute media key

        e_Apostrophe = '\'',            // ''' or '"' key

        e_Right,                        // Right arrow key
        e_Left,                         // Left arrow key
        e_Down,                         // Down arrow key
        e_Up,                           // Up arrow key

        e_Comma = ',',                  // ',' or '<' key
        e_Minus,                        // '-' or '_' key
        e_Period,                       // '.' or '>' key
        e_SlashForward,                 // '/' or '?' key

        e_0 = '0',                      // '0' or ')' key
        e_1,                            // '1' or '!' key
        e_2,                            // '2' or '@' key
        e_3,                            // '3' or '#' key
        e_4,                            // '4' or '$' key
        e_5,                            // '5' or '%' key
        e_6,                            // '6' or '^' key
        e_7,                            // '7' or '&' key
        e_8,                            // '8' or '*' key
        e_9,                            // '9' or '(' key

        e_Any,                          // Any key. Used in functions to return true for any key event

        e_SemiColon = ';',              // ';' or ':' key

        e_Function,                     // Function or Fn key

        e_Equal = '=',                  // '=' or '+' key

        e_CtrlAny,                      // Left or right CTRL or control key
        e_ShiftAny,                     // Left or right Shift key
        e_AltAny,                       // Left or right Alt or alt key

        e_A = 'A',                      // 'A' or 'a' key
        e_B,                            // 'B' or 'b' key
        e_C,                            // 'C' or 'c' key
        e_D,                            // 'D' or 'd' key
        e_E,                            // 'E' or 'e' key
        e_F,                            // 'F' or 'f' key
        e_G,                            // 'G' or 'g' key
        e_H,                            // 'H' or 'h' key
        e_I,                            // 'I' or 'i' key
        e_J,                            // 'J' or 'j' key
        e_K,                            // 'K' or 'k' key
        e_L,                            // 'L' or 'l' key
        e_M,                            // 'M' or 'm' key
        e_N,                            // 'N' or 'n' key
        e_O,                            // 'O' or 'o' key
        e_P,                            // 'P' or 'p' key
        e_Q,                            // 'Q' or 'q' key
        e_R,                            // 'R' or 'r' key
        e_S,                            // 'S' or 's' key
        e_T,                            // 'T' or 't' key
        e_U,                            // 'U' or 'u' key
        e_V,                            // 'V' or 'v' key
        e_W,                            // 'W' or 'w' key
        e_X,                            // 'X' or 'x' key
        e_Y,                            // 'Y' or 'y' key
        e_Z,                            // 'Z' or 'z' key

        e_BracketLeft = '[',            // '[' or '{' key
        e_BackSlash,                    // '\' or '|' key
        e_BracketRight,                 // ']' or '}' key

        e_Menu,                         // Menu or context or right click key

        e_Escape,                       // Escape key

        e_Tilde = '`',                  // '`' or '~' key

        e_Enter,                        // Enter or Return key
        e_Tab,                          // Tab key
        e_Backspace,                    // Backspace key
        e_Insert,                       // Insert key
        e_Delete,                       // Delete key

        e_PageUp,                       // Page Up or PgUp key
        e_PageDown,                     // Page Down or PgDn key
        e_Home,                         // Home key
        e_End,                          // End key

        e_CapsLock,                     // Caps Lock or Caps key
        e_ScrollLock,                   // Scroll Lock or ScrLk key
        e_NumLock,                      // Number Lock or NumLock key
        e_PrintScreen,                  // Print Screen or PrtSc key
        e_Pause,                        // Pause key

        e_F1,                           // F1 function key
        e_F2,                           // F2 function key
        e_F3,                           // F3 function key
        e_F4,                           // F4 function key
        e_F5,                           // F5 function key
        e_F6,                           // F6 function key
        e_F7,                           // F7 function key
        e_F8,                           // F8 function key
        e_F9,                           // F9 function key
        e_F10,                          // F10 function key
        e_F11,                          // F11 function key
        e_F12,                          // F12 function key
        e_F13,                          // F13 function key
        e_F14,                          // F14 function key
        e_F15,                          // F15 function key
        e_F16,                          // F16 function key
        e_F17,                          // F17 function key
        e_F18,                          // F18 function key
        e_F19,                          // F19 function key
        e_F20,                          // F20 function key
        e_F21,                          // F21 function key
        e_F22,                          // F22 function key
        e_F23,                          // F23 function key
        e_F24,                          // F24 function key
        e_F25,                          // F25 function key

        e_Pad_0,                        // '0' or Ins number pad key
        e_Pad_1,                        // '1' or End number pad key
        e_Pad_2,                        // '2' or Down number pad key
        e_Pad_3,                        // '3' or PgDn number pad key
        e_Pad_4,                        // '4' or Left number pad key
        e_Pad_5,                        // '5' number pad key
        e_Pad_6,                        // '6' or Right number pad key
        e_Pad_7,                        // '7' or Home number pad key
        e_Pad_8,                        // '8' or Up number pad key
        e_Pad_9,                        // '9' or PgUp number pad key
        e_Pad_Decimal,                  // '.' or Del number pad key
        e_Pad_Division,                 // '/' number pad key
        e_Pad_Multiply,                 // '*' number pad key
        e_Pad_Minus,                    // '-' number pad key
        e_Pad_Plus,                     // '+' number pad key
        e_Pad_Return,                   // Enter or Return number pad key
        e_Pad_Equal,                    // '=' number pad key

        e_ScrollUp,                     // Navigate up like a mouse scroll up
        e_ScrollDown,                   // Navigate down like a mouse scroll down
        e_ScrollLeft,                   // Navigate left like a mouse scroll left
        e_ScrollRight,                  // Navigate right like a mouse scroll right

        e_MAX,                          // Invalid entry and end of enum

        // Aliases
        e_MouseLeft = e_MouseButton1,   // Left mouse button
        e_MouseRight = e_MouseButton2,  // Right mouse button
        e_MouseMiddle = e_MouseButton3, // Middle mouse button

        e_GamepadA = e_Gamepad0,        // A controller button
        e_GamepadB,                     // B controller button
        e_GamepadX,                     // X controller button
        e_GamepadY,                     // Y controller button
        e_GamepadYBumperLeft,           // Left shoulder controller button
        e_GamepadYBumperRight,          // Right shoulder controller button
        e_GamepadBack,                  // Back controller button
        e_GamepadStart,                 // Start controller button
        e_GamepadGuide,                 // Guide or center or branded controller button
        e_GamepadStickLeft,             // Left thumb stick controller click or button
        e_GamepadStickRight,            // Right thumb stick controller click or button
        e_GamepadPadUp,                 // Up directional pad controller button
        e_GamepadPadRight,              // Right directional pad controller button
        e_GamepadPadDown,               // Down directional pad controller button
        e_GamepadPadLeft,               // Left directional pad controller button

        e_GamepadCross = e_Gamepad0,    // Cross or X Playstation controller button
        e_GamepadCircle,                // Circle or O Playstation controller button
        e_GamepadSquare,                // Square Playstation controller button
        e_GamepadTriangle,              // Triangle Playstation controller button
    };

    // #TODO Override math operators so QKey+int doesn't convert to an int and require static_cast<QKey>(key + int) everywhere
    // #TODO QKey operator+(QKey g, int t) { g = static_cast<QKey>(g + t); return g; }
    // #TODO QKey operator+(int t, QKey g) { return g + t; }
    // #TODO Refactor standard naming
    // QKey operator++(QKey g) { g = static_cast<QKey>(g + 1); return g; }
    // QKey operator++(QKey g, int t) { g = static_cast<QKey>(g + t); return g; }

}
