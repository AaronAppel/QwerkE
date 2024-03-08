#pragma once

// Reference: https://www.gamedev.net/blogs/entry/2250186-designing-a-robust-input-handling-system-for-games/

#include "QF_Enums.h"
#include "QF_eKeys.h"
#include "QF_PathDefines.h"

#ifdef _QGLFW3
struct GLFWwindow;
#endif

namespace QwerkE {

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

        u16 RegisterOnKeyEvent(eKeys key, OnKeyEventCallback callback);
        void UnregisterOnKeyEvent(eKeys key, u16 id);

        void ToggleLogKeyEvents();

        void InitializeKeysCodex(u16* keysCodex);

#ifdef _QGLFW3
        eKeys GLFWToQwerkEKey(int key);
        void SetupCallbacks(GLFWwindow* window);
#else
#error "Define window/input library!"
#endif

    }

}
