#include "QE_Input.h"

#include "QF_QKey.h"

// #if WINDOWS
#include <WinUser.h>
// #endif // WINDOWS

namespace QwerkE {

    namespace Input {

        void SendSystemInput(u16 a_VirtualKeyCode, bool a_KeyDown, u16 a_ScanCode)
        {
            // #if WINDOWS
            // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-sendinput

            INPUT input;
            if (a_VirtualKeyCode <= VK_XBUTTON2)
            {
                // https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-mouseinput
                input.type = INPUT_MOUSE;

                input.mi.dx = 0;
                input.mi.dy = 0;
                input.mi.mouseData = 0;

                switch (a_VirtualKeyCode)
                {
                case VK_LBUTTON:
                    input.mi.dwFlags = a_KeyDown ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_LEFTUP;
                    break;
                case VK_RBUTTON:
                    input.mi.dwFlags = a_KeyDown ? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_RIGHTUP;
                    break;
                // case VK_CANCEL: Control break processing
                case VK_MBUTTON:
                    input.mi.dwFlags = a_KeyDown ? MOUSEEVENTF_MIDDLEDOWN : MOUSEEVENTF_MIDDLEUP;
                    break;
                case VK_XBUTTON1:
                case VK_XBUTTON2:
                    input.mi.dwFlags = a_KeyDown ? MOUSEEVENTF_XDOWN : MOUSEEVENTF_XUP;
                    break;
                // case : input.mi.dwFlags = MOUSEEVENTF_MOVE
                // case : input.mi.dwFlags = MOUSEEVENTF_WHEEL
                default:
                    break;
                }
            }
            else
            {
                input.type = INPUT_KEYBOARD;

                input.ki.time = 0;
                input.ki.dwExtraInfo = 0;
                input.ki.wScan = a_ScanCode; // hardware scan code for key
                input.ki.wVk = a_VirtualKeyCode;

                input.ki.dwFlags = a_KeyDown ? 0 : KEYEVENTF_KEYUP; // 0 for key press
            }

            // input.hi; // Hardware input

            SendInput(1, &input, sizeof(INPUT));

            // #endif // WINDOWS
        }

        void SendSystemInput(QKey a_Key, bool a_KeyDown)
        {
            u16 keyCode = a_Key; // #TODO Convert QKey to system (virtual) key code
            u16 scanCode = 0;
            SendSystemInput(keyCode, a_KeyDown, scanCode);
        }

    }

}
