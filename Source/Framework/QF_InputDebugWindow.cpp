#ifdef _QDEBUG
#include "QC_BitIndexRingBuffer.h"

#include "QF_Input.h"
#include "QF_InputStatesBitRingBuffer.h"

namespace QwerkE {

    namespace Input {

        ///////////// #TODO Implement
        extern BitIndexRingBuffer<u32, bits4> s_CharsBuffer; // #TODO Review handling char input
        ///////////// #TODO Implement

        extern InputStatesBitRingBuffer<bits5> s_Keys;

        extern InputStatesBitRingBuffer<bits3> s_MouseButtons; // #TODO Investigate GLFW mouse down limit (estimated 3 until loss of input)
        extern BitIndexRingBuffer<float, bits2> s_MouseScrolls;
        extern BitIndexRingBuffer<vec2f, bits2> s_MousePositionsBuffer; // #NOTE Recent write can be used to get current position (no library specific get state)

        extern InputStatesBitRingBuffer<bits4> s_GamepadButtons;
        extern BitIndexRingBuffer<vec2f, bits2> s_GamepadAxisLeftStickBuffer; // #NOTE Recent write can be used to get current position (no library specific get state)
        extern BitIndexRingBuffer<vec2f, bits2> s_GamepadAxisRightStickBuffer; // #NOTE Recent write can be used to get current position (no library specific get state)
        // #NOTE Triggers might be better as separate float buffers
        extern BitIndexRingBuffer<vec2f, bits2> s_GamepadAxisTriggersBuffer; // #NOTE Recent write can be used to get current position (no library specific get state)

        extern u64 s_InputsCount;
        extern std::vector<int> s_DeviceIds;

        u8 s_Most1FrameKeyInputs;
        vec2f s_LastNonZeroMouseDelta;

