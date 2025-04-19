#include "QF_Input.h"

#include <map>
#include <unordered_map>

#ifdef _QGLFW3
#include "Libraries/glfw/glfw3.h"
#endif

#ifdef _QDEARIMGUI
#include "Libraries/imgui/QwerkE_imgui.h"
#endif

#include "QC_CallbackFunction.h"

#include "QF_Enums.h"
#include "QF_Log.h"
#include "QF_Paths.h"
#include "QF_Serialize.h"
#include "QF_Window.h"

namespace QwerkE {

    namespace Input {

        constexpr char* s_GameActionsFileName = "GameActions.qdata";
        static GameActions s_GameActions;

        std::vector<std::pair<eKeys, CallbackFunction<OnKeyEventCallback>>> s_OnKeyEventCallBacks;

#ifdef _QDEBUG
        bool s_LogKeyEventInfo = false;
#endif

        static const u8 ONE_FRAME_MAX_INPUT = 12;

        static vec2f s_MousePos = vec2f(.0f);
        static vec2f s_FrameMouseDelta = vec2f(.0f);
        static vec2f s_MouseDragStart = vec2f(.0f);
        static bool s_MouseDragReset = false;

        static bool s_KeyEventsAreDirty = true;
        static u16 s_FrameKeyEvents[ONE_FRAME_MAX_INPUT];
        static bool s_FrameKeyStates[ONE_FRAME_MAX_INPUT] = { false };

#ifdef _QGLFW3
        static const u16 s_GlfwKeyCodexSize = GLFW_KEY_LAST + 1;
        static u16* s_GlfwKeyCodex = new unsigned short[s_GlfwKeyCodexSize];
#endif

#ifdef _QDEARIMGUI
        static u16* s_ImGuiKeyCodex = new unsigned short[eKeys_MAX];
#endif

        // #TODO Review using GLFW state IF QwerkE::Window can provide a nice un/register API
        // Windows currently needs to callback to Input. Might as well register callbacks once and avoid the extra logic in Windows.
        static bool s_eKeyStates[eKeys_MAX] = { false };

        static vec2f s_FrameMouseScrollOffsets = vec2f(.0f);

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

#ifdef _QGLFW3
        eKeys GLFWToQwerkEKey(int key)
        {
            if (key < 0 || key >= s_GlfwKeyCodexSize)
            {
                LOG_ERROR("{0} Key code {1} unsupported!", __FUNCTION__, key);
                return eKeys::eKeys_MAX;
            }
            return (eKeys)s_GlfwKeyCodex[key];
        }
#endif

#ifdef _QDEARIMGUI
        ImGuiKey QwerkEKeyToImGui(eKeys key)
        {
            return (ImGuiKey)s_ImGuiKeyCodex[key];
        }
#endif

        void local_GlfwKeysCodex(u16* keysCodex);
        void local_ImGuiKeysCodex(u16* keysCodex);

        void Initialize()
        {
            // KeyCallback kc = &OnKeyEvent;
            // Window::RegisterKeyCallback(&kc);

#ifdef _QGLFW3
            memset(s_GlfwKeyCodex, 0, s_GlfwKeyCodexSize);
            SetupCallbacks((GLFWwindow*)Window::GetContext());
            local_GlfwKeysCodex(s_GlfwKeyCodex);
            local_ImGuiKeysCodex(s_ImGuiKeyCodex);
#else
#error "Define input library!"
#endif
            Serialize::FromFile(Paths::Setting(s_GameActionsFileName).c_str(), s_GameActions, true);

            NewFrame();
        }

        void NewFrame()
        {
            s_FrameMouseScrollOffsets.x = 0.f;
            s_FrameMouseScrollOffsets.y = 0.f;

            if (s_MouseDragReset)
            {
                s_MouseDragStart = vec2f(0.f);
                s_MouseDragReset = false;
            }

            if (s_KeyEventsAreDirty)
            {
                memset(s_FrameKeyEvents, eKeys_NULL_KEY, ONE_FRAME_MAX_INPUT * sizeof(short));
                memset(s_FrameKeyStates, eKeyState::eKeyState_Release, ONE_FRAME_MAX_INPUT * sizeof(bool));
                s_KeyEventsAreDirty = false;
            }
        }

