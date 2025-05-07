#include "QC_BitIndexRingBuffer.h"
#include "QC_TypeDefs.h"

#include "QF_Input.h"
#include "QF_InputStatesBitRingBuffer.h"
#include "QF_Paths.h"
#include "QF_QKey.h"
#include "QF_Serialize.h"

namespace QwerkE {

    namespace Input {

        ///////////// #TODO Implement
        BitIndexRingBuffer<u32, bits4> s_CharsBuffer; // #TODO Review handling char input
        ///////////// #TODO Implement

        InputStatesBitRingBuffer<QKey, bits5> s_Keys;

        InputStatesBitRingBuffer<QKey, bits3> s_MouseButtons; // #TODO Investigate GLFW mouse down limit (estimated 3 until loss of input)
        BitIndexRingBuffer<float, bits2> s_MouseScrolls;
        BitIndexRingBuffer<vec2f, bits2> s_MousePositionsBuffer;

        InputStatesBitRingBuffer<QGamepad, bits4> s_GamepadButtons;
        BitIndexRingBuffer<vec2f, bits2> s_GamepadAxisLeftStickBuffer;
        BitIndexRingBuffer<vec2f, bits2> s_GamepadAxisRightStickBuffer;
        // #NOTE Triggers might be better as separate float buffers
        BitIndexRingBuffer<vec2f, bits2> s_GamepadAxisTriggersBuffer;

        constexpr char* s_GameActionsFileName = "GameActions.qdata";
        static GameActions s_GameActions;
        GameActions& GetGameActions() { return s_GameActions; }

        std::vector<std::pair<GUID, KeyCallback>> s_KeyCallbacks;
        std::vector<std::pair<GUID, MouseCallback>> s_MouseCallbacks;
        std::vector<std::pair<GUID, GamepadCallback>> s_GamepadCallbacks;

#ifdef _QDEBUG
        u64 s_InputsCount = 0;
#endif // _QDEBUG

        void Internal_Initialize()
        {
            Serialize::FromFile(Paths::Setting(s_GameActionsFileName).c_str(), s_GameActions, true);

            s_MouseScrolls.AddMarker(0);
            s_MouseScrolls.AddMarker(1);
            s_MousePositionsBuffer.AddMarker(0);
            s_MousePositionsBuffer.AddMarker(1);

            s_GamepadAxisLeftStickBuffer.AddMarker(0);
            s_GamepadAxisLeftStickBuffer.AddMarker(1);
            s_GamepadAxisRightStickBuffer.AddMarker(0);
            s_GamepadAxisRightStickBuffer.AddMarker(1);
            s_GamepadAxisTriggersBuffer.AddMarker(0);
            s_GamepadAxisTriggersBuffer.AddMarker(1);
        }

        void Internal_NewFrame()
        {
            s_Keys.Advance();

            s_MouseButtons.Advance();
            s_MouseScrolls.AdvanceAllMarkers();
            s_MousePositionsBuffer.AdvanceAllMarkers();

            s_GamepadButtons.Advance();
            s_GamepadAxisLeftStickBuffer.AdvanceAllMarkers();
            s_GamepadAxisRightStickBuffer.AdvanceAllMarkers();
            s_GamepadAxisTriggersBuffer.AdvanceAllMarkers();
        }

        void Internal_Shutdown()
        {
            // #TODO Reset all static state
            // s_CharsBuffer;
            // s_GameActions;

            s_Keys.Clear();

            s_MouseButtons.Clear();
            s_MouseScrolls.Reset();
            s_MousePositionsBuffer.Reset();

            s_GamepadButtons.Clear();
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

            for (size_t i = 0; i < s_KeyCallbacks.size(); i++)
            {
                s_KeyCallbacks[i].second(a_Key, a_KeyState);
            }
        }

        void Internal_MouseMove(const vec2f& a_NewPosition)
        {
#ifdef _QDEBUG
            ++s_InputsCount;
#endif // _QDEBUG
            // #TODO Handle:
            // - Mouse drag
            // if (mouse button down)
            s_MousePositionsBuffer.Write(a_NewPosition);

            for (size_t i = 0; i < s_MouseCallbacks.size(); i++)
            {
                s_MouseCallbacks[i].second(e_MouseMove, QKeyState::e_KeyStateDown, 0.f, a_NewPosition);
            }
        }

