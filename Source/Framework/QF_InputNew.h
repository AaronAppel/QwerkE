#include "QF_Input.h"

#include "QF_QKey.h"

namespace QwerkE {

    namespace Input {

        // #NOTE Example/demo mock up of new input system API

        // Considerations //
        // Input layering, consumption, or otherwise avoiding/ignoring input based on context such as focused windows and gameplay state(s)

        // Stage 1 //
        // Minimal (MVP)
        // Polling API
        void Initialize_New();
        void NewFrame_New();

        bool KeyPressed(const QKey a_Key);
        bool KeyReleased(const QKey a_Key);
        bool KeyDown(const QKey a_Key);

        bool MousePressed(const QKey a_Key);
        bool MouseReleased(const QKey a_Key);
        bool MouseDown(const QKey a_Key);
        vec2u16 MousePos();
        vec2f MouseDelta();

        bool GamepadPressed(const QKey a_Key);
        bool GamepadReleased(const QKey a_Key);
        bool GamepadDown(const QKey a_Key);
        vec2f GamepadAxis(const int a_AxisIndex);

#ifdef _QDEBUG
        void Update(); // #TESTING For input system refactor only
#endif // _QDEBUG

#ifdef _QDEBUG
        void DrawDebugWindow();
#endif // _QDEBUG

        // Stage 2 //
        // Callbacks and events
        // void OnKeyStateChange(int key, void* callback) {} // Register
        // void OnKeyStateChangeStop(int key, void* callback) {} // Unregister
        // or
        // void OnKey(int callback) {} // Any key register
        // void OnKey(int key, void* callback) {} // Specific key register
        // void OnKeyStop(int key) {} // Any key unregister
        // void OnKeyStop(int key, void* callback) {} // Specific key unregister

        // Stage 3 //
        // Devices and non-global or per-device input
        struct InputDevice
        {
            int id;
            int type; // 0 keyboard, 1 mouse, 2 joystick

            bool enabled = true; // Disable for cinematics, UI input, window/UI loses focus
        };

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
        struct InputAction
        {
            int id;
            char keys[5];
        };

        // bool ActionPressed(const InputAction a_Action);
        // bool ActionReleased(const InputAction a_Action);
        // bool ActionDown(const InputAction a_Action);

        // Maybe fun to have but really just extra. Behaves like registering and unregistering plus counting.
        // Really just to avoid user from managing state, but at what cost?
        // void StartPressCounter(int key) { }
        // u16 GetPressCounter(int key) { return 0; }
        // void StopPressCounter(int key) { }

    }

}
