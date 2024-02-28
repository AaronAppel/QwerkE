#include "QF_Input.h"

#include <map>

#include "QF_Debug.h"
#include "QF_Enums.h"
#include "QF_Log.h"
#include "QF_Window.h"

namespace QwerkE {

    namespace Input {

        static const u8 ONE_FRAME_MAX_INPUT = 12;

        static vec2 s_MousePos = vec2(0, 0);
        static vec2 s_MouseFrameDelta = vec2(0, 0);
        static vec2 s_MouseDragStart = vec2(0, 0);
        static bool s_MouseDragReset = false;

        static bool s_KeyEventsAreDirty = true;
        static u16 s_InputEventKeys[ONE_FRAME_MAX_INPUT];
        static bool s_InputEventValues[ONE_FRAME_MAX_INPUT] = { false };

        static u16* s_KeyCodex = new unsigned short[GLFW_KEY_LAST];
        static bool s_KeyStates[eKeys_MAX] = { false };

        static void local_RaiseInputEvent(eKeys key, eKeyState state)
        {
            s_KeyEventsAreDirty = true;

            for (int i = 0; i < ONE_FRAME_MAX_INPUT; i++)
            {
                if (s_InputEventKeys[i] == eKeys_NULL_KEY)
                {
                    s_InputEventKeys[i] = key;
                    s_InputEventValues[i] = state;
                    return;
                }
            }
        }

        eKeys GLFWToQwerkEKey(int key)
        {
            return (eKeys)s_KeyCodex[key];
        }

        void Initialize()
        {
#ifdef GLFW3
            memset(s_KeyCodex, 0, GLFW_KEY_LAST);
            SetupCallbacks((GLFWwindow*)Window::GetContext());
            InitializeGLFWKeysCodex(s_KeyCodex);
#else
#error "Define input library!"
#endif

            NewFrame();
        }

        void NewFrame()
        {
            if (s_MouseDragReset)
            {
                s_MouseDragStart = vec2(0.f);
                s_MouseDragReset = false;
            }

            if (s_KeyEventsAreDirty)
            {
                memset(s_InputEventKeys, eKeys_NULL_KEY, ONE_FRAME_MAX_INPUT * sizeof(short));
                memset(s_InputEventValues, eKeyState::eKeyState_Release, ONE_FRAME_MAX_INPUT * sizeof(bool));
                s_KeyEventsAreDirty = false;
            }
        }

        void OnMouseMove(vec2 position)
        {
            s_MouseFrameDelta = s_MousePos - position;
            s_MousePos = position;
        }

        void OnMouseMove(float x, float y)
        {
            s_MouseFrameDelta = s_MousePos - vec2(x, y);
            s_MousePos = vec2(x, y);
        }

        void OnMouseClick(eKeys key, eKeyState state)
        {
            // TODO:: Handle mouse drag for eKeys_RightClick and eKeys_MiddleClick
            if (key == eKeys_LeftClick && state == eKeyState::eKeyState_Press)
            {
                s_MouseDragStart = s_MousePos; // start drag
            }
            else if (key == eKeys_LeftClick && state == eKeyState::eKeyState_Release)
            {
                s_MouseDragReset = true; // drag ended, reset value next frame so it can be used this frame
            }
            s_KeyStates[key] = state; // TODO: Is setting bool to key state an issue?

            local_RaiseInputEvent(key, state);
        }

        void OnKeyEvent(eKeys key, eKeyState state)
        {
            local_RaiseInputEvent(key, state);
            s_KeyStates[key] = state;
        }

        vec2 GetMouseDragDelta()
        {
            if (s_KeyStates[eKeys::eKeys_LeftClick])
            {
                return s_MousePos - s_MouseDragStart;
            }
            return vec2(0.0f, 0.0f);
        }

        bool GetIsKeyDown(eKeys key)
        {
            return s_KeyStates[key];
        }

        bool FrameKeyAction(eKeys key, eKeyState state)
        {
            if (s_InputEventKeys[0] != eKeys::eKeys_NULL_KEY)
            {
                for (int i = 0; i < ONE_FRAME_MAX_INPUT; i++)
                {
                    if (s_InputEventKeys[i] == key)
                        return s_InputEventValues[i] == (bool)state;
                }
            }
            return false;
        }

    }

}
