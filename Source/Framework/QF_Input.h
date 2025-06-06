#pragma once

#include <functional>

#include "QC_TypeDefs.h"

#include "QF_GameActions.h"
#include "QF_QKey.h"

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
        bool MouseDown(const QKey a_Key);

        bool MouseScrolled();
        float MouseScrollDelta();

        vec2f MousePos();
        bool MouseMoved();
        vec2f MouseDelta();

        bool GamepadPressed(const QGamepad a_Key, const QGamepad a_GamepadId = QGamepad::e_GamepadId0);
        bool GamepadReleased(const QGamepad a_Key, const QGamepad a_GamepadId = QGamepad::e_GamepadId0);
        bool GamepadDown(const QGamepad a_Key, const QGamepad a_GamepadId = QGamepad::e_GamepadId0);

        vec2f GamepadAxis(const int a_AxisIndex, const QGamepad a_GamepadId = QGamepad::e_GamepadId0);
        bool GamepadAxisMoved(const int a_AxisIndex, const QGamepad a_GamepadId = QGamepad::e_GamepadId0);

        u8 GamepadButtonCount(const QGamepad a_Key, const QGamepad a_GamepadId = QGamepad::e_GamepadId0);
        u8 GamepadAxesCount(const QGamepad a_Key, const QGamepad a_GamepadId = QGamepad::e_GamepadId0);

        u8 GamepadsCount();
        const char* const GamepadName(const QGamepad a_Key, const QGamepad a_GamepadId = QGamepad::e_GamepadId0);

        using KeyCallback = std::function<void(QKey a_Key, QKeyState a_State)>;
        void OnKey(const KeyCallback& a_Callback);
        void OnKeyStop(const KeyCallback& a_FuncId);

        using MouseCallback = std::function<void(QKey a_Key, QKeyState a_State, float a_ScrollDelta, const vec2f& a_MousePosition)>;
        void OnMouse(const MouseCallback& a_Callback);
        void OnMouseStop(const MouseCallback& a_FuncId);

        using GamepadCallback = std::function<void(QGamepad a_Input, QKeyState a_State, const vec2f& a_Axis12, const vec2f& a_Axis34, const vec2f& a_Axis56, QGamepad a_GamepadId)>;
        void OnGamepad(const GamepadCallback& a_Callback);
        void OnGamepadStop(const GamepadCallback& a_Func);

        GameActions& GetGameActions();

#ifdef _QDEBUG
        void DrawDebugWindow();
#endif // _QDEBUG

        // bool ActionPressed(const InputAction a_Action);
        // bool ActionReleased(const InputAction a_Action);
        // bool ActionDown(const InputAction a_Action);

        // Stage 3 //
        // Clean up standard API, and look at adding input mapping and contextual input handling

        // Additional considerations //
        // Input layering, consumption, or otherwise avoiding/ignoring input based on context such as focused windows and gameplay state(s)
        // Example contexts: Editor mode start scene button can then become the stop scene button when the context change to play mode.
        // Same button/key combo, different context-dependent action.

        // EXPERIMENTAL InputHandler class (mapping and state management like enabled or game paused)
        // Input::InputHandler m_Handler;
        // keyMappings = {
        //     { "Jump", QKey::A },
        //     { "MoveLeft", QKey::Left }
        //     { "MoveLeft", QGamepad::e_GamepadAxis01 }
        // };
        // m_Handler->SetKeyMappings(keyMappings);
        // handlerContexts = { "GamePlaying" /*, "GamePaused"*/ };
        // m_Handler->SetContexts(handlerContexts);
        // m_Handler->RegisterEventCallbacks(true); // #NOTE To automatically disable on game events like game paused, using assigned contexts
        // m_Handler->SetEnabled(true);
        // if (m_Handler->Activated("Jump")) { player.Jump(); }
        // if (m_Handler->IsActivate("Walk")) { player.pos += player.forward * player.walkSpeed; }
        // if (m_Handler->Deactivate("Move")) { player.isSprinting = false; }

        // Maybe fun to have but really just extra. Behaves like registering and unregistering plus counting.
        // Really just to avoid user from managing state, but at what cost?
        // void Input::InputHandlerStartPressCounter(int key) { }
        // u16 Input::InputHandlerGetPressCounter(int key) { return 0; }
        // void Input::InputHandlerStopPressCounter(int key) { }

        // InputDevice object as handler?
    }

}
