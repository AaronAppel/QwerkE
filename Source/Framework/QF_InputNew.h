#include "QF_Input.h"

namespace QwerkE {

    namespace Input {

        // #NOTE Example/demo mock up of new input system API

        struct InputDevice
        {
            int id;
            int type; // 0 keyboard, 1 mouse, 2 joystick

            bool enabled = true; // Disable for cinematics, UI input, window/UI loses focus
        };

        // #NOTE Consuming input can mess with multi frame actions like holding, dragging, double click/press, repeating, counting, etc
        bool KeyDown(int key, bool consume = false) { }
        bool KeyUp(int key, bool consume = false) { }
        bool KeyState(int key, bool consume = false) { }
        bool KeyDoubled(int key, bool consume = false) { } // For double click and double button press detection
        bool KeyRepeating(int key, bool consume = false) { } // #TODO Look into Windows and GLFW key repeat behaviour

        void OnKey(int callback) { }
        void OnKeyStop(int callback) { }

        // Time management. May be too niche and add overhead to input system that one or two user calls could manage themselves
        float KeyStateDuration(int key, bool consume = false) { }
        // Versus
        float TimeSincePress(int key) { }
        float TimeSinceRelease(int key) { }

        void StartPressCounter(int key) { }
        u16 GetPressCounter(int key) { }
        void StopPressCounter(int key) { }

        bool IsMouseDragging() { }
        vec2f MouseDragDelta() { }
        vec2f MouseDragFrameDelta() { }

        // #NOTE Could be given to things like editor windows that can define a bool to know/call to enable/disable input (isFocused method or bool)
        InputDevice* Device(int deviceIdOrType) { } // #TODO Create device for user

    }

}
