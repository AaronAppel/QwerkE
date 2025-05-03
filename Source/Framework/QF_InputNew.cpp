#include "QC_BitIndexRingBuffer.h"
#include "QC_TypeDefs.h"

#include "QF_InputNew.h"
#include "QF_InputStatesBitRingBuffer.h"
#include "QF_QKey.h"

namespace QwerkE {

    namespace Input {

        ///////////// #TODO Implement
        BitIndexRingBuffer<u32, bits4> s_CharsBuffer; // #TODO Review handling char input
        BitIndexRingBuffer<vec2f, bits4> s_MousePositionsBuffer;
        ///////////// #TODO Implement

        InputStatesBitRingBuffer<bits5> s_Keys;
        InputStatesBitRingBuffer<bits3> s_MouseButtons; // #TODO Investigate GLFW mouse down limit (estimated 3 until loss of input)
        InputStatesBitRingBuffer<bits3> s_MouseScroll; // #TODO Implement mouse scroll
        InputStatesBitRingBuffer<bits4> s_GamepadButtons;

#ifdef _QDEBUG
        u64 s_InputsSinceReset = 0;
#endif // _QDEBUG

        void NewFrame_Internal()
        {
            s_Keys.Advance();
            s_MouseButtons.Advance();
            s_GamepadButtons.Advance();
        }

        void OnKeyEvent_New(const QKey a_Key, const QKeyState a_KeyState)
        {
#ifdef _QDEBUG
            ++s_InputsSinceReset;
#endif // _QDEBUG

            s_Keys.Write(a_Key, a_KeyState);
        }

        void OnMouseMove_New(const double xpos, const double ypos)
        {
            // #TODO Handle:
            // - Mouse drag
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

        void OnGamepadAxisEvent(const unsigned char a_AxisId, const float a_AxisValue)
        {
            // s_GamepadAxes.Write();
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

        vec2u16 MousePos()
        {
            // #TODO Implement
            s_MousePositionsBuffer;
            return vec2f(0.f, 0.f);
        }

        bool MouseMoved()
        {
            // #TODO Implement
            s_MousePositionsBuffer;
            return false;
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
            // #TODO Implement
            return vec2f(0.f, 0.f);
        }

    }

}
