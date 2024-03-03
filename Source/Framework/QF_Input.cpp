#include "QF_Input.h"

#include <map>
#include <unordered_map>

#include "QF_Debug.h"
#include "QF_Enums.h"
#include "QF_Log.h"
#include "QF_Window.h"

namespace QwerkE {

    namespace Input {

        std::vector<std::pair<eKeys, CallbackFunction<OnKeyEventCallback>>> s_OnKeyEventCallBacks2;

        void Test(eKeys key, eKeyState state)
        {
            LOG_INFO("{0} Inside", __FUNCTION__);
        }

        void Test2(eKeys key, eKeyState state)
        {
            LOG_INFO("{0} Inside", __FUNCTION__);
        }

        void Func()
        {
            CallbackFunction callback = CallbackFunction<OnKeyEventCallback>(Test);
            s_OnKeyEventCallBacks2.push_back(std::make_pair(eKeys::eKeys_0, callback));

            CallbackFunction callback2 = CallbackFunction<OnKeyEventCallback>(Test);
            s_OnKeyEventCallBacks2.push_back(std::make_pair(eKeys::eKeys_0, callback2));

            CallbackFunction callback3 = CallbackFunction<OnKeyEventCallback>(Test2);
            s_OnKeyEventCallBacks2.push_back(std::make_pair(eKeys::eKeys_0, callback3));

            for (int i = s_OnKeyEventCallBacks2.size() - 1; i >= 0; i--)
            {
                auto pair = s_OnKeyEventCallBacks2[i];
                if (pair.second.Id() == callback2.Id())
                {
                    s_OnKeyEventCallBacks2.erase(s_OnKeyEventCallBacks2.begin() + i);
                    break;
                }
            }

            OnKeyEventCallback onKeyEventCallback = s_OnKeyEventCallBacks2[0].second.Callback();
            onKeyEventCallback(eKeys::eKeys_0, eKeyState::eKeyState_Max);
        }

        std::vector<std::pair<eKeys, OnKeyEventCallback>> s_OnKeyEventCallBacks;

#ifdef _QDebug
        bool s_LogKeyEventInfo = false;
#endif

        static const u8 ONE_FRAME_MAX_INPUT = 12;

        static vec2 s_MousePos = vec2(0.f);
        static vec2 s_FrameMouseDelta = vec2(0.f);
        static vec2 s_MouseDragStart = vec2(0.f);
        static bool s_MouseDragReset = false;

        static bool s_KeyEventsAreDirty = true;
        static u16 s_FrameKeyEvents[ONE_FRAME_MAX_INPUT];
        static bool s_FrameKeyStates[ONE_FRAME_MAX_INPUT] = { false };

#ifdef GLFW3
        static const u16 s_GlfwKeyCodexSize = GLFW_KEY_LAST + 1;
        static u16* s_GlfwKeyCodex = new unsigned short[s_GlfwKeyCodexSize];
#endif
        static bool s_eKeyStates[eKeys_MAX] = { false };

        static vec2 s_FrameMouseScrollOffsets = vec2(0.f);

        static void local_RaiseInputEvent(eKeys key, eKeyState state)
        {
            s_KeyEventsAreDirty = true;

            for (int i = 0; i < ONE_FRAME_MAX_INPUT; i++)
            {
                if (s_FrameKeyEvents[i] == eKeys_NULL_KEY)
                {
                    s_FrameKeyEvents[i] = key;
                    s_FrameKeyStates[i] = state;
                    return;
                }
            }
        }

        eKeys GLFWToQwerkEKey(int key)
        {
            if (key < 0 || key >= s_GlfwKeyCodexSize)
            {
                LOG_ERROR("{0} Key code {1} unsupported!", __FUNCTION__, key);
                return eKeys::eKeys_MAX;
            }
            return (eKeys)s_GlfwKeyCodex[key];
        }

        void Initialize()
        {
#ifdef GLFW3
            memset(s_GlfwKeyCodex, 0, s_GlfwKeyCodexSize);
            SetupCallbacks((GLFWwindow*)Window::GetContext());
            InitializeGLFWKeysCodex(s_GlfwKeyCodex);
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
                memset(s_FrameKeyEvents, eKeys_NULL_KEY, ONE_FRAME_MAX_INPUT * sizeof(short));
                memset(s_FrameKeyStates, eKeyState::eKeyState_Release, ONE_FRAME_MAX_INPUT * sizeof(bool));
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
            s_eKeyStates[key] = state; // TODO: Is setting bool to key state an issue?

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

        void UnregisterOnKeyEvent(eKeys key, OnKeyEventCallback callback, u8 id)
        {
            // #TODO Find a way to unregister using function pointer references
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
            s_eKeyStates[key] = state;
        }

        vec2 GetMouseDragDelta()
        {
            if (s_eKeyStates[eKeys::eKeys_LeftClick])
            {
                return s_MousePos - s_MouseDragStart;
            }
            return vec2(0.0f, 0.0f);
        }

        bool GetIsKeyDown(eKeys key)
        {
            return s_eKeyStates[key];
        }

        bool FrameKeyAction(eKeys key, eKeyState state)
        {
            if (s_FrameKeyEvents[0] != eKeys::eKeys_NULL_KEY)
            {
                for (int i = 0; i < ONE_FRAME_MAX_INPUT; i++)
                {
                    if (s_FrameKeyEvents[i] == key)
                        return s_FrameKeyStates[i] == (bool)state;
                }
            }
            return false;
        }

    }

}