        void OnMouseMove(vec2f position)
        {
            s_FrameMouseDelta = s_MousePos - position;
            s_MousePos = position;
        }

        void OnMouseMove(float x, float y)
        {
            s_FrameMouseDelta = s_MousePos - vec2f(x, y);
            s_MousePos = vec2f(x, y);
        }

        void OnMouseClick(eKeys key, eKeyState state)
        {
            // TODO:: Handle mouse drag for eKeys_RightClick and eKeys_MiddleClick
            if ((key == eKeys_MouseButton1 || key == eKeys_MouseButton2) &&
                state == eKeyState::eKeyState_Press)
            {
                s_MouseDragStart = s_MousePos; // start drag
            }
            else if ((key == eKeys_MouseButton1 || key == eKeys_MouseButton2) &&
                state == eKeyState::eKeyState_Release)
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

        void OnJoystickEvent(int joystickId, int eventId)
        {
            if (GLFW_CONNECTED == eventId)
            {
                // The joystick was connected
            }
            else if (GLFW_DISCONNECTED == eventId)
            {
                // The joystick was disconnected
            }
        }

        bool IsJoystickButtonDown(eKeys key)
        {
            int count;
            if (const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count))
            {
                return GLFW_PRESS && buttons[GLFW_GAMEPAD_BUTTON_A];
            }

            return false;
        }

        const vec2f& MouseFrameDelta()
        {
            return s_FrameMouseDelta;
        }

        const vec2f& MouseScrollDelta()
        {
            return s_FrameMouseScrollOffsets;
        }

        u16 RegisterOnKeyEvent(eKeys key, OnKeyEventCallback callback)
        {
            s_OnKeyEventCallBacks.emplace_back(std::make_pair(key, callback));
            return s_OnKeyEventCallBacks.back().second.Id();
        }

        void UnregisterOnKeyEvent(eKeys key, u16 id)
        {
            for (size_t i = 0; i < s_OnKeyEventCallBacks.size(); i++)
            {
                auto pair = s_OnKeyEventCallBacks[i];
                if (pair.first == key && pair.second.Id() == id)
                {
                    s_OnKeyEventCallBacks.erase(s_OnKeyEventCallBacks.begin() + i);
                    return;
                }
            }
        }

        void ToggleLogKeyEvents()
        {
#ifdef _QDEBUG
            s_LogKeyEventInfo = !s_LogKeyEventInfo;
#endif
        }

        void CheckOnKeyEventCallBacks(eKeys key, eKeyState state)
        {
            for (int i = (int)s_OnKeyEventCallBacks.size() - 1; i >= 0; i--)
            {
                if (s_OnKeyEventCallBacks[i].first)
                {
                    s_OnKeyEventCallBacks[i].second.Callback()(key, state);
                }
            }
        }

        void OnKeyEvent(eKeys key, eKeyState state)
        {
#ifdef _QDEBUG
            if (s_LogKeyEventInfo)
            {
                LOG_TRACE("{0} Key {1} event {2}", __FUNCTION__, (char)key, (int)state);
            }
#endif

            CheckOnKeyEventCallBacks(key, state);
            local_RaiseInputEvent(key, state);
            s_eKeyStates[key] = state;
        }

        const vec2f& MousePosition()
        {
            return s_MousePos;
        }

        vec2f MouseDragDelta()
        {
            if (s_eKeyStates[eKeys::eKeys_LeftClick])
            {
                return s_MousePos - s_MouseDragStart;
            }
            return vec2f(.0f, .0f);
        }

        bool IsKeyDown(eKeys key)
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

        GameActions& GetGameActions()
        {
            return s_GameActions;
        }

    }

}
