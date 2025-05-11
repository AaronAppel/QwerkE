#include "QF_Input.h"

#include <vector>

#include "QC_BitIndexRingBuffer.h"
#include "QC_TypeDefs.h"

#include "QF_InputStatesBitRingBuffer.h"
#include "QF_Paths.h"
#include "QF_QKey.h"
#include "QF_Serialize.h"

namespace QwerkE {

    namespace Input {

        InputStatesBitRingBuffer<QKey, u5> s_Keys;

        InputStatesBitRingBuffer<QKey, u3> s_MouseButtons; // #TODO Investigate GLFW mouse down limit (estimated 3 until loss of input)
        BitIndexRingBuffer<float, u2> s_MouseScrolls;
        BitIndexRingBuffer<vec2f, u2> s_MousePositionsBuffer;

        std::vector<std::pair<QGamepad, InputStatesBitRingBuffer<QGamepad, u4>>> s_GamepadsButtons;
        BitIndexRingBuffer<vec2f, u2> s_GamepadAxisLeftStickBuffer; // #TODO Support multiple gamepads
        BitIndexRingBuffer<vec2f, u2> s_GamepadAxisRightStickBuffer;
        BitIndexRingBuffer<vec2f, u2> s_GamepadAxisTriggersBuffer; // #NOTE Triggers might be better as separate float buffers

        static constexpr char* s_GameActionsFileName = "GameActions.qdata";
        static GameActions s_GameActions;
        GameActions& GetGameActions() { return s_GameActions; }

        std::vector<KeyCallback> s_KeyCallbacks;
        std::vector<MouseCallback> s_MouseCallbacks;
        std::vector<GamepadCallback> s_GamepadCallbacks;

        static constexpr u8 s_LastFrameEndMarkerId = 0;

#ifdef _QDEBUG
        u64 s_InputsCount = 0;
#endif // _QDEBUG

        void Internal_Initialize()
        {
            Serialize::FromFile(Paths::Setting(s_GameActionsFileName).c_str(), s_GameActions, true);

            s_MouseScrolls.AddMarker(0);
            s_MouseScrolls.AddMarker(0);
            s_MousePositionsBuffer.AddMarker(0);
            s_MousePositionsBuffer.AddMarker(0);

            s_GamepadAxisLeftStickBuffer.AddMarker(0);
            s_GamepadAxisLeftStickBuffer.AddMarker(0);
            s_GamepadAxisRightStickBuffer.AddMarker(0);
            s_GamepadAxisRightStickBuffer.AddMarker(0);
            s_GamepadAxisTriggersBuffer.AddMarker(0);
            s_GamepadAxisTriggersBuffer.AddMarker(0);
        }

        void Internal_NewFrame()
        {
            s_Keys.Advance();

            s_MouseButtons.Advance();
            s_MouseScrolls.AdvanceAllMarkers();
            s_MousePositionsBuffer.AdvanceAllMarkers();

            for (u8 i = 0; i < s_GamepadsButtons.size(); i++)
            {
                s_GamepadsButtons[i].second.Advance();
            }
            s_GamepadAxisLeftStickBuffer.AdvanceAllMarkers();
            s_GamepadAxisRightStickBuffer.AdvanceAllMarkers();
            s_GamepadAxisTriggersBuffer.AdvanceAllMarkers();
        }

        void Internal_Shutdown()
        {
            s_Keys.Clear();

            s_MouseButtons.Clear();
            s_MouseScrolls.Reset();
            s_MousePositionsBuffer.Reset();

            for (u8 i = 0; i < s_GamepadsButtons.size(); i++)
            {
                s_GamepadsButtons[i].second.Clear();
            }
            s_GamepadsButtons.clear();

            s_GamepadAxisLeftStickBuffer.Reset();
            s_GamepadAxisRightStickBuffer.Reset();
            s_GamepadAxisTriggersBuffer.Reset();
        }

        void Internal_KeyChanged(const QKey a_Key, const QKeyState a_KeyState)
        {
#ifdef _QDEBUG
            ++s_InputsCount;
#endif // _QDEBUG
            s_Keys.Write(a_Key, a_KeyState);

            for (u16 i = 0; i < s_KeyCallbacks.size(); i++)
            {
                s_KeyCallbacks[i](a_Key, a_KeyState);
            }
        }

