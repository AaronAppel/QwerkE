#include "QE_Input.h"

#include "QF_QKey.h"

// #if WINDOWS
#include <WinUser.h>
// #endif // WINDOWS

namespace QwerkE {

    namespace Input {

        void SendSystemInput(u16 a_VirtualKeyCode, bool a_KeyDown, u16 a_ScanCode)
        {
            // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-sendinput
            // #if WINDOWS

            INPUT input;

            if (a_VirtualKeyCode <= VK_XBUTTON2)
            {
                // https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-mouseinput
                input.type = INPUT_MOUSE;

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
                // case : input.mi.dwFlags = MOUSEEVENTF_HWHEEL
                default:
                    break;
                }
            }
            else
            {
                input.type = INPUT_KEYBOARD;

                input.ki.wScan = a_ScanCode; // hardware scan code for key
                input.ki.time = 0;
                input.ki.dwExtraInfo = 0;
                input.ki.wVk = a_VirtualKeyCode;
            }

            if (a_KeyDown)
            {
                input.ki.dwFlags = 0; // 0 for key press
                input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
            }
            else
            {
                input.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
                input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
            }

            SendInput(1, &input, sizeof(INPUT));

            // #endif // WINDOWS
        }

        void SendSystemInput(QKey a_Key, bool a_KeyDown)
        {
            // #if WINDOWS

            INPUT input;
            input.type = INPUT_KEYBOARD;
            input.ki.wScan = 0; // hardware scan code for key
            input.ki.time = 0;
            input.ki.dwExtraInfo = 0;
            // input.ki.wVk = 0x41; // virtual-key code for the "A" key

            // #TODO Map QwerkE keys to Windows key codes
            // input.ki.wVk = QkeyToWindowsKey(a_Key);

            if (a_KeyDown)
            {
                input.ki.dwFlags = 0; // 0 for key press
            }
            else
            {
                input.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
            }

            SendInput(1, &input, sizeof(INPUT));

            // #endif // WINDOWS
        }

    }

}
