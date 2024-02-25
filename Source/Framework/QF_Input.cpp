#include "QF_Input.h"

#include <map>

#include "QF_Enums.h"
#include "QF_Keyboard.h"
#include "QF_Mouse.h"
#include "QF_Window.h"

namespace QwerkE {

    namespace Input {

        static int g_NumPlayers = 1;

        static bool m_KeyEventsAreDirty = true; // TODO: Wipe at init?
        static u16 m_InputEventKeys[ONE_FRAME_MAX_INPUT];
        static bool m_InputEventValues[ONE_FRAME_MAX_INPUT] = { false };
        u16* s_KeyCodex = nullptr;

        static std::map<eInputDeviceTypes, InputDevice*> m_Devices;
        static void SetupDeviceCustomKeys() {}
        static bool AddDevice(InputDevice* device);

        static eInputDeviceTypes m_DefaultMouse = eInputDeviceTypes::Mouse_Device0;
        static eInputDeviceTypes m_DefaultKeyboard = eInputDeviceTypes::Keyboard_Device0;

        void AssignSystemKeys(InputDevice* device); // TODO: Change to a max keys buffer that only needs to track 10ish keys at a time
        void RaiseInputEvent(eKeys key, eKeyState state);

        bool FrameKeyAction(eKeys key, eKeyState state); // check if key was pressed or released this frame

        // #TODO Support ProcessMouseScroll
        // #TODO Hide public methods for processing input
        void ProcessMouseMove(vec2 position);
        void ProcessMouseMove(float x, float y);
        void ProcessMouseClick(eKeys key, eKeyState state);
        void ProcessKeyEvent(eKeys key, eKeyState state);
        eKeys GLFWToQwerkEKey(int key);

        void Initialize()
        {
    #ifdef GLFW3
            s_KeyCodex = new unsigned short[GLFW_KEY_LAST];
            memset(s_KeyCodex, 0, GLFW_KEY_LAST); // set values to 0

            SetupGLFWKeyCodex();
            SetupCallbacks((GLFWwindow*)Window::GetContext());
    #endif

            Keyboard* keyboard = new Keyboard(eInputDeviceTypes::Keyboard_Device0);
            Mouse* mouse = new Mouse(eInputDeviceTypes::Mouse_Device0);
            AddDevice(keyboard);
            AddDevice(mouse);

            AssignSystemKeys(keyboard);
            AssignSystemKeys(mouse);

            NewFrame(); // Init buffers
        }

        void NewFrame()
        {
            // reset state of each input device
            // TODO: Loop through array to reset all devices
            Mouse* mouse = (Mouse*)m_Devices.find(m_DefaultMouse)->second;
            mouse->NewFrame();

            Keyboard* keyboard = (Keyboard*)m_Devices.find(m_DefaultKeyboard)->second;
            keyboard->NewFrame();

            if (m_KeyEventsAreDirty)
            {
                memset(m_InputEventKeys, eKeys_NULL_KEY, ONE_FRAME_MAX_INPUT * sizeof(short));
                memset(m_InputEventValues, eKeyState::eKeyState_Release, ONE_FRAME_MAX_INPUT * sizeof(bool));
                m_KeyEventsAreDirty = false;
            }
        }

        void ProcessMouseMove(vec2 position)
        {
            Mouse* mouse = (Mouse*)m_Devices.find(m_DefaultMouse)->second;
            mouse->s_MouseFrameDelta = mouse->s_MousePos - position;
            mouse->s_MousePos = position;
        }

        void ProcessMouseMove(float x, float y)
        {
            Mouse* mouse = (Mouse*)m_Devices.find(m_DefaultMouse)->second;
            mouse->s_MouseFrameDelta = mouse->s_MousePos - vec2(x, y);
            mouse->s_MousePos = vec2(x, y);
        }

        void ProcessMouseClick(eKeys key, eKeyState state) // handle mouse clicks
        {
            Mouse* mouse = (Mouse*)m_Devices.find(m_DefaultMouse)->second;
            // TODO:: Handle mouse drag for eKeys_RightClick and eKeys_MiddleClick
            if (key == eKeys_LeftClick && state == eKeyState::eKeyState_Press)
            {
                mouse->s_MouseDragStart = mouse->s_MousePos; // start drag
            }
            else if (key == eKeys_LeftClick && state == eKeyState::eKeyState_Release)
            {
                mouse->s_DragReset = true; // drag ended, reset value next frame so it can be used this frame
            }
            mouse->GetKeyStates()[key] = state; // TODO: Is setting bool to key state an issue?

            RaiseInputEvent(key, state);
        }

        void ProcessKeyEvent(eKeys key, eKeyState state)
        {
            Keyboard* keyboard = (Keyboard*)m_Devices.find(m_DefaultKeyboard)->second;
            keyboard->SetOneFrameBufferDirty();

            RaiseInputEvent(key, state);
            keyboard->RaiseInputEvent(key, state);
            // TODO: Convert key into eKeys range
            keyboard->GetKeyStates()[key] = state;
        }

        vec2 GetMouseDragDelta()
        {
            Mouse* mouse = (Mouse*)m_Devices.find(m_DefaultMouse)->second;
            if (mouse->GetKeyStates()[eKeys::eKeys_LeftClick])
            {
                return mouse->s_MousePos - mouse->s_MouseDragStart;
            }
            return vec2(0.0f, 0.0f);
        }

        bool GetIsKeyDown(eKeys key)
        {
            return m_Devices.find(m_DefaultKeyboard)->second->GetKeyStates()[key];
        }

        bool FrameKeyAction(eKeys key, eKeyState state)
        {
            if (m_InputEventKeys[0] != eKeys::eKeys_NULL_KEY) // was a key even pressed?
            {
                for (int i = 0; i < ONE_FRAME_MAX_INPUT; i++)
                {
                    if (m_InputEventKeys[i] == key)
                        return m_InputEventValues[i] == (bool)state;
                }
            }
            return false;
        }

        void RaiseInputEvent(eKeys key, eKeyState state)
        {
            m_KeyEventsAreDirty = true;

            for (int i = 0; i < ONE_FRAME_MAX_INPUT; i++)
            {
                if (m_InputEventKeys[i] == eKeys_NULL_KEY)
                {
                    m_InputEventKeys[i] = key;
                    m_InputEventValues[i] = state;
                    return;
                }
            }
        }

        bool AddDevice(InputDevice* device)
        {
            if (device && m_Devices.find(device->GetType()) == m_Devices.end())
            {
                AssignSystemKeys(device);
                m_Devices[device->GetType()] = device;
                return true;
            }
            return false;
        }

        void AssignSystemKeys(InputDevice* device)
        {
    #ifdef GLFW3
            //void AssignGLFWKeys(InputDevice * device);
            //void SetupGLFWKeyCodex(); // TODO: Remove
            //eKeys GLFWToQwerkEKey(int key) const
            //{
            //    return (eKeys)m_KeyCodex[key];
            //}
    #endif

    #ifdef GLFW3
            // TODO: Reduce the size of array
            device->Initialize();
            AssignGLFWKeys(device->GetType(), device->GetKeyCodex());

    #elif _QWin32
            AssignWin32Keys();

    #elif _QMac32 // 64
            AssignMacKeys();

    #elif _QLinux32 // 64
            AssignLinuxKeys();

    #else
            // Other Platforms or default layout
            // TODO: Map ascii default layout
            // https://www.asciitable.com/index/asciifull.gif
    #pragma error "Input(): No platform defined! Cannot assign keys!"
    #endif
        }

    }

}