        void Internal_MouseMove(const vec2f& a_NewPosition)
        {
#ifdef _QDEBUG
            ++s_InputsCount;
#endif // _QDEBUG
            s_MousePositionsBuffer.Write(a_NewPosition);

            for (u16 i = 0; i < s_MouseCallbacks.size(); i++)
            {
                s_MouseCallbacks[i](e_MouseMove, QKeyState::e_KeyStateDown, 0.f, a_NewPosition);
            }
        }

        void Internal_MouseButton(const QKey a_Key, const QKeyState a_KeyState)
        {
#ifdef _QDEBUG
            ++s_InputsCount;
#endif // _QDEBUG
            s_MouseButtons.Write(a_Key, a_KeyState);

            for (u16 i = 0; i < s_MouseCallbacks.size(); i++)
            {
                s_MouseCallbacks[i](a_Key, a_KeyState, 0.f, {});
            }
        }

        void Internal_MouseScroll(const double a_OffsetX, const double a_OffsetY)
        {
#ifdef _QDEBUG
            ++s_InputsCount;
#endif // _QDEBUG
            s_MouseScrolls.Write(a_OffsetY); // #TODO Look at how to use the a_OffsetX

            for (u16 i = 0; i < s_MouseCallbacks.size(); i++)
            {
                const QKey scrollKey = a_OffsetY >= 0.f ? QKey::e_ScrollUp : QKey::e_ScrollDown;
                s_MouseCallbacks[i](scrollKey, QKeyState::e_KeyStateDown, a_OffsetY, {});
            }
        }

        void Internal_GamepadAxis(const unsigned char a_AxisId, const vec2f a_AxisValue, const QGamepad a_GamepadId)
        {
#ifdef _QDEBUG
            ++s_InputsCount;
#endif // _QDEBUG
            switch (a_AxisId)
            {
            case 0:
            case 1:
                s_GamepadAxisLeftStickBuffer.Write(a_AxisValue);

                for (u16 i = 0; i < s_GamepadCallbacks.size(); i++)
                {
                    s_GamepadCallbacks[i](QGamepad::e_GamepadAxis01, QKeyState::e_KeyStateDown, s_GamepadAxisLeftStickBuffer.ReadTop(), {}, {}, a_GamepadId);
                }
                break;
            case 2:
            case 3:
                s_GamepadAxisRightStickBuffer.Write(a_AxisValue);

                for (u16 i = 0; i < s_GamepadCallbacks.size(); i++)
                {
                    s_GamepadCallbacks[i](QGamepad::e_GamepadAxis23, QKeyState::e_KeyStateDown, {}, s_GamepadAxisRightStickBuffer.ReadTop(), {}, a_GamepadId);
                }
                break;
            case 4:
            case 5:
                s_GamepadAxisTriggersBuffer.Write(a_AxisValue);

                for (u16 i = 0; i < s_GamepadCallbacks.size(); i++)
                {
                    s_GamepadCallbacks[i](QGamepad::e_GamepadAxis45, QKeyState::e_KeyStateDown, {}, {}, s_GamepadAxisTriggersBuffer.ReadTop(), a_GamepadId);
                }
                break;
            }
        }

        void Internal_GamepadButton(const QGamepad a_Key, const QKeyState a_KeyState, const QGamepad a_GamepadId)
        {
#ifdef _QDEBUG
            ++s_InputsCount;
#endif // _QDEBUG
            ASSERT(QGamepad::e_GamepadId0 <= a_GamepadId && QGamepad::e_QGamepadIdMax > a_GamepadId, "Invalid a_GamepadId!");
            for (u8 i = 0; i < s_GamepadsButtons.size(); i++)
            {
                if (a_GamepadId == s_GamepadsButtons[i].first)
                {
                    s_GamepadsButtons[i].second.Write(a_Key, a_KeyState);
                    break;
                }
            }

            for (u16 i = 0; i < s_GamepadCallbacks.size(); i++)
            {
                s_GamepadCallbacks[i](a_Key, a_KeyState, {}, {}, {}, a_GamepadId);
            }
        }

