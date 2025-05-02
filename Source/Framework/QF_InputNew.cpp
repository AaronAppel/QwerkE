#include "QC_BitIndexRingBuffer.h"

#include "QF_InputNew.h"
#include "QF_InputStatesBitRingBuffer.h"

namespace QwerkE {

    namespace Input {

        ///////////// #TODO Implement
        InputStatesBitRingBuffer<bits4> s_Keys; // #TODO Implement

        QKey s_TempQKey;
        BitIndexRingBuffer<QKey, bits4> s_KeysBuffer;
        BitIndexRingBuffer<QKey, bits4> s_KeyStatesBuffer;
        BitIndexRingBuffer<u32, bits4> s_CharsBuffer;

        BitIndexRingBuffer<QKey, bits4> s_MouseButtonsBuffer;
        bool s_MouseButtonsStatesBuffer[bits4::MAX + 1]; // #TODO Shrink bools from byte to bit
        BitIndexRingBuffer<vec2f, bits4> s_MousePositionsBuffer;

        BitIndexRingBuffer<QKey, bits4> s_GamepadsBuffer; // #TODO Any gamepad?
        ///////////// #TODO Implement

        using RollingIndex = bits4; // #NOTE Utilize rollover
        constexpr u16 s_HistoryBufferSize = RollingIndex::MAX + 1;

        QKey s_HistoryBufferKeys[s_HistoryBufferSize];
        bool s_HistoryBufferStates[s_HistoryBufferSize]; // #TODO Shrink bools from byte to bit

        RollingIndex s_NextToWriteIndex = 0;
        RollingIndex s_LastFrameStartIndex = 0;
        RollingIndex s_LastFrameEndIndex = 0;

        u8 s_KeysCurrentlyDown = 0; // Supports e_Any
        u8 s_MouseButtonsCurrentlyDown = 0; // Supports e_Any

#ifdef _QDEBUG
        u8 s_InputsThisFrame = 0;
        u8 s_Most1FrameInputs = 0;
        u64 s_InputsSinceReset = 0;
#endif // _QDEBUG

        void Initialize_Internal()
        {
            s_KeysBuffer.AddMarker(0); // End last frame
            s_KeysBuffer.AddMarker(0); // Start last frame
        }

        void NewFrame_Internal()
        {
#ifdef _QDEBUG
            s_InputsThisFrame = 0;
#endif // _QDEBUG
            s_LastFrameStartIndex = s_LastFrameEndIndex;
            s_LastFrameEndIndex = s_NextToWriteIndex;
        }

        void Update_Internal()
        {
            if (ImGui::Begin("IndexBitBuffer"))
            {
                ImGui::PushItemWidth(100);
                int temp = s_TempQKey;
                if (ImGui::InputInt("TempKey", (int*)&temp))
                {
                    s_TempQKey = static_cast<QKey>(temp);
                }
                ImGui::PopItemWidth();

                if (ImGui::Button("Write"))
                {
                    s_KeysBuffer.Write(s_TempQKey);
                }
                if (ImGui::Button("Advance"))
                {
                    // s_KeysBuffer.AdvanceAllMarkers();

                    s_KeysBuffer.SetMarkerPosition(1, s_KeysBuffer.MarkerPosition(0));
                    s_KeysBuffer.SetMarkerPosition(0, s_KeysBuffer.HeadIndex());
                }
                ImGui::Separator();
                for (size_t i = 0; i < s_KeysBuffer.Size(); i++)
                {
                    ImGui::Text("[%i] %i", i, s_KeysBuffer.ReadRandom(i));
                    if (i == s_KeysBuffer.HeadIndex())
                    {
                        ImGui::SameLine();
                        ImGui::Text("<- H");
                    }
                    if (i == s_KeysBuffer.MarkerPosition(0))
                    {
                        ImGui::SameLine();
                        ImGui::Text("<- E");
                    }
                    if (i == s_KeysBuffer.MarkerPosition(1))
                    {
                        ImGui::SameLine();
                        ImGui::Text("<- S");
                    }
                }
                ImGui::End();
            }
        }

