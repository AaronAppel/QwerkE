#include "QC_BitIndexRingBuffer.h"
#include "QC_TypeDefs.h"

#include "QF_InputNew.h"
#include "QF_InputStatesBitRingBuffer.h"
#include "QF_QKey.h"

namespace QwerkE {

    namespace Input {

        ///////////// #TODO Implement
        BitIndexRingBuffer<u32, bits4> s_CharsBuffer; // #TODO Review handling char input
        ///////////// #TODO Implement

        InputStatesBitRingBuffer<bits5> s_Keys;
        InputStatesBitRingBuffer<bits3> s_MouseButtons; // #TODO Investigate GLFW mouse down limit (estimated 3 until loss of input)
        InputStatesBitRingBuffer<bits3> s_MouseScroll; // #TODO Implement mouse scroll
        InputStatesBitRingBuffer<bits4> s_GamepadButtons;

        BitIndexRingBuffer<vec2f, bits2> s_MousePositionsBuffer; // #NOTE Recent write can be used to get current position (no library specific get state)
        BitIndexRingBuffer<vec2f, bits2> s_GamepadAxisLeftStickBuffer; // #NOTE Recent write can be used to get current position (no library specific get state)
        BitIndexRingBuffer<vec2f, bits2> s_GamepadAxisRightStickBuffer; // #NOTE Recent write can be used to get current position (no library specific get state)
        // #NOTE Triggers might be better as separate float buffers
        BitIndexRingBuffer<vec2f, bits2> s_GamepadAxisTriggersBuffer; // #NOTE Recent write can be used to get current position (no library specific get state)

#ifdef _QDEBUG
        u64 s_InputsSinceReset = 0;
#endif // _QDEBUG

        void Initialize_Internal()
        {
            s_MousePositionsBuffer.AddMarker(0);
            s_MousePositionsBuffer.AddMarker(1);

            s_GamepadAxisLeftStickBuffer.AddMarker(0);
            s_GamepadAxisLeftStickBuffer.AddMarker(1);
            s_GamepadAxisRightStickBuffer.AddMarker(0);
            s_GamepadAxisRightStickBuffer.AddMarker(1);
            s_GamepadAxisTriggersBuffer.AddMarker(0);
            s_GamepadAxisTriggersBuffer.AddMarker(1);
        }

        void NewFrame_Internal()
        {
            s_Keys.Advance();
            s_MouseButtons.Advance();
            s_GamepadButtons.Advance();

            s_MousePositionsBuffer.AdvanceAllMarkers();

            s_GamepadAxisLeftStickBuffer.AdvanceAllMarkers();
            s_GamepadAxisRightStickBuffer.AdvanceAllMarkers();
            s_GamepadAxisTriggersBuffer.AdvanceAllMarkers();
        }

        void OnKeyEvent_New(const QKey a_Key, const QKeyState a_KeyState)
        {
#ifdef _QDEBUG
            ++s_InputsSinceReset;
#endif // _QDEBUG
            s_Keys.Write(a_Key, a_KeyState);
        }

        void OnMouseMove_New(const vec2f& a_NewPosition)
        {
            // #TODO Handle:
            // - Mouse drag
            // if (mouse button down)
            s_MousePositionsBuffer.Write(a_NewPosition);
        }

        void OnMouseButton_New(const QKey a_Key, const QKeyState a_KeyState)
        {
            s_MouseButtons.Write(a_Key, a_KeyState);
        }

        void OnMouseScroll_New(const double xoffset, const double yoffset)
        {
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
        }

        void OnGamepadAxisEvent(const unsigned char a_AxisId, const vec2f a_AxisValue)
        {
            switch (a_AxisId)
            {
            case 0:
            case 1:
                s_GamepadAxisLeftStickBuffer.Write(a_AxisValue);
                break;
            case 2:
            case 3:
                s_GamepadAxisRightStickBuffer.Write(a_AxisValue);
                break;
            case 4:
            case 5:
                s_GamepadAxisTriggersBuffer.Write(a_AxisValue);
                break;
            }
        }

        void OnGamepadButtonEvent(const QKey a_Key, const QKeyState a_KeyState)
        {
            s_GamepadButtons.Write(a_Key, a_KeyState);
        }

        bool KeyPressed(const QKey a_Key)
        {
            return s_Keys.KeyThisFrame(a_Key, QKeyState::e_KeyStateDown);
        }

        bool KeyReleased(const QKey a_Key)
        {
            return s_Keys.KeyThisFrame(a_Key, QKeyState::e_KeyStateUp);
        }

        bool KeyDown_Internal(const QKey a_Key) { return false; }

        bool MousePressed(const QKey a_Key)
        {
            return s_MouseButtons.KeyThisFrame(a_Key, QKeyState::e_KeyStateDown);
        }

        bool MouseReleased(const QKey a_Key)
        {
            return s_MouseButtons.KeyThisFrame(a_Key, QKeyState::e_KeyStateUp);
        }

        bool MouseDown_Internal(const QKey a_Key) { return false; }

        bool MouseScrolled() // #TODO Or force MousePressed?
        {
            // #TODO Implement
            s_MouseScroll;
            return false;
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
            // #TODO Implement
            s_MousePositionsBuffer;
            return vec2f(0.f, 0.f);
        }

        bool GamepadPressed(const QKey a_Key)
        {
            return s_GamepadButtons.KeyThisFrame(a_Key, QKeyState::e_KeyStateDown);
        }

        bool GamepadReleased(const QKey a_Key)
        {
            return s_GamepadButtons.KeyThisFrame(a_Key, QKeyState::e_KeyStateUp);
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

    }

}
