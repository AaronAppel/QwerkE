#include "QC_BitIndexRingBuffer.h"

#include "QF_InputNew.h"
#include "QF_InputStatesBitRingBuffer.h"

namespace QwerkE {

    namespace Input {

        ///////////// #TODO Implement
        extern InputStatesBitRingBuffer<bits4> s_Keys; // #TODO Implement

        extern BitIndexRingBuffer<QKey, bits4> s_KeysBuffer;
        extern BitIndexRingBuffer<QKey, bits4> s_KeyStatesBuffer;
        extern BitIndexRingBuffer<u32, bits4> s_CharsBuffer;

        extern BitIndexRingBuffer<QKey, bits4> s_MouseButtonsBuffer;
        extern bool s_MouseButtonsStatesBuffer[bits4::MAX + 1];
        extern BitIndexRingBuffer<vec2f, bits4> s_MousePositionsBuffer;

        extern BitIndexRingBuffer<QKey, bits4> s_GamepadsBuffer; // #TODO Any gamepad?
        ///////////// #TODO Implement

        using RollingIndex = bits4; // #NOTE Utilize rollover
        constexpr u16 s_HistoryBufferSize = RollingIndex::MAX + 1;

        extern QKey s_HistoryBufferKeys[];
        extern bool s_HistoryBufferStates[]; // #TODO Shrink bools from byte to bit

        extern RollingIndex s_NextToWriteIndex;
        extern RollingIndex s_LastFrameStartIndex;
        extern RollingIndex s_LastFrameEndIndex;

        extern u8 s_KeysCurrentlyDown; // Supports e_Any
        extern u8 s_MouseButtonsCurrentlyDown; // Supports e_Any

#ifdef _QDEBUG
        extern u8 s_InputsThisFrame;
        extern u8 s_Most1FrameInputs;
        extern u64 s_InputsSinceReset;

        void DrawDebugWindow()
        {
            if (!ImGui::Begin("Input System"))
            {
                ImGui::End();
                return;
            }

            if (ImGui::Button("Reset"))
            {
                s_InputsSinceReset = 0;
                s_NextToWriteIndex.value = 0;
                s_LastFrameEndIndex.value = 0;
                s_LastFrameStartIndex.value = 0;

                memset(s_HistoryBufferKeys, 0, s_HistoryBufferSize);
                memset(s_HistoryBufferStates, 0, s_HistoryBufferSize);
            }

            ImGui::Text("Inputs this frame: %i", s_InputsThisFrame);
            ImGui::Text("Most 1 frame inputs: %i", s_Most1FrameInputs);
            ImGui::Text("Inputs since reset: %i", s_InputsSinceReset);
            ImGui::Text("Keys currently down: %i", s_KeysCurrentlyDown);

            ImGui::Text("Rolling index max: %i", RollingIndex::MAX);
            ImGui::Text("History buffer size: %i", s_HistoryBufferSize);

            ImGui::Text("Indices");
            ImGui::Text("Next: %i", s_NextToWriteIndex.value);
            ImGui::Text("LastEnd: %i", s_LastFrameEndIndex.value);
            ImGui::Text("LastStart: %i", s_LastFrameStartIndex.value);

            if (ImGui::CollapsingHeader("Key Values"))
            {
                for (size_t i = 0; i < s_HistoryBufferSize; i++)
                {
                    ImGui::Text("%i", s_HistoryBufferKeys[i]);
                    ImGui::SameLine();
                    ImGui::Text(" %c", s_HistoryBufferKeys[i]);
                    if (i == s_NextToWriteIndex.value)
                    {
                        ImGui::SameLine();
                        ImGui::Text("<-");
                    }
                }
            }
            if (ImGui::CollapsingHeader("Key States"))
            {
                for (size_t i = 0; i < s_HistoryBufferSize; i++)
                {
                    ImGui::Text("%i", s_HistoryBufferStates[i]);
                    if (i == s_NextToWriteIndex.value)
                    {
                        ImGui::SameLine();
                        ImGui::Text("<-");
                    }
                }
            }

            if (ImGui::CollapsingHeader("Mouse Values"))
            {
                for (size_t i = 0; i < s_MouseButtonsBuffer.Size(); i++)
                {
                    ImGui::Text("%i", s_MouseButtonsBuffer.ReadRandom(i));
                    ImGui::SameLine();
                    ImGui::Text(" %i", s_MouseButtonsBuffer.ReadRandom(i));
                    if (i == s_MouseButtonsBuffer.HeadIndex())
                    {
                        ImGui::SameLine();
                        ImGui::Text("<-");
                    }
                }
            }
            if (ImGui::CollapsingHeader("Mouse States"))
            {
                for (size_t i = 0; i < s_MouseButtonsBuffer.Size(); i++)
                {
                    ImGui::Text("%i", s_MouseButtonsStatesBuffer[i]);
                    if (i == s_MouseButtonsBuffer.HeadIndex())
                    {
                        ImGui::SameLine();
                        ImGui::Text("<-");
                    }
                }
            }


            if (ImGui::CollapsingHeader("New Keys"))
            {
                for (size_t i = 0; i < s_Keys.Size(); i++)
                {
                    ImGui::Text("%i", s_Keys.ReadKey(i));
                }
            }
            if (ImGui::CollapsingHeader("New Keys States"))
            {
                for (size_t i = 0; i < s_Keys.Size(); i++)
                {
                    ImGui::Text("%i", s_Keys.ReadKeyState(i));
                }
            }

            ImGui::End();
        }
#endif // _QDEBUG

    }

}
