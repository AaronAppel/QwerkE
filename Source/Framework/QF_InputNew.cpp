#include "QF_InputNew.h"

#include "QF_eKeys.h"

namespace QwerkE {

    namespace Input {

        using RollingIndex = bits4; // #NOTE Utilize rollover
        constexpr u16 s_HistoryBufferSize = RollingIndex::max + 1;

        eKeys s_HistoryBufferKeys[s_HistoryBufferSize];
        eKeyState s_HistoryBufferStates[s_HistoryBufferSize];

        RollingIndex s_CurrentHistoryIndex = 0;
        RollingIndex s_LastFrameStartIndex = 0;
        RollingIndex s_LastFrameEndIndex = 0;

#ifdef _QDEBUG
        static u16 s_InputsThisFrame = 0; // #TODO Review 16 bit value
#endif // _QDEBUG

        void NewFrame_New()
        {
            s_InputsThisFrame = 0;
            s_LastFrameStartIndex = s_LastFrameEndIndex;
            s_LastFrameEndIndex = s_CurrentHistoryIndex;
        }

        u8 InputEventsThisFrame()
        {
            const u8 eventsThisFrame = s_CurrentHistoryIndex.bits >= s_LastFrameEndIndex.bits ?
                s_CurrentHistoryIndex.bits - s_LastFrameEndIndex.bits :
                s_HistoryBufferSize - s_LastFrameEndIndex.bits + s_CurrentHistoryIndex.bits;
#ifdef _QDEBUG
            // #TODO Update assert and add more debugging
            // ASSERT(s_InputsThisFrame == eventsThisFrame, "Error calculating input events this frame!");
#endif // _QDEBUG
            return eventsThisFrame;
        }

#ifdef _QDEBUG
        u8 RemainingWritesThisFrame()
        {
            const u8 remainingWritesThisFrame =
                s_CurrentHistoryIndex.bits >= s_LastFrameStartIndex.bits ?
                s_HistoryBufferSize - (s_CurrentHistoryIndex.bits - s_LastFrameStartIndex.bits) :
                s_LastFrameStartIndex.bits - s_CurrentHistoryIndex.bits;
            return remainingWritesThisFrame;
        }
#endif // _QDEBUG

        void OnKeyEvent_New(eKeys a_Key, eKeyState a_KeyState)
        {
#ifdef _QDEBUG
            ++s_InputsThisFrame;
            ASSERT(0 < RemainingWritesThisFrame(), "Buffer wrapped around!");
#endif // _QDEBUG

            s_HistoryBufferKeys[s_CurrentHistoryIndex.bits] = a_Key;
            s_HistoryBufferStates[s_CurrentHistoryIndex.bits] = a_KeyState;

            ++s_CurrentHistoryIndex.bits;
        }

        bool KeyCheck(const eKeys a_Key, const eKeyState a_KeyState)
        {
            RollingIndex index = s_CurrentHistoryIndex.bits - 1;
            u8 inputEventsThisFrame = InputEventsThisFrame();

            while (inputEventsThisFrame > 0)
            {
                if (a_Key == s_HistoryBufferKeys[index.bits] &&
                    a_KeyState == s_HistoryBufferStates[index.bits])
                {
                    return true;
                }
                --index.bits;
                --inputEventsThisFrame;
            }

            return false;
        }

        bool KeyPressed(const eKeys a_Key)
        {
            return KeyCheck(a_Key, eKeyState::eKeyState_Press);
        }

        bool KeyReleased(const eKeys a_Key)
        {
            return KeyCheck(a_Key, eKeyState::eKeyState_Release);
        }

    }

}
