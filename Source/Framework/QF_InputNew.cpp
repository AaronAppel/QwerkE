#include "QF_InputNew.h"

#include "QF_eKeys.h"

namespace QwerkE {

    namespace Input {

        // Stage 1 //
        // Minimal (MVP)
        // Polling API

        struct Xbit
        {
            Xbit(unsigned char value)
            {
                bitValue = value;
            }
            unsigned int bitValue : 8; // x bit unsigned integer
        };

        using HistoryIndexPointer = Xbit; // Supports rollover

        constexpr u8 s_HistoryBufferSize = U8_MAX;
        eKeys s_HistoryBufferKeys[s_HistoryBufferSize + 1];
        eKeyState s_HistoryBufferStates[s_HistoryBufferSize + 1];
        HistoryIndexPointer s_CurrentHistoryIndex = 100;
        HistoryIndexPointer s_LastFrameStartIndex = 0;
        HistoryIndexPointer s_LastFrameEndIndex = 0;

        u16 s_InputsThisFrame = 0; // #TODO Review 16 bit value

        void NewFrame_New()
        {
            s_InputsThisFrame = 0;
            s_LastFrameStartIndex = s_LastFrameEndIndex;
            s_LastFrameEndIndex = s_CurrentHistoryIndex;
        }

        void OnKeyEvent_New(eKeys key, eKeyState state)
        {
            ++s_InputsThisFrame;
            // #TODO Use distance between current and last frame start, to know if last frame is being overwritten
            const u16 range =
                s_CurrentHistoryIndex.bitValue >= s_LastFrameStartIndex.bitValue ?
                s_HistoryBufferSize + 1 - (s_CurrentHistoryIndex.bitValue - s_LastFrameStartIndex.bitValue) :
                s_LastFrameStartIndex.bitValue - s_CurrentHistoryIndex.bitValue;
            ASSERT(s_InputsThisFrame <= range, "Buffer wrapped around!");

            s_HistoryBufferKeys[s_CurrentHistoryIndex.bitValue] = key;
            s_HistoryBufferStates[s_CurrentHistoryIndex.bitValue] = state;
            ++s_CurrentHistoryIndex.bitValue;
        }

    }

}
