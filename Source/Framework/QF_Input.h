#pragma once

// Reference: https://www.gamedev.net/blogs/entry/2250186-designing-a-robust-input-handling-system-for-games/

#include "QF_eKeys.h"
#include "QF_GameActions.h"
#include "QF_PathDefines.h"

#ifdef _QGLFW3
struct GLFWwindow;
#endif

enum ImGuiKey : int;
namespace QwerkE {

    namespace Input {

        using OnKeyEventCallback = std::function<void(eKeys key, eKeyState state)>;

        void Initialize();

        void NewFrame(); // #NOTE Call before libraries each frame

        bool FrameKeyAction(eKeys key, eKeyState state);

        GameActions& GetGameActions();

        const vec2f& MousePosition();
        vec2f MouseDragDelta();
        const vec2f& MouseFrameDelta();
        const vec2f& MouseScrollDelta();

        bool IsKeyDown(eKeys key);

        void OnMouseMove(vec2f position);
        void OnMouseMove(float x, float y);
        void OnMouseClick(eKeys key, eKeyState state);
        void OnMouseScroll(float x, float y);

        void OnKeyEvent(eKeys key, eKeyState state);

        u16 RegisterOnKeyEvent(eKeys key, OnKeyEventCallback callback);
        void UnregisterOnKeyEvent(eKeys key, u16 id);

        void ToggleLogKeyEvents();

#ifdef _QGLFW3
        eKeys GLFWToQwerkEKey(int key);

        ImGuiKey QwerkEKeyToImGui(eKeys key);
        void SetupCallbacks(GLFWwindow* window);
#else
#error "Define window/input library!"
#endif

    }

}
