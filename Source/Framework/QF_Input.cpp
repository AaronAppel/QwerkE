#include "QF_Input.h"

#include <map>
#include <unordered_map>

#include "QF_Debug.h"
#include "QF_Enums.h"
#include "QF_Log.h"
#include "QF_Window.h"

namespace QwerkE {

    namespace Input {

        std::vector<std::pair<eKeys, OnKeyEventCallback>> s_OnKeyEventCallBacks;

#ifdef _QDebug
        bool s_LogKeyEventInfo = false;
#endif

        static const u8 ONE_FRAME_MAX_INPUT = 12;

        static vec2 s_MousePos = vec2(0.f);
        static vec2 s_FrameMouseDelta = vec2(0.f);
        static vec2 s_MouseDragStart = vec2(0.f);
        static bool s_MouseDragReset = false;

        static bool s_KeyEventsAreDirty = true; // #TODO Deprecate and use GLFW state to know key states
        static u16 s_InputEventKeys[ONE_FRAME_MAX_INPUT];
        static bool s_InputEventValues[ONE_FRAME_MAX_INPUT] = { false };

#ifdef GLFW3
        static const u16 s_KeyCodexSize = GLFW_KEY_LAST + 1; // #TODO GLFW dependency
#endif
        static u16* s_KeyCodex = new unsigned short[s_KeyCodexSize];
        static bool s_KeyStates[eKeys_MAX] = { false };

        static vec2 s_FrameMouseScrollOffsets = vec2(0.f);

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
            if (key < 0 || key >= s_KeyCodexSize)
            {
                LOG_ERROR("{0} Key code {1} unsupported!", __FUNCTION__, key);
                return eKeys::eKeys_MAX;
            }
            return (eKeys)s_KeyCodex[key];
        }

        void Initialize()
        {
#ifdef GLFW3
            memset(s_KeyCodex, 0, s_KeyCodexSize);
            SetupCallbacks((GLFWwindow*)Window::GetContext());
            InitializeGLFWKeysCodex(s_KeyCodex);
#else
#error "Define input library!"
#endif

            NewFrame();
        }

        void NewFrame()
        {
            s_FrameMouseScrollOffsets.x = 0.f;
            s_FrameMouseScrollOffsets.y = 0.f;

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
            s_FrameMouseDelta = s_MousePos - position;
            s_MousePos = position;
        }

        void OnMouseMove(float x, float y)
        {
            s_FrameMouseDelta = s_MousePos - vec2(x, y);
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

        void OnMouseScroll(float x, float y)
        {
            s_FrameMouseScrollOffsets.x = x;
            s_FrameMouseScrollOffsets.y = y;
        }

        const vec2& MouseScrollDelta()
        {
            return s_FrameMouseScrollOffsets;
        }

        void RegisterOnKeyEvent(eKeys key, OnKeyEventCallback callback)
        {
            s_OnKeyEventCallBacks.push_back(std::make_pair(key, callback));
        }

        void UnregisterOnKeyEvent(eKeys key, OnKeyEventCallback callback)
        {
            for (size_t i = 0; i < s_OnKeyEventCallBacks.size(); i++)
            {
                auto pair = s_OnKeyEventCallBacks[i];
                if (pair.first == key)
                {
                    const bool sameAddress = &pair.second == &callback; // #TODO Compare function addresses properly
                    if (true || sameAddress)
                    {
                        s_OnKeyEventCallBacks.erase(s_OnKeyEventCallBacks.begin() + i);
                        break;
                    }
                }
            }
        }

        void ToggleLogKeyEvents()
        {
#ifdef _QDebug
            s_LogKeyEventInfo = !s_LogKeyEventInfo;
#endif
        }

        void CheckOnKeyEventCallBacks(eKeys key, eKeyState state)
        {
            // Backwards in case a callback unregisters itself after call
            for (int i = s_OnKeyEventCallBacks.size() - 1; i >= 0; i--)
            {
                if (s_OnKeyEventCallBacks[i].first)
                {
                    s_OnKeyEventCallBacks[i].second(key, state);
                }
            }
        }

        void OnKeyEvent(eKeys key, eKeyState state)
        {
#ifdef _QDebug
            if (s_LogKeyEventInfo)
            {
                LOG_TRACE("{0} Key {1} event {2}", __FUNCTION__, (char)key, (int)state);
            }
#endif

            CheckOnKeyEventCallBacks(key, state);

            local_RaiseInputEvent(key, state);

            if (state == eKeyState::eKeyState_Release)
            {
                int bp = 0;
            }
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
