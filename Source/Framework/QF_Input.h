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
    // TODO: Key event callback registration. Should that be handled by the event system? probably...

    namespace Input {

        void Initialize();

        void NewFrame(); // #Note Call before libraries each frame

        bool FrameKeyAction(eKeys key, eKeyState state);

        vec2 GetMouseDragDelta();

        bool GetIsKeyDown(eKeys key);

        void OnMouseMove(vec2 position);
        void OnMouseMove(float x, float y);
        void OnMouseClick(eKeys key, eKeyState state);
        void OnKeyEvent(eKeys key, eKeyState state);

#ifdef GLFW3
        eKeys GLFWToQwerkEKey(int key);
        void SetupCallbacks(GLFWwindow* window);
        void InitializeGLFWKeysCodex(u16* keysCodex);
#else
#error "Define input library!"
#endif

    }

}
