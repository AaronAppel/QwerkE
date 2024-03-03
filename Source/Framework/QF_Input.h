#pragma once

// Reference: https://www.gamedev.net/blogs/entry/2250186-designing-a-robust-input-handling-system-for-games/

#include "QF_Defines.h"

#ifdef GLFW3
#include "Libraries/glew/glew.h" // #TODO Review GLEW inclusion
#include "Libraries/glfw/glfw3.h"
#else
#error Define library!
#endif

#include "QF_Enums.h"
#include "QF_eKeys.h"
#include "QF_Defines.h"

namespace QwerkE {

    // TODO: Provide cross platform callback API for convenience
    // TODO: Add input functions for non-GLFW projects
    // TODO: Add input query functions such as WNDPROC for getting input and CheckInput() game loop function.
    // TODO: Key event callback registration. Should that be handled by the event system? probably...

    namespace Input {

        using OnKeyEventCallback = std::function<void(eKeys key, eKeyState state)>;

        void Initialize();

        void NewFrame(); // #Note Call before libraries each frame

        bool FrameKeyAction(eKeys key, eKeyState state);

        vec2 GetMouseDragDelta();

        bool GetIsKeyDown(eKeys key);

        void OnMouseMove(vec2 position);
        void OnMouseMove(float x, float y);
        void OnMouseClick(eKeys key, eKeyState state);
        void OnMouseScroll(float x, float y);

        const vec2& MouseScrollDelta();

        void OnKeyEvent(eKeys key, eKeyState state);

        void RegisterOnKeyEvent(eKeys key, OnKeyEventCallback callback);
        void UnregisterOnKeyEvent(eKeys key, OnKeyEventCallback callback, u8 id = 0); // #TODO Use an id to unregister

        void ToggleLogKeyEvents();

#ifdef GLFW3
        eKeys GLFWToQwerkEKey(int key);
        void SetupCallbacks(GLFWwindow* window);
        void InitializeGLFWKeysCodex(u16* keysCodex);
#else
#error "Define input library!"
#endif

    }

}
