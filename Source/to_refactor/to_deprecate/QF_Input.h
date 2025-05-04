#pragma once

// Reference: https://www.gamedev.net/blogs/entry/2250186-designing-a-robust-input-handling-system-for-games/
// And the code: https://github.com/grimtraveller/scribblings-by-apoch/tree/master/inputmapping

#include "QF_eKeys.h"
#include "QF_GameActions.h"
#include "QF_PathDefines.h"

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

        void OnJoystickEvent(int joystickId, int eventId);

        bool IsJoystickButtonDown(eKeys key);

        [[nodiscard]] u16 RegisterOnKeyEvent(eKeys key, OnKeyEventCallback callback);
        void UnregisterOnKeyEvent(eKeys key, u16 id);

        void ToggleLogKeyEvents();

    }

}