        void Internal_MouseButton(const QKey a_Key, const QKeyState a_KeyState)
        {
#ifdef _QDEBUG
            ++s_InputsCount;
#endif // _QDEBUG
            s_MouseButtons.Write(a_Key, a_KeyState);

            for (size_t i = 0; i < s_MouseCallbacks.size(); i++)
            {
                s_MouseCallbacks[i].second(a_Key, a_KeyState, 0.f, {});
            }
        }

        void Internal_MouseScroll(const double xoffset, const double yoffset)
        {
#ifdef _QDEBUG
            ++s_InputsCount;
#endif // _QDEBUG
            // #TODO Look at how to use the xoffset
            s_MouseScrolls.Write(yoffset);

            // #TODO Handle:
            // - Scroll button up/down

            if (-1.f == xoffset)
            {
                // #TODO Add scroll event to history?
            }
            else if (1.f == xoffset)
            {
                // #TODO Add scroll event to history?
            }

            if (-1.f == yoffset)
            {
                // #TODO Add scroll event to history?
            }
            else if (1.f == yoffset)
            {
                // #TODO Add scroll event to history?
            }

            for (size_t i = 0; i < s_MouseCallbacks.size(); i++)
            {
                const QKey scrollKey = yoffset >= 0.f ? QKey::e_ScrollUp : QKey::e_ScrollDown;
                s_MouseCallbacks[i].second(scrollKey, QKeyState::e_KeyStateDown, yoffset, {});
            }
        }

        void Internal_GamepadAxis(const unsigned char a_AxisId, const vec2f a_AxisValue)
        {
#ifdef _QDEBUG
            ++s_InputsCount;
#endif // _QDEBUG
            switch (a_AxisId)
            {
            case 0:
            case 1:
                s_GamepadAxisLeftStickBuffer.Write(a_AxisValue);

                for (size_t i = 0; i < s_GamepadCallbacks.size(); i++)
                {
                    s_GamepadCallbacks[i].second(QGamepad::e_GamepadAxis01, QKeyState::e_KeyStateDown, s_GamepadAxisLeftStickBuffer.ReadTop(), {}, {});
                }
                break;
            case 2:
            case 3:
                s_GamepadAxisRightStickBuffer.Write(a_AxisValue);

                for (size_t i = 0; i < s_GamepadCallbacks.size(); i++)
                {
                    s_GamepadCallbacks[i].second(QGamepad::e_GamepadAxis23, QKeyState::e_KeyStateDown, {}, s_GamepadAxisRightStickBuffer.ReadTop(), {});
                }
                break;
            case 4:
            case 5:
                s_GamepadAxisTriggersBuffer.Write(a_AxisValue);

                for (size_t i = 0; i < s_GamepadCallbacks.size(); i++)
                {
                    s_GamepadCallbacks[i].second(QGamepad::e_GamepadAxis45, QKeyState::e_KeyStateDown, {}, {}, s_GamepadAxisTriggersBuffer.ReadTop());
                }
                break;
            }
        }

        void Internal_GamepadButton(const QGamepad a_Key, const QKeyState a_KeyState)
        {
#ifdef _QDEBUG
            ++s_InputsCount;
#endif // _QDEBUG
            s_GamepadButtons.Write(a_Key, a_KeyState);

            for (size_t i = 0; i < s_GamepadCallbacks.size(); i++)
            {
                s_GamepadCallbacks[i].second(a_Key, a_KeyState, {}, {}, {});
            }
        }