        void DrawDebugWindow()
        {
            if (!ImGui::Begin("Input System"))
            {
                ImGui::End();
                return;
            }

            constexpr float lineHeight = 22.f;

            if (ImGui::BeginChild("##Misc", { ImGui::GetContentRegionAvail().x - ( strlen("Gamepad down: 00   ") * ImGui::g_pixelsPerCharacter ), 3 * lineHeight}))
            {
                // ImGui::Text("Misc");
                unsigned char keysThisFrame = s_Keys.InputThisFrame();
                ImGui::Text("Keys this frame: %i", keysThisFrame);
                if (keysThisFrame > s_Most1FrameKeyInputs)
                {
                    s_Most1FrameKeyInputs = keysThisFrame;
                }
                ImGui::Text("1 frame keys: %i", s_Most1FrameKeyInputs);
                ImGui::Text("Inputs count: %i", s_InputsCount);
            }
            ImGui::EndChild();

            ImGui::SameLine();

            if (ImGui::BeginChild("##CurrentlyDown", { 0.f, 3 * lineHeight}))
            {
                ImGui::Text("         Keys down: %i", s_Keys.DownKeys());
                ImGui::Text("     Mouse down: %i", s_MouseButtons.DownKeys());
                ImGui::Text("Gamepad down: %i", s_GamepadButtons.DownKeys());
            }
            ImGui::EndChild();
            ImGui::Dummy({ 0.f, 2.f });

            ImGui::Text("Mouse pos: %.0f, %.0f", Input::MousePos().x, Input::MousePos().y);
            const vec2f newMouseDelta = Input::MouseDelta();
            ImGui::Text("Mouse delta: %.0f, %.0f", newMouseDelta.x, newMouseDelta.y);
            if (glm::length(newMouseDelta) > 1.0f) // #TODO Abstract glm call
            {
                s_LastNonZeroMouseDelta = newMouseDelta;
            }
            ImGui::Text("Last non-zero mouse delta: %.0f, %.0f", s_LastNonZeroMouseDelta.x, s_LastNonZeroMouseDelta.y);
            ImGui::Dummy({ 0.f, 2.f });

            if (ImGui::CollapsingHeader("Keyboard"))
            {
                ImGui::Text("QKeys + Key QKeyStates");
                for (size_t i = 0; i < s_Keys.Size(); i++)
                {
                    ImGui::Text("%i %i", s_Keys.ReadKey(i), s_Keys.ReadKeyState(i));
                    if (i == s_Keys.Head())
                    {
                        ImGui::SameLine();
                        ImGui::Text("<-");
                    }
                }
            }

            if (ImGui::CollapsingHeader("Mouse"))
            {
                if (ImGui::BeginChild("##MouseButtons", { ImGui::g_pixelsPerCharacter * 7, s_MouseButtons.Size() * lineHeight + lineHeight }))
                {
                    ImGui::Text("Buttons");
                    for (size_t i = 0; i < s_MouseButtons.Size(); i++)
                    {
                        ImGui::Text("%i %i", s_MouseButtons.ReadKey(i), s_MouseButtons.ReadKeyState(i));
                        if (i == s_MouseButtons.Head())
                        {
                            ImGui::SameLine();
                            ImGui::Text("<-");
                        }
                    }
                }
                ImGui::EndChild();

                ImGui::SameLine();

                if (ImGui::BeginChild("Mouse Positions", { ImGui::g_pixelsPerCharacter * 14, s_MousePositionsBuffer.Size() * lineHeight + lineHeight }))
                {
                    ImGui::Text("Positions");
                    for (size_t i = 0; i < s_MousePositionsBuffer.Size(); i++)
                    {
                        ImGui::Text("%.0f, %.0f", s_MousePositionsBuffer.ReadRandom(i).x, s_MousePositionsBuffer.ReadRandom(i).y);
                        if (i == s_MousePositionsBuffer.HeadIndex())
                        {
                            ImGui::SameLine();
                            ImGui::Text("<-");
                        }
                    }
                }
                ImGui::EndChild();

                ImGui::SameLine();

                if (ImGui::BeginChild("Mouse Scrolls", { 0.f, s_MouseScrolls.Size() * lineHeight + lineHeight }))
                {
                    ImGui::Text("Scrolls");
                    for (size_t i = 0; i < s_MouseScrolls.Size(); i++)
                    {
                        ImGui::Text("%.0f", s_MouseScrolls.ReadRandom(i));
                        if (i == s_MouseScrolls.HeadIndex())
                        {
                            ImGui::SameLine();
                            ImGui::Text("<-");
                        }
                    }
                }
                ImGui::EndChild();
            }

            // #TODO Get number of gamepads and names, ids, etc
            if (ImGui::CollapsingHeader("Gamepad"))
            {
                if (s_DeviceIds.empty())
                {
                    ImGui::Text("No gamepad detected");
                }

                if (ImGui::BeginChild("Gamepad Buttons + Button States", { ImGui::g_pixelsPerCharacter * 10, s_GamepadButtons.Size() * lineHeight + lineHeight }))
                {
                    ImGui::Text("Buttons");
                    for (size_t i = 0; i < s_GamepadButtons.Size(); i++)
                    {
                        ImGui::Text("%i %i", s_GamepadButtons.ReadKey(i), s_GamepadButtons.ReadKeyState(i));
                        if (i == s_GamepadButtons.Head())
                        {
                            ImGui::SameLine();
                            ImGui::Text("<-");
                        }
                    }
                }
                ImGui::EndChild();

                ImGui::SameLine();

                if (ImGui::BeginChild("Gamepad Axes", { 0.f, s_GamepadButtons.Size() * lineHeight + lineHeight }))
                {
                    ImGui::Text("Axes");
                    ImGui::SeparatorText("Left Stick");
                    for (size_t i = 0; i < s_GamepadAxisLeftStickBuffer.Size(); i++)
                    {
                        ImGui::Dummy({ (ImGui::GetContentRegionAvail().x - ImGui::g_pixelsPerCharacter * 17) * 0.5f, 0.f });
                        ImGui::SameLine();
                        vec2f axis = s_GamepadAxisLeftStickBuffer.ReadRandom(i);
                        ImGui::Text("%.4f, %.4f", axis.x, axis.y);
                        if (i == s_GamepadAxisLeftStickBuffer.HeadIndex())
                        {
                            ImGui::SameLine();
                            ImGui::Text("<-");
                        }
                    }
                    ImGui::SeparatorText("Right Stick");
                    for (size_t i = 0; i < s_GamepadAxisRightStickBuffer.Size(); i++)
                    {
                        ImGui::Dummy({ (ImGui::GetContentRegionAvail().x - ImGui::g_pixelsPerCharacter * 17) * 0.5f, 0.f });
                        ImGui::SameLine();
                        vec2f axis = s_GamepadAxisRightStickBuffer.ReadRandom(i);
                        ImGui::Text("%.4f, %.4f", axis.x, axis.y);
                        if (i == s_GamepadAxisRightStickBuffer.HeadIndex())
                        {
                            ImGui::SameLine();
                            ImGui::Text("<-");
                        }
                    }
                    ImGui::SeparatorText("Triggers");
                    for (size_t i = 0; i < s_GamepadAxisTriggersBuffer.Size(); i++)
                    {
                        ImGui::Dummy({ (ImGui::GetContentRegionAvail().x - ImGui::g_pixelsPerCharacter * 17) * 0.5f, 0.f });
                        ImGui::SameLine();
                        vec2f axis = s_GamepadAxisTriggersBuffer.ReadRandom(i);
                        ImGui::Text("%.4f, %.4f", axis.x, axis.y);
                        if (i == s_GamepadAxisTriggersBuffer.HeadIndex())
                        {
                            ImGui::SameLine();
                            ImGui::Text("<-");
                        }
                    }
                }
                ImGui::EndChild();
            }
            ImGui::End();
        }

    }

}
#endif // _QDEBUG
