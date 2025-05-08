#pragma once

#include "QF_GameActions.h"
#include "QF_QKey.h"

#include <functional>

namespace QwerkE {

    namespace Input {

        void Initialize();
        void NewFrame();
        void Shutdown();

        bool KeyPressed(const QKey a_Key);
        bool KeyReleased(const QKey a_Key);
        bool KeyDown(const QKey a_Key);

        bool MousePressed(const QKey a_Key);
        bool MouseReleased(const QKey a_Key);
        bool MouseScrolled();
        float MouseScrollDelta();
        bool MouseDown(const QKey a_Key);

        vec2f MousePos();
        bool MouseMoved();
        vec2f MouseDelta();

        bool GamepadPressed(const QGamepad a_Key, const QGamepad a_GamepadId = QGamepad::e_GamepadId0);
        bool GamepadReleased(const QGamepad a_Key, const QGamepad a_GamepadId = QGamepad::e_GamepadId0);
        bool GamepadDown(const QGamepad a_Key, const QGamepad a_GamepadId = QGamepad::e_GamepadId0);

        vec2f GamepadAxis(const int a_AxisIndex, const QGamepad a_GamepadId = QGamepad::e_GamepadId0);
        bool GamepadAxisMoved(const int a_AxisIndex, const QGamepad a_GamepadId = QGamepad::e_GamepadId0);
        const char* const GamepadName(const QGamepad a_Key, const QGamepad a_GamepadId = QGamepad::e_GamepadId0);

        u8 GamepadsCount();
        u8 GamepadButtonCount(const QGamepad a_Key, const QGamepad a_GamepadId = QGamepad::e_GamepadId0);
        u8 GamepadAxesCount(const QGamepad a_Key, const QGamepad a_GamepadId = QGamepad::e_GamepadId0);

        using KeyCallback = std::function<void(QKey a_Key, QKeyState a_State)>;
        void OnKey(const KeyCallback& a_Callback);
        void OnKeyStop(const KeyCallback& a_FuncId);

        using MouseCallback = std::function<void(QKey a_Key, QKeyState a_State, float a_ScrollDelta, const vec2f& a_MousePosition)>;
        void OnMouse(const MouseCallback& a_Callback);
        void OnMouseStop(const MouseCallback& a_FuncId);

        using GamepadCallback = std::function<void(QGamepad a_Input, QKeyState a_State, const vec2f& a_Axis12, const vec2f& a_Axis34, const vec2f& a_Axis56, QGamepad a_GamepadId)>;
        void OnGamepad(const GamepadCallback& a_Callback);
        void OnGamepadStop(const GamepadCallback& a_Func);

#ifdef _QDEBUG
        void DrawDebugWindow();
#endif // _QDEBUG

        // Stage 3 //
        // Devices and non-global or per-device input
        // struct InputDevice
        // {
        //     int id;
        //     int type; // 0 keyboard, 1 mouse, 2 joystick
        //
        //     bool enabled = true; // Disable for cinematics, UI input, window/UI loses focus
        // };

        // u8 DeviceCount();
        // u8 KeyboardCount();
        // u8 MouseCount();
        // u8 GamepadCount();

        // GLFW_JOYSTICK_1 to GLFW_JOYSTICK_16 or GLFW_JOYSTICK_LAST

        // Already defined
        // bool IsMouseDragging() { return false; }
        // vec2f MouseDragDelta() { return {}; }
        // vec2f MouseDragFrameDelta() { return {}; }

        // #NOTE Could be given to things like editor windows that can define a bool to know/call to enable/disable input (isFocused method or bool)
        // InputDevice* Device(int deviceIdOrType) { return 0; } // #TODO Create device for user

        // Stage 4 //
        // More complex input tracking. Handle for users to be more full featured and convenient
        // #NOTE Consuming input can mess with multi frame actions like holding, dragging, double click/press, repeating, counting, etc
        // bool KeyDoubled(int key, bool consume = false) { return false; } // For double click and double button press detection
        // bool KeyRepeating(int key, bool consume = false) { return false; } // #TODO Look into Windows and GLFW key repeat behaviour

        // keyHistorySize=150, keyHistory[keyHistorySize], keyHistoryTime[keyHistorySize]
        // Look through key history buffer for key inputs over a_TimeSeconds. What if many input come in, or a_TimeSeconds is a large value?
        // int KeyPressesInTime(const int a_Key, const float a_TimeSeconds) { return 0; }

        // Time management. May be too niche and add overhead to input system that one or two user calls could manage themselves
        // float KeyStateDuration(int key, bool consume = false) { return 0.f; }
        // Versus
        // float TimeSincePress(int key) { return 0.f; }
        // float TimeSinceRelease(int key) { return 0.f; }

        // Stage 5 //
        // Input mappings and complex input actions
        // struct InputAction
        // {
        //     int id;
        //     char keys[5];
        // };

        GameActions& GetGameActions();
        // bool ActionPressed(const InputAction a_Action);
        // bool ActionReleased(const InputAction a_Action);
        // bool ActionDown(const InputAction a_Action);

        // Maybe fun to have but really just extra. Behaves like registering and unregistering plus counting.
        // Really just to avoid user from managing state, but at what cost?
        // void StartPressCounter(int key) { }
        // u16 GetPressCounter(int key) { return 0; }
        // void StopPressCounter(int key) { }

        // Additional considerations //
        // Input layering, consumption, or otherwise avoiding/ignoring input based on context such as focused windows and gameplay state(s)

    }

}
