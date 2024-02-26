#pragma once

// Reference: https://www.gamedev.net/blogs/entry/2250186-designing-a-robust-input-handling-system-for-games/

#include "QF_Defines.h"

#ifdef GLFW3
#include "Libraries/glew/GL/glew.h" // #TODO Review GLEW inclusion
#include "Libraries/glfw/GLFW/glfw3.h"
#else
#error Define library!
#endif

#include "QF_Enums.h"
#include "QF_eKeys.h"
#include "QF_Defines.h"
#include "QC_Vector.h"

namespace QwerkE {

    // TODO: Provide cross platform callback API for convenience
    // TODO: Add input functions for non-GLFW projects
    // TODO: Add input query functions such as WNDPROC for getting input and CheckInput() game loop function.
    // TODO: Think of abstracting class for different libraries and platforms
    // TODO: Key event callback registration. Should that be handled by the event system? probably...
    // TODO: Support more than 1 keyboard or multiple mice... for fun.
    // TODO: Fix GLFW callbacks to avoid "static" everywhere

    namespace Input {

        class KeyboardExtAPI;
        class InputDevice;

        const u8 ONE_FRAME_MAX_INPUT = 12;

        void Initialize();

        void NewFrame(); // Call before other libraries! Resets frame input buffer

        vec2 GetMouseDragDelta(); // get distance from mouse down
        bool GetIsKeyDown(eKeys key); // check if keyIsDown

        const KeyboardExtAPI* GetDeviceAPI();

        bool FrameKeyAction(eKeys key, eKeyState state); // Check if key was pressed or released this frame

        // #TODO Support ProcessMouseScroll
        // #TODO Hide public methods for processing input
        void ProcessMouseMove(vec2 position);
        void ProcessMouseMove(float x, float y);
        void ProcessMouseClick(eKeys key, eKeyState state);
        void ProcessKeyEvent(eKeys key, eKeyState state);
        eKeys GLFWToQwerkEKey(int key);

#ifdef GLFW3
        void SetupCallbacks(GLFWwindow* window);

        void AssignGLFWKeys(eInputDeviceTypes deviceType, u16* deviceKeysCodex);
        void SetupGLFWKeyCodex(); // TODO: Remove
#else
#error "Define input library!"
#endif

    }

}
