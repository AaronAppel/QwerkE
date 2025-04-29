#include "QF_InputNew.h"

namespace QwerkE {

    namespace Input {

        using RollingIndex = bits4; // #NOTE Utilize rollover
        constexpr u16 s_HistoryBufferSize = RollingIndex::max + 1;

        QKey s_HistoryBufferKeys[s_HistoryBufferSize];
        bool s_HistoryBufferStates[s_HistoryBufferSize];

        RollingIndex s_NextToWriteIndex = 0;
        RollingIndex s_LastFrameStartIndex = 0;
        RollingIndex s_LastFrameEndIndex = 0;

        u8 s_KeysCurrentlyDown = 0; // Supports e_Any

#ifdef _QDEBUG
        static u8 s_InputsThisFrame = 0;
        static u8 s_Most1FrameInputs = 0;
        static u64 s_InputsSinceReset = 0;
#endif // _QDEBUG

        void NewFrame_Internal()
        {
            s_InputsThisFrame = 0;
            s_LastFrameStartIndex = s_LastFrameEndIndex;
            s_LastFrameEndIndex = s_NextToWriteIndex;
        }

        u8 InputEventsThisFrame()
        {
            const u8 eventsThisFrame = s_NextToWriteIndex.bits >= s_LastFrameEndIndex.bits ?
                s_NextToWriteIndex.bits - s_LastFrameEndIndex.bits :
                s_HistoryBufferSize - s_LastFrameEndIndex.bits + s_NextToWriteIndex.bits;
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
                s_NextToWriteIndex.bits >= s_LastFrameStartIndex.bits ?
                s_HistoryBufferSize - (s_NextToWriteIndex.bits - s_LastFrameStartIndex.bits) :
                s_LastFrameStartIndex.bits - s_NextToWriteIndex.bits;
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

            s_HistoryBufferKeys[s_NextToWriteIndex.bits] = a_Key;
            s_HistoryBufferStates[s_NextToWriteIndex.bits] = a_KeyState;

            if (KeyStateDown == a_KeyState)
            {
                ++s_KeysCurrentlyDown;
            }
            else
            {
                --s_KeysCurrentlyDown;
            }

            ++s_NextToWriteIndex.bits;
        }

        void OnMouseMove_New(const double xpos, const double ypos)
        {
            // #TODO Handle:
            // - Mouse drag
        }

        void OnMouseButton_New(const int button, const int action, const int mods)
        {
            // #TODO Handle:
            // - Mouse key/button up/down
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
            RollingIndex index = s_NextToWriteIndex.bits - 1;
            ASSERT(U8_MAX >= s_HistoryBufferSize, "u8 too small for range!");
            u8 inputEventsThisFrame = InputEventsThisFrame();

            while (inputEventsThisFrame > 0)
            {
                if ((a_Key == s_HistoryBufferKeys[index.bits] || e_Any == a_Key) &&
                    a_KeyState == s_HistoryBufferStates[index.bits])
                {
                    return true;
                }
                --index.bits;
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

#ifdef _QDEBUG
        void DrawDebugWindow()
        {
            if (!ImGui::Begin("Input System"))
                return;

            if (ImGui::Button("Reset"))
            {
                s_InputsSinceReset = 0;
                s_NextToWriteIndex.bits = 0;
                s_LastFrameEndIndex.bits = 0;
                s_LastFrameStartIndex.bits = 0;

                memset(s_HistoryBufferKeys, 0, s_HistoryBufferSize);
                memset(s_HistoryBufferStates, 0, s_HistoryBufferSize);
            }

            ImGui::Text("Inputs this frame: %i", s_InputsThisFrame);
            ImGui::Text("Most 1 frame inputs: %i", s_Most1FrameInputs);
            ImGui::Text("Inputs since reset: %i", s_InputsSinceReset);
            ImGui::Text("Keys currently down: %i", s_KeysCurrentlyDown);

            ImGui::Text("Rolling index max: %i", RollingIndex::max);
            ImGui::Text("History buffer size: %i", s_HistoryBufferSize);

            ImGui::Text("Indices");
            ImGui::Text("Next: %i", s_NextToWriteIndex.bits);
            ImGui::Text("LastEnd: %i", s_LastFrameEndIndex.bits);
            ImGui::Text("LastStart: %i", s_LastFrameStartIndex.bits);

            if (ImGui::CollapsingHeader("Keys"))
            {
                for (size_t i = 0; i < s_HistoryBufferSize; i++)
                {
                    ImGui::Text("%i", s_HistoryBufferKeys[i]);
                    if (i == s_NextToWriteIndex.bits)
                    {
                        ImGui::SameLine();
                        ImGui::Text("<-");
                    }
                }
            }
            if (ImGui::CollapsingHeader("States"))
            {
                for (size_t i = 0; i < s_HistoryBufferSize; i++)
                {
                    ImGui::Text("%i", s_HistoryBufferStates[i]);
                    if (i == s_NextToWriteIndex.bits)
                    {
                        ImGui::SameLine();
                        ImGui::Text("<-");
                    }
                }
            }

            ImGui::End();
        }
#endif // _QDEBUG

    }

}