        u8 InputEventsThisFrame()
        {
            const u8 eventsThisFrame = s_NextToWriteIndex.value >= s_LastFrameEndIndex.value ?
                s_NextToWriteIndex.value - s_LastFrameEndIndex.value :
                s_HistoryBufferSize - s_LastFrameEndIndex.value + s_NextToWriteIndex.value;
#ifdef _QDEBUG
            // #TODO Update assert and add more debugging
            // ASSERT(s_InputsThisFrame == eventsThisFrame, "Error calculating input events this frame!");
#endif // _QDEBUG
            return eventsThisFrame;
        }

#ifdef _QDEBUG
        u8 RemainingWritesThisFrame()
        {
            // #TODO Add assert and test logic more thoroughly
            const u8 remainingWritesThisFrame =
                s_NextToWriteIndex.value >= s_LastFrameStartIndex.value ?
                s_HistoryBufferSize - (s_NextToWriteIndex.value - s_LastFrameStartIndex.value) :
                s_LastFrameStartIndex.value - s_NextToWriteIndex.value;
            return remainingWritesThisFrame;
        }
#endif // _QDEBUG

        void OnKeyEvent_New(const QKey a_Key, const bool a_KeyState)
        {
#ifdef _QDEBUG
            ++s_InputsThisFrame;
            ++s_InputsSinceReset;
            ASSERT(0 < RemainingWritesThisFrame(), "Writing over last/this frame state!");
            if (s_InputsThisFrame > s_Most1FrameInputs)
            {
                s_Most1FrameInputs = s_InputsThisFrame;
            }
#endif // _QDEBUG

            {
                // Newest
                s_Keys.Write(a_Key, a_KeyState);
            }
            {
                // Newer
                // #TODO Maybe create a collection that holds QKeys and matching booleans in 1 collection. Move even more code out of here.
                s_HistoryBufferStates[s_KeysBuffer.HeadIndex()] = a_KeyState;
                s_KeysBuffer.Write(a_Key);
            }
            {
                // Old
                s_HistoryBufferKeys[s_NextToWriteIndex.value] = a_Key;
                s_HistoryBufferStates[s_NextToWriteIndex.value] = a_KeyState;
            }

            if (KeyStateDown == a_KeyState)
            {
                ++s_KeysCurrentlyDown;
            }
            else
            {
                --s_KeysCurrentlyDown;
            }

            ++s_NextToWriteIndex.value;
        }

        void OnMouseMove_New(const double xpos, const double ypos)
        {
            // #TODO Handle:
            // - Mouse drag
        }

        void OnMouseButton_New(const QKey a_Key, const bool a_KeyState)
        {
            s_MouseButtonsStatesBuffer[s_MouseButtonsBuffer.HeadIndex()] = a_KeyState;
            s_MouseButtonsBuffer.Write(a_Key);

            if (KeyStateDown == a_KeyState)
            {
                ++s_MouseButtonsCurrentlyDown;
            }
            else
            {
                --s_MouseButtonsCurrentlyDown;
            }
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

        void OnJoystickEvent()
        {
            // #TODO: Joystick buttons
        }

        bool KeyHistoryCheck(const QKey a_Key, const bool a_KeyState)
        {
            RollingIndex index = s_NextToWriteIndex.value - 1;
            ASSERT(U8_MAX >= s_HistoryBufferSize, "u8 too small for range!");
            u8 inputEventsThisFrame = InputEventsThisFrame();

            while (inputEventsThisFrame > 0)
            {
                if ((a_Key == s_HistoryBufferKeys[index.value] || e_Any == a_Key) &&
                    a_KeyState == s_HistoryBufferStates[index.value])
                {
                    return true;
                }
                --index.value;
                --inputEventsThisFrame;
            }

            return false;
        }

        bool KeyPressed(const QKey a_Key)
        {
            return KeyHistoryCheck(a_Key, KeyStateDown);
        }

        bool KeyReleased(const QKey a_Key)
        {
            return KeyHistoryCheck(a_Key, KeyStateUp);
        }

        bool KeyDown_Internal(const QKey a_Key) { return false; }

        bool MousePressed(const QKey a_Key)
        {
            // #TODO Implement
            s_MouseButtonsBuffer;
            return false;
        }

        bool MouseReleased(const QKey a_Key)
        {
            // #TODO Implement
            s_MouseButtonsBuffer;
            return false;
        }

        bool MouseDown_Internal(const QKey a_Key) { return false; }

        bool MouseScrolled() // #TODO Or force MousePressed?
        {
            // #TODO Implement
            s_MouseButtonsBuffer;
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
            // #TODO Implement
            return false;
        }

        bool GamepadReleased(const QKey a_Key)
        {
            // #TODO Implement
            return false;
        }

        bool GamepadDown(const QKey a_Key)
        {
            // #TODO Implement
            return false;
        }

        vec2f GamepadAxis(const int a_AxisIndex)
        {
            // #TODO Implement
            return vec2f(0.f, 0.f);
        }

    }

}
