#include "QC_BitIndexRingBuffer.h"

#include "QF_InputNew.h"
#include "QF_InputStatesBitRingBuffer.h"

namespace QwerkE {

    namespace Input {

        ///////////// #TODO Implement
        extern BitIndexRingBuffer<u32, bits4> s_CharsBuffer; // #TODO Review handling char input
        extern BitIndexRingBuffer<vec2f, bits2> s_MousePositionsBuffer;
        ///////////// #TODO Implement

        extern InputStatesBitRingBuffer<bits5> s_Keys;
        extern InputStatesBitRingBuffer<bits3> s_MouseButtons; // #TODO Investigate GLFW mouse down limit (estimated 3 until loss of input) or is just my mouse limited to 3
        extern InputStatesBitRingBuffer<bits4> s_GamepadButtons;
        extern BitIndexRingBuffer<vec2f, bits2> s_GamepadAxisLeftStickBuffer; // #NOTE Recent write can be used to get current position (no library specific get state)
        extern BitIndexRingBuffer<vec2f, bits2> s_GamepadAxisRightStickBuffer; // #NOTE Recent write can be used to get current position (no library specific get state)
        // #NOTE Triggers might be better as separate float buffers
        extern BitIndexRingBuffer<vec2f, bits2> s_GamepadAxisTriggersBuffer; // #NOTE Recent write can be used to get current position (no library specific get state)

        u8 s_Most1FrameKeyInputs;

#ifdef _QDEBUG
        extern u64 s_InputsSinceReset;

        void DrawDebugWindow()
        {
            if (!ImGui::Begin("Input System"))
            {
                ImGui::End();
                return;
            }

            unsigned char keysThisFrame = s_Keys.InputThisFrame();
            ImGui::Text("Key inputs this frame: %i", keysThisFrame);
            if (keysThisFrame > s_Most1FrameKeyInputs)
            {
                s_Most1FrameKeyInputs = keysThisFrame;
            }
            ImGui::Text("Most 1 frame key inputs: %i", s_Most1FrameKeyInputs);
            ImGui::Text("Inputs since reset: %i", s_InputsSinceReset);
            ImGui::Text("Keys currently down: %i", s_Keys.DownKeys());
            ImGui::Text("Mouse buttons currently down: %i", s_MouseButtons.DownKeys());
            ImGui::Text("Mouse pos: %f, %f", Input::MousePos().x, Input::MousePos().y);

            ImGui::SeparatorText("Keys");
            if (ImGui::CollapsingHeader("Keys QKeys + Key QKeyStates"))
            {
                for (size_t i = 0; i < s_Keys.Size(); i++)
                {
                    ImGui::Text("%i", s_Keys.ReadKey(i));
                    ImGui::SameLine();
                    ImGui::Text(" %i", s_Keys.ReadKeyState(i));
                    if (i == s_Keys.Head())
                    {
                        ImGui::SameLine();
                        ImGui::Text("<-");
                    }
                }
            }

            ImGui::SeparatorText("Mouse");
            if (ImGui::CollapsingHeader("Mouse Buttons + Button QKeyStates"))
            {
                for (size_t i = 0; i < s_MouseButtons.Size(); i++)
                {
                    ImGui::Text("%i", s_MouseButtons.ReadKey(i));
                    ImGui::SameLine();
                    ImGui::Text(" %i", s_MouseButtons.ReadKeyState(i));
                    if (i == s_MouseButtons.Head())
                    {
                        ImGui::SameLine();
                        ImGui::Text("<-");
                    }
                }
            }
            if (ImGui::CollapsingHeader("Mouse Positions"))
            {
                for (size_t i = 0; i < s_MousePositionsBuffer.Size(); i++)
                {
                    ImGui::Text("%f, %f", s_MousePositionsBuffer.ReadRandom(i).x, s_MousePositionsBuffer.ReadRandom(i).y);
                    if (i == s_MousePositionsBuffer.HeadIndex())
                    {
                        ImGui::SameLine();
                        ImGui::Text("<-");
                    }
                }
            }

            ImGui::SeparatorText("Gamepad");
            // #TODO Get number of gamepads and names, ids, etc
            if (ImGui::CollapsingHeader("Gamepad Buttons + Button States"))
            {
                for (size_t i = 0; i < s_GamepadButtons.Size(); i++)
                {
                    ImGui::Text("%i", s_GamepadButtons.ReadKey(i));
                    ImGui::SameLine();
                    ImGui::Text(" %i", s_GamepadButtons.ReadKeyState(i));
                    if (i == s_GamepadButtons.Head())
                    {
                        ImGui::SameLine();
                        ImGui::Text("<-");
                    }
                }
            }
            if (ImGui::CollapsingHeader("Gamepad Axes"))
            {
                ImGui::SeparatorText("Left Stick");
                for (size_t i = 0; i < s_GamepadAxisLeftStickBuffer.Size(); i++)
                {
                    vec2f axis = s_GamepadAxisLeftStickBuffer.ReadRandom(i);
                    ImGui::Text("%f, %f", axis.x, axis.y);
                    if (i == s_GamepadAxisLeftStickBuffer.HeadIndex())
                    {
                        ImGui::SameLine();
                        ImGui::Text("<-");
                    }
                }
                ImGui::SeparatorText("Right Stick");
                for (size_t i = 0; i < s_GamepadAxisRightStickBuffer.Size(); i++)
                {
                    vec2f axis = s_GamepadAxisRightStickBuffer.ReadRandom(i);
                    ImGui::Text("%f, %f", axis.x, axis.y);
                    if (i == s_GamepadAxisRightStickBuffer.HeadIndex())
                    {
                        ImGui::SameLine();
                        ImGui::Text("<-");
                    }
                }
                ImGui::SeparatorText("Triggers");
                for (size_t i = 0; i < s_GamepadAxisTriggersBuffer.Size(); i++)
                {
                    vec2f axis = s_GamepadAxisTriggersBuffer.ReadRandom(i);
                    ImGui::Text("%f, %f", axis.x, axis.y);
                    if (i == s_GamepadAxisTriggersBuffer.HeadIndex())
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