        template <typename T>
        bool Local_KeyChanged(T a_BitRingBuffer, const QKey a_Key, const QKeyState e_KeyState)
        {
            ASSERT(QKey::e_QKeyMax > a_Key, "Invalid QKey!");

            switch (a_Key)
            {
            case QKey::e_QKeyMax: // Invalid entry
                return false;

            case QKey::e_Any: // Support QKey::e_Any
                return a_BitRingBuffer.KeyThisFrame(a_Key, e_KeyState, true);

                // Support e_CtrlAny, e_ShiftAny, and e_AltAny
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
            return s_MouseScrolls.HeadIndex() != s_MouseScrolls.MarkerPosition(0); // #TODO Improve hard coded marker index
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
            return s_MousePositionsBuffer.HeadIndex() != s_MousePositionsBuffer.MarkerPosition(0); // #TODO Improve hard coded marker index
        }

        vec2f MouseDelta()
        {
            if (MouseMoved())
            {
                bits2 index = s_MousePositionsBuffer.HeadIndex();
                index.value -= 1;
                vec2f thisFrame = s_MousePositionsBuffer.ReadRandom(index.value);
                index.value -= 1;
                return thisFrame - s_MousePositionsBuffer.ReadRandom(index.value);
            }
            return vec2f(0.f, 0.f);
        }

        bool GamepadPressed(const QGamepad a_Key)
        {
            return s_GamepadButtons.KeyThisFrame(a_Key, QKeyState::e_KeyStateDown, QGamepad::e_GamepadAny == a_Key);
        }

        bool GamepadReleased(const QGamepad a_Key)
        {
            return s_GamepadButtons.KeyThisFrame(a_Key, QKeyState::e_KeyStateUp, QGamepad::e_GamepadAny == a_Key);
        }

        vec2f GamepadAxis(const int a_AxisIndex)
        {
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
            return vec2f(0.f, 0.f);
        }

        bool GamepadAxisMoved(const int a_AxisIndex)
        {
            switch (a_AxisIndex)
            {
            case 0:
            case 1:
                return s_GamepadAxisLeftStickBuffer.HeadIndex() != s_GamepadAxisLeftStickBuffer.MarkerPosition(0); // #TODO Improve hard coded marker index
            case 2:
            case 3:
                return s_GamepadAxisRightStickBuffer.HeadIndex() != s_GamepadAxisRightStickBuffer.MarkerPosition(0); // #TODO Improve hard coded marker index
            case 4:
            case 5:
                return s_GamepadAxisTriggersBuffer.HeadIndex() != s_GamepadAxisTriggersBuffer.MarkerPosition(0); // #TODO Improve hard coded marker index
            }
            return false;
        }

        GUID OnKey(KeyCallback a_Callback)
        {
            GUID callbackGuid;
            s_KeyCallbacks.push_back({ callbackGuid, a_Callback });
            return callbackGuid;
        }

        void OnKeyStop(const GUID& a_FuncId)
        {
            for (size_t i = 0; i < s_KeyCallbacks.size(); i++)
            {
                if (a_FuncId == s_KeyCallbacks[i].first)
                {
                    s_KeyCallbacks.erase(s_KeyCallbacks.begin() + i);
                    return;
                }
            }
            LOG_WARN("Could not remove KeyCallback event with GUID {0}", a_FuncId);
        }

        GUID OnMouse(MouseCallback a_Callback)
        {
            GUID callbackGuid;
            s_MouseCallbacks.push_back({ callbackGuid, a_Callback });
            return callbackGuid;
        }

        void OnMouseStop(const GUID& a_FuncId)
        {
            for (size_t i = 0; i < s_MouseCallbacks.size(); i++)
            {
                if (a_FuncId == s_MouseCallbacks[i].first)
                {
                    s_MouseCallbacks.erase(s_MouseCallbacks.begin() + i);
                    return;
                }
            }
            LOG_WARN("Could not remove MouseCallback event with GUID {0}", a_FuncId);
        }

        GUID OnGamepad(GamepadCallback a_Callback)
        {
            GUID callbackGuid;
            s_GamepadCallbacks.push_back({ callbackGuid, a_Callback });
            return callbackGuid;
        }

        void OnGamepadStop(const GUID& a_FuncId)
        {
            for (size_t i = 0; i < s_GamepadCallbacks.size(); i++)
            {
                if (a_FuncId == s_GamepadCallbacks[i].first)
                {
                    s_GamepadCallbacks.erase(s_GamepadCallbacks.begin() + i);
                    return;
                }
            }
            LOG_WARN("Could not remove GamepadCallback event with GUID {0}", a_FuncId);
        }

    }

}
