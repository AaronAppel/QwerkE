#include "QE_Input.h"

#include "QF_QKey.h"

// #if WINDOWS
#include <WinUser.h>
// #endif // WINDOWS

namespace QwerkE {

    namespace Input {

        InputContexts s_CurrentContext = e_None;
        std::vector<ContextChangedCallback> s_ContextChangedCallbacks;

        InputContexts CurrentContext()
        {
            return s_CurrentContext;
        }

        void ChangeCurrentContext(const InputContexts a_NewInputContext)
        {
            for (size_t i = 0; i < s_ContextChangedCallbacks.size(); i++)
            {
                s_ContextChangedCallbacks[i](a_NewInputContext);
            }
            s_CurrentContext = a_NewInputContext;
        }

        void OnContextChange(const ContextChangedCallback& a_Callback)
        {
            for (u16 i = 0; i < s_ContextChangedCallbacks.size(); i++)
            {
                if (a_Callback.target_type() == s_ContextChangedCallbacks[i].target_type())
                {
                    LOG_WARN("ContextChangedCallback event already registered!");
                    return;
                }
            }
            s_ContextChangedCallbacks.push_back(a_Callback);
        }

        void StopContextChange(const ContextChangedCallback& a_Callback)
        {
            for (u16 i = 0; i < s_ContextChangedCallbacks.size(); i++)
            {
                if (a_Callback.target_type() == s_ContextChangedCallbacks[i].target_type())
                {
                    s_ContextChangedCallbacks.erase(s_ContextChangedCallbacks.begin() + i);
                    return;
                }
            }
            LOG_WARN("Could not remove ContextChangedCallback event!");
        }

        void SendSystemInput(u16 a_VirtualKeyCode, bool a_KeyDown, u16 a_ScanCode)
        {
#if _QWINDOWS
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
#else
#error "Unsupported platform!"
#endif // _QWINDOWS
        }

        void SendSystemInput(QKey a_Key, bool a_KeyDown)
        {
            ASSERT(false, "Not implemented!");
            u16 keyCode = a_Key; // #TODO Convert QKey to system (virtual) key code
            u16 scanCode = 0;
            SendSystemInput(keyCode, a_KeyDown, scanCode);
        }

    }

}