        template <typename T>
        bool Local_KeyChanged(T a_BitRingBuffer, const QKey a_Key, const QKeyState e_KeyState)
        {
            ASSERT(QKey::e_QKeyMax > a_Key, "Invalid QKey!");

            switch (a_Key)
            {
            case QKey::e_Any:
                return a_BitRingBuffer.KeyThisFrame(a_Key, e_KeyState, true);

            case QKey::e_CtrlAny:
                return a_BitRingBuffer.KeyThisFrame(QKey::e_CtrlL, e_KeyState) || a_BitRingBuffer.KeyThisFrame(QKey::e_CtrlR, e_KeyState);
            case QKey::e_ShiftAny:
                return a_BitRingBuffer.KeyThisFrame(QKey::e_ShiftL, e_KeyState) || a_BitRingBuffer.KeyThisFrame(QKey::e_ShiftR, e_KeyState);
            case QKey::e_AltAny:
                return a_BitRingBuffer.KeyThisFrame(QKey::e_AltL, e_KeyState) || a_BitRingBuffer.KeyThisFrame(QKey::e_AltR, e_KeyState);
            }
            return a_BitRingBuffer.KeyThisFrame(a_Key, e_KeyState);
        }

        bool KeyPressed(const QKey a_Key)
        {
            return Local_KeyChanged(s_Keys, a_Key, e_KeyStateDown);
        }

        bool KeyReleased(const QKey a_Key)
        {
            return Local_KeyChanged(s_Keys, a_Key, e_KeyStateUp);
        }

        bool MousePressed(const QKey a_Key)
        {
            return Local_KeyChanged(s_MouseButtons, a_Key, e_KeyStateDown);
        }

        bool MouseReleased(const QKey a_Key)
        {
            return Local_KeyChanged(s_MouseButtons, a_Key, e_KeyStateDown);
        }

        bool MouseScrolled()
        {
            return s_MouseScrolls.HeadIndex() != s_MouseScrolls.MarkerPosition(s_LastFrameEndMarkerId);
        }

        float MouseScrollDelta()
        {
            if (MouseScrolled())
            {
                return s_MouseScrolls.ReadTop();
            }
            return 0.f;
        }

        vec2f MousePos()
        {
            return s_MousePositionsBuffer.ReadTop();
        }

        bool MouseMoved()
        {
            return s_MousePositionsBuffer.HeadIndex() != s_MousePositionsBuffer.MarkerPosition(s_LastFrameEndMarkerId);
        }

        vec2f MouseDelta()
        {
            if (MouseMoved())
            {
                u2 index = s_MousePositionsBuffer.HeadIndex();
                index.value -= 1;
                vec2f thisFrame = s_MousePositionsBuffer.ReadRandom(index.value);
                index.value -= 1;
                return thisFrame - s_MousePositionsBuffer.ReadRandom(index.value);
            }
            return vec2f(0.f, 0.f);
        }

        bool GamepadPressed(const QGamepad a_Key, const QGamepad a_GamepadId)
        {
            ASSERT(QGamepad::e_GamepadId0 <= a_GamepadId && QGamepad::e_QGamepadIdMax > a_GamepadId, "Invalid a_GamepadId!");
            for (u8 i = 0; i < s_GamepadsButtons.size(); i++)
            {
                if (a_GamepadId == s_GamepadsButtons[i].first)
                {
                    s_GamepadsButtons[i].second.KeyThisFrame(a_Key, QKeyState::e_KeyStateDown, QGamepad::e_GamepadAny == a_Key);
                }
            }
            // LOG_WARN("a_GamepadId not found!");
            return false;
        }

        bool GamepadReleased(const QGamepad a_Key, const QGamepad a_GamepadId)
        {
            ASSERT(QGamepad::e_GamepadId0 <= a_GamepadId && QGamepad::e_QGamepadIdMax > a_GamepadId, "Invalid a_GamepadId!");
            for (u8 i = 0; i < s_GamepadsButtons.size(); i++)
            {
                if (a_GamepadId == s_GamepadsButtons[i].first)
                {
                    s_GamepadsButtons[i].second.KeyThisFrame(a_Key, QKeyState::e_KeyStateUp, QGamepad::e_GamepadAny == a_Key);
                }
            }
            // LOG_WARN("a_GamepadId not found!");
            return false;
        }

        vec2f GamepadAxis(const int a_AxisIndex, const QGamepad a_GamepadId)
        {
            ASSERT(QGamepad::e_QGamepadAxisIndexFirst <= a_AxisIndex && QGamepad::e_QGamepadAxisIndexMax > a_AxisIndex, "Invalid axis");
            ASSERT(QGamepad::e_GamepadId0 <= a_GamepadId && QGamepad::e_QGamepadIdMax > a_GamepadId, "Invalid a_GamepadId!");
            // #TODO What is more intuitive, always returning a vec2f, or individual floats? Do users want that?
            switch (a_AxisIndex)
            {
            case 0:
            case 1:
                return s_GamepadAxisLeftStickBuffer.ReadTop();
            case 2:
            case 3:
                return s_GamepadAxisRightStickBuffer.ReadTop();
            case 4:
            case 5:
                return s_GamepadAxisTriggersBuffer.ReadTop();
            }
            // LOG_WARN("a_GamepadId not found!");
            return vec2f(0.f, 0.f);
        }

