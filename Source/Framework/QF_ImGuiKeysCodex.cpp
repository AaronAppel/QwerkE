#include "QF_Input.h"

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QwerkE_imgui.h"
#endif

namespace QwerkE {

    namespace Input {

        void local_ImGuiKeysCodex(u16* keysCodex)
        {
#ifdef _QDEARIMGUI

            // ImGuiKey_None = 0,

            // constexpr u16 s_ImGuiKeyOffset = 512;
            keysCodex[eKeys_Tab] = ImGuiKey_Tab; // = 512, == ImGuiKey_NamedKey_BEGIN

            keysCodex[eKeys_Left] = ImGuiKey_LeftArrow;
            keysCodex[eKeys_Right] = ImGuiKey_RightArrow;
            keysCodex[eKeys_Up] = ImGuiKey_UpArrow;
            keysCodex[eKeys_Down] = ImGuiKey_DownArrow;

            keysCodex[eKeys_PageUp] = ImGuiKey_PageUp;
            keysCodex[eKeys_Down] = ImGuiKey_PageDown;
            keysCodex[eKeys_Home] = ImGuiKey_Home;
            keysCodex[eKeys_End] = ImGuiKey_End;
            keysCodex[eKeys_Insert] = ImGuiKey_Insert;
            keysCodex[eKeys_Delete] = ImGuiKey_Delete;
            keysCodex[eKeys_Backspace] = ImGuiKey_Backspace;
            keysCodex[eKeys_Space] = ImGuiKey_Space;
            keysCodex[eKeys_Return] = ImGuiKey_Enter;
            keysCodex[eKeys_Escape] = ImGuiKey_Escape;

            keysCodex[eKeys_LCTRL] = ImGuiKey_LeftCtrl;
            keysCodex[eKeys_LSHIFT] = ImGuiKey_LeftShift;
            keysCodex[eKeys_LALT] = ImGuiKey_LeftAlt;
            keysCodex[eKeys_LSUPER] = ImGuiKey_LeftSuper;

            keysCodex[eKeys_RCTRL] = ImGuiKey_RightCtrl;
            keysCodex[eKeys_RSHIFT] = ImGuiKey_RightShift;
            keysCodex[eKeys_RALT] = ImGuiKey_RightAlt;
            keysCodex[eKeys_RSUPER] = ImGuiKey_RightSuper;
            keysCodex[eKeys_Menu] = ImGuiKey_Menu;

            keysCodex[eKeys_0] = ImGuiKey_0;
            keysCodex[eKeys_1] = ImGuiKey_1;
            keysCodex[eKeys_2] = ImGuiKey_2;
            keysCodex[eKeys_3] = ImGuiKey_3;
            keysCodex[eKeys_4] = ImGuiKey_4;
            keysCodex[eKeys_5] = ImGuiKey_5;
            keysCodex[eKeys_6] = ImGuiKey_6;
            keysCodex[eKeys_7] = ImGuiKey_7;
            keysCodex[eKeys_8] = ImGuiKey_8;
            keysCodex[eKeys_9] = ImGuiKey_9;

            keysCodex[eKeys_A] = ImGuiKey_A;
            keysCodex[eKeys_B] = ImGuiKey_B;
            keysCodex[eKeys_C] = ImGuiKey_C;
            keysCodex[eKeys_D] = ImGuiKey_D;
            keysCodex[eKeys_E] = ImGuiKey_E;
            keysCodex[eKeys_F] = ImGuiKey_F;
            keysCodex[eKeys_G] = ImGuiKey_G;
            keysCodex[eKeys_H] = ImGuiKey_H;
            keysCodex[eKeys_I] = ImGuiKey_I;
            keysCodex[eKeys_J] = ImGuiKey_J;

            keysCodex[eKeys_K] = ImGuiKey_K;
            keysCodex[eKeys_L] = ImGuiKey_L;
            keysCodex[eKeys_M] = ImGuiKey_M;
            keysCodex[eKeys_N] = ImGuiKey_N;
            keysCodex[eKeys_O] = ImGuiKey_O;
            keysCodex[eKeys_P] = ImGuiKey_P;
            keysCodex[eKeys_Q] = ImGuiKey_Q;
            keysCodex[eKeys_R] = ImGuiKey_R;
            keysCodex[eKeys_S] = ImGuiKey_S;
            keysCodex[eKeys_T] = ImGuiKey_T;
            keysCodex[eKeys_U] = ImGuiKey_U;

            keysCodex[eKeys_U] = ImGuiKey_U;
            keysCodex[eKeys_V] = ImGuiKey_V;
            keysCodex[eKeys_W] = ImGuiKey_W;
            keysCodex[eKeys_X] = ImGuiKey_X;
            keysCodex[eKeys_Y] = ImGuiKey_Y;
            keysCodex[eKeys_Z] = ImGuiKey_Z;

            keysCodex[eKeys_F1]  = ImGuiKey_F1;
            keysCodex[eKeys_F2]  = ImGuiKey_F2;
            keysCodex[eKeys_F3]  = ImGuiKey_F3;
            keysCodex[eKeys_F4]  = ImGuiKey_F4;
            keysCodex[eKeys_F5]  = ImGuiKey_F5;
            keysCodex[eKeys_F6]  = ImGuiKey_F6;
            keysCodex[eKeys_F7]  = ImGuiKey_F7;
            keysCodex[eKeys_F8]  = ImGuiKey_F8;
            keysCodex[eKeys_F9]  = ImGuiKey_F9;
            keysCodex[eKeys_F10] = ImGuiKey_F10;
            keysCodex[eKeys_F11] = ImGuiKey_F11;
            keysCodex[eKeys_F12] = ImGuiKey_F12;

            keysCodex[eKeys_F13] = ImGuiKey_F13;
            keysCodex[eKeys_F14] = ImGuiKey_F14;
            keysCodex[eKeys_F15] = ImGuiKey_F15;
            keysCodex[eKeys_F16] = ImGuiKey_F16;
            keysCodex[eKeys_F17] = ImGuiKey_F17;
            keysCodex[eKeys_F18] = ImGuiKey_F18;
            keysCodex[eKeys_F19] = ImGuiKey_F19;
            keysCodex[eKeys_F20] = ImGuiKey_F20;
            keysCodex[eKeys_F21] = ImGuiKey_F21;
            keysCodex[eKeys_F22] = ImGuiKey_F22;
            keysCodex[eKeys_F23] = ImGuiKey_F23;
            keysCodex[eKeys_F24] = ImGuiKey_F24;

            keysCodex[eKeys_Apostrophe]     = ImGuiKey_Apostrophe;
            keysCodex[eKeys_Comma]          = ImGuiKey_Comma;
            keysCodex[eKeys_Minus]          = ImGuiKey_Minus;
            keysCodex[eKeys_Period]         = ImGuiKey_Period;
            keysCodex[eKeys_ForwardSlash]   = ImGuiKey_Slash;
            keysCodex[eKeys_SemiColon]      = ImGuiKey_Semicolon;

            keysCodex[eKeys_Equal]          = ImGuiKey_Equal;        // =
            keysCodex[eKeys_LeftBracket]    = ImGuiKey_LeftBracket;  // [
            keysCodex[eKeys_BackSlash]      = ImGuiKey_Backslash;    // \ (this text inhibit multiline comment caused by backslash)
            keysCodex[eKeys_RightBracket]   = ImGuiKey_RightBracket; // ]
            keysCodex[eKeys_Tilde]          = ImGuiKey_GraveAccent;  // `

            keysCodex[eKeys_CapsLock]       = ImGuiKey_CapsLock;
            keysCodex[eKeys_ScrollLock]     = ImGuiKey_ScrollLock;
            keysCodex[eKeys_NumLock]        = ImGuiKey_NumLock;
            keysCodex[eKeys_PrintScreen]    = ImGuiKey_PrintScreen;
            keysCodex[eKeys_Pause]          = ImGuiKey_Pause;

            keysCodex[eKeys_Pad_0] = ImGuiKey_Keypad0;
            keysCodex[eKeys_Pad_1] = ImGuiKey_Keypad1;
            keysCodex[eKeys_Pad_2] = ImGuiKey_Keypad2;
            keysCodex[eKeys_Pad_3] = ImGuiKey_Keypad3;
            keysCodex[eKeys_Pad_4] = ImGuiKey_Keypad4;
            keysCodex[eKeys_Pad_5] = ImGuiKey_Keypad5;
            keysCodex[eKeys_Pad_6] = ImGuiKey_Keypad6;
            keysCodex[eKeys_Pad_7] = ImGuiKey_Keypad7;
            keysCodex[eKeys_Pad_8] = ImGuiKey_Keypad8;
            keysCodex[eKeys_Pad_9] = ImGuiKey_Keypad9;

            keysCodex[eKeys_Pad_Decimal] = ImGuiKey_KeypadDecimal;
            keysCodex[eKeys_Pad_Division] = ImGuiKey_KeypadDivide;
            keysCodex[eKeys_Pad_Multiply] = ImGuiKey_KeypadMultiply;
            keysCodex[eKeys_Pad_Minus] = ImGuiKey_KeypadSubtract;
            keysCodex[eKeys_Pad_Plus] = ImGuiKey_KeypadAdd;
            keysCodex[eKeys_Pad_Return] = ImGuiKey_KeypadEnter;
            keysCodex[eKeys_Pad_Equal] = ImGuiKey_KeypadEqual;

            // #TODO Support AppBack/Forward?
            // keysCodex[eKeys_Pad_] = ImGuiKey_AppBack; // Available on some keyboard/mouses. Often referred as "Browser Back"
            // keysCodex[eKeys_Pad_] = ImGuiKey_AppForward;

            // #TODO Support gamepad?
            // Gamepad (some of those are analog values, 0.0f to 1.0f)                          // NAVIGATION ACTION
            // (download controller mapping PNG/PSD at http://dearimgui.com/controls_sheets)
            // ImGuiKey_GamepadStart,          // Menu (Xbox)      + (Switch)   Start/Options (PS)
            // ImGuiKey_GamepadBack,           // View (Xbox)      - (Switch)   Share (PS)
            // ImGuiKey_GamepadFaceLeft,       // X (Xbox)         Y (Switch)   Square (PS)        // Tap: Toggle Menu. Hold: Windowing mode (Focus/Move/Resize windows)
            // ImGuiKey_GamepadFaceRight,      // B (Xbox)         A (Switch)   Circle (PS)        // Cancel / Close / Exit
            // ImGuiKey_GamepadFaceUp,         // Y (Xbox)         X (Switch)   Triangle (PS)      // Text Input / On-screen Keyboard
            // ImGuiKey_GamepadFaceDown,       // A (Xbox)         B (Switch)   Cross (PS)         // Activate / Open / Toggle / Tweak
            // ImGuiKey_GamepadDpadLeft,       // D-pad Left                                       // Move / Tweak / Resize Window (in Windowing mode)
            // ImGuiKey_GamepadDpadRight,      // D-pad Right                                      // Move / Tweak / Resize Window (in Windowing mode)
            // ImGuiKey_GamepadDpadUp,         // D-pad Up                                         // Move / Tweak / Resize Window (in Windowing mode)
            // ImGuiKey_GamepadDpadDown,       // D-pad Down                                       // Move / Tweak / Resize Window (in Windowing mode)
            // ImGuiKey_GamepadL1,             // L Bumper (Xbox)  L (Switch)   L1 (PS)            // Tweak Slower / Focus Previous (in Windowing mode)
            // ImGuiKey_GamepadR1,             // R Bumper (Xbox)  R (Switch)   R1 (PS)            // Tweak Faster / Focus Next (in Windowing mode)
            // ImGuiKey_GamepadL2,             // L Trig. (Xbox)   ZL (Switch)  L2 (PS) [Analog]
            // ImGuiKey_GamepadR2,             // R Trig. (Xbox)   ZR (Switch)  R2 (PS) [Analog]
            // ImGuiKey_GamepadL3,             // L Stick (Xbox)   L3 (Switch)  L3 (PS)
            // ImGuiKey_GamepadR3,             // R Stick (Xbox)   R3 (Switch)  R3 (PS)
            // ImGuiKey_GamepadLStickLeft,     // [Analog]                                         // Move Window (in Windowing mode)
            // ImGuiKey_GamepadLStickRight,    // [Analog]                                         // Move Window (in Windowing mode)
            // ImGuiKey_GamepadLStickUp,       // [Analog]                                         // Move Window (in Windowing mode)
            // ImGuiKey_GamepadLStickDown,     // [Analog]                                         // Move Window (in Windowing mode)
            // ImGuiKey_GamepadRStickLeft,     // [Analog]
            // ImGuiKey_GamepadRStickRight,    // [Analog]
            // ImGuiKey_GamepadRStickUp,       // [Analog]
            // ImGuiKey_GamepadRStickDown,     // [Analog]

            // Aliases: Mouse Buttons (auto-submitted from AddMouseButtonEvent() calls)
            // - This is mirroring the data also written to io.MouseDown[], io.MouseWheel, in a format allowing them to be accessed via standard key API.
            // ImGuiKey_MouseLeft, ImGuiKey_MouseRight, ImGuiKey_MouseMiddle, ImGuiKey_MouseX1, ImGuiKey_MouseX2, ImGuiKey_MouseWheelX, ImGuiKey_MouseWheelY,

            // ImGuiKey_NamedKey_END = ImGuiKey_COUNT,
            // ImGuiKey_NamedKey_COUNT = ImGuiKey_NamedKey_END - ImGuiKey_NamedKey_BEGIN,
#endif
        }

    }

}