        bool GamepadAxisMoved(const int a_AxisIndex, const QGamepad a_GamepadId)
        {
            ASSERT(QGamepad::e_QGamepadAxisIndexFirst <= a_AxisIndex && QGamepad::e_QGamepadAxisIndexMax > a_AxisIndex, "Invalid axis");
            ASSERT(QGamepad::e_GamepadId0 <= a_GamepadId && QGamepad::e_QGamepadIdMax > a_GamepadId, "Invalid a_GamepadId!");
            switch (a_AxisIndex)
            {
            case 0:
            case 1:
                return s_GamepadAxisLeftStickBuffer.HeadIndex() != s_GamepadAxisLeftStickBuffer.MarkerPosition(s_LastFrameEndMarkerId);
            case 2:
            case 3:
                return s_GamepadAxisRightStickBuffer.HeadIndex() != s_GamepadAxisRightStickBuffer.MarkerPosition(s_LastFrameEndMarkerId);
            case 4:
            case 5:
                return s_GamepadAxisTriggersBuffer.HeadIndex() != s_GamepadAxisTriggersBuffer.MarkerPosition(s_LastFrameEndMarkerId);
            }
            // LOG_WARN("a_GamepadId not found!");
            return false;
        }

        void OnKey(const KeyCallback& a_OnKeyFunction)
        {
            for (u16 i = 0; i < s_KeyCallbacks.size(); i++)
            {
                if (a_OnKeyFunction.target_type() == s_KeyCallbacks[i].target_type())
                {
                    LOG_WARN("KeyCallback event already registered!");
                    return;
                }
            }
            s_KeyCallbacks.push_back(a_OnKeyFunction);
        }

        void OnKeyStop(const KeyCallback& a_OnKeyFunction)
        {
            for (u16 i = 0; i < s_KeyCallbacks.size(); i++)
            {
                if (a_OnKeyFunction.target_type() == s_KeyCallbacks[i].target_type())
                {
                    s_KeyCallbacks.erase(s_KeyCallbacks.begin() + i);
                    return;
                }
            }
            LOG_WARN("Could not remove KeyCallback event!");
        }

        void OnMouse(const MouseCallback& a_OnMouseFunction)
        {
            for (u16 i = 0; i < s_MouseCallbacks.size(); i++)
            {
                if (a_OnMouseFunction.target_type() == s_MouseCallbacks[i].target_type())
                {
                    LOG_WARN("MouseCallback event already registered!");
                    return;
                }
            }
            s_MouseCallbacks.push_back(a_OnMouseFunction);
        }

        void OnMouseStop(const MouseCallback& a_OnMouseFunction)
        {
            for (u16 i = 0; i < s_MouseCallbacks.size(); i++)
            {
                if (a_OnMouseFunction.target_type() == s_MouseCallbacks[i].target_type())
                {
                    s_MouseCallbacks.erase(s_MouseCallbacks.begin() + i);
                    return;
                }
            }
            LOG_WARN("Could not remove MouseCallback event!");
        }

        void OnGamepad(const GamepadCallback& a_OnGamepadFunction)
        {
            for (u16 i = 0; i < s_GamepadCallbacks.size(); i++)
            {
                if (a_OnGamepadFunction.target_type() == s_GamepadCallbacks[i].target_type())
                {
                    LOG_WARN("GamepadCallback event already registered!");
                    return;
                }
            }
            s_GamepadCallbacks.push_back(a_OnGamepadFunction);
        }

        void OnGamepadStop(const GamepadCallback& a_OnGamepadFunction)
        {
            for (u16 i = 0; i < s_GamepadCallbacks.size(); i++)
            {
                if (a_OnGamepadFunction.target_type() == s_GamepadCallbacks[i].target_type())
                {
                    s_GamepadCallbacks.erase(s_GamepadCallbacks.begin() + i);
                    return;
                }
            }
            LOG_WARN("Could not remove GamepadCallback event!");
        }

    }

}
