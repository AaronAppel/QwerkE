#ifdef _QDEBUG
#include <vector>

#include "QC_BitIndexRingBuffer.h"

#include "QF_Input.h"
#include "QF_InputStatesBitRingBuffer.h"

#include "QE_Input.h"

namespace QwerkE {

    namespace Input {

        extern InputStatesBitRingBuffer<QKey, u5> s_Keys;

        extern InputStatesBitRingBuffer<QKey, u3> s_MouseButtons;
        extern BitIndexRingBuffer<float, u2> s_MouseScrolls;
        extern BitIndexRingBuffer<vec2f, u2> s_MousePositionsBuffer;

        extern std::vector<std::pair<QGamepad, InputStatesBitRingBuffer<QGamepad, u4>>> s_GamepadsButtons;
        extern std::vector<std::pair<QGamepad, BitIndexRingBuffer<vec2f, u2>>> s_GamepadAxisLeftStickBuffers;
        extern std::vector<std::pair<QGamepad, BitIndexRingBuffer<vec2f, u2>>> s_GamepadAxisRightStickBuffers;
        extern std::vector<std::pair<QGamepad, BitIndexRingBuffer<vec2f, u2>>> s_GamepadAxisTriggersBuffers;

        extern u64 s_InputsCount;
        extern std::vector<int> s_GamepadIds;

        u8 s_Most1FrameKeyInputs = 0;
        vec2f s_LastNonZeroMouseDelta;

        // #TODO Review "Send Input" section
        s32 s_SendSystemInputKeyCode = 65;
        bool s_SendSystemInputKeyDown = true;

        void DrawInputStates()
        {
#if _QDEARIMGUI
            if (!ImGui::Begin("Input System"))
            {
                ImGui::End();
                return;
            }

            if (KeyDown(QKey::e_A))
            {
                SendSystemInput(s_SendSystemInputKeyCode, s_SendSystemInputKeyDown, 0);
            }

            ImGui::PushItemWidth(100.f);
            ImGui::InputInt("Vk Code", &s_SendSystemInputKeyCode, 1, 100, ImGuiInputTextFlags_::ImGuiInputTextFlags_None);
            ImGui::PopItemWidth();
            ImGui::SameLine();
            ImGui::Checkbox("Up", &s_SendSystemInputKeyDown);
            ImGui::SameLine();
            if (ImGui::SmallButton("Send Input"))
            {
                SendSystemInput(s_SendSystemInputKeyCode, s_SendSystemInputKeyDown, 0);
            }
            ImGui::SameLine();
            if (ImGui::SmallButton("Send Both"))
            {
                SendSystemInput(s_SendSystemInputKeyCode, true, 0);
                SendSystemInput(s_SendSystemInputKeyCode, false, 0);
            }
            ImGui::Separator();

            constexpr float lineHeight = 22.f;

            if (ImGui::SmallButton("Reset"))
            {
                Shutdown();
            }

            if (ImGui::BeginChild("##Misc", { ImGui::GetContentRegionAvail().x - (strlen("Gamepad [0] down: %i  ") * ImGui::g_pixelsPerCharacter), 3 * lineHeight }))
            {
                // ImGui::Text("Misc");
                unsigned char keysThisFrame = s_Keys.InputThisFrame();
                ImGui::Text("Keys this frame: %i", keysThisFrame);
                if (keysThisFrame > s_Most1FrameKeyInputs)
                {
                    s_Most1FrameKeyInputs = keysThisFrame;
                }
                ImGui::Text("Most 1 frame keys: %i", s_Most1FrameKeyInputs);
                ImGui::Text("Inputs count: %i", s_InputsCount);
            }
            ImGui::EndChild();

            ImGui::SameLine();
            if (ImGui::BeginChild("##CurrentlyDown", { 0.f, 3 * lineHeight }))
            {
                ImGui::Text("               Keys down: %i", s_Keys.DownKeys());
                ImGui::Text("           Mouse down: %i", s_MouseButtons.DownKeys());
                if (s_GamepadsButtons.empty())
                {
                    ImGui::Text("No gamepad detected");
                }
                else
                {
                    ImGui::Text("Gamepad [0] down: %i", s_GamepadsButtons[0].second.DownKeys());
                }
            }
            ImGui::EndChild();
            ImGui::Dummy({ 0.f, 2.f });

            ImGui::Text("Mouse pos: %.0f, %.0f", Input::MousePos().x, Input::MousePos().y);
            const vec2f newMouseDelta = Input::MouseDelta();
            ImGui::Text("Mouse delta: %.0f, %.0f", newMouseDelta.x, newMouseDelta.y);
            if (Math::Magnitude(newMouseDelta) > 1.0f)
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
            if (ImGui::CollapsingHeader("Gamepad##CollapsingHeader"))
            {
                if (s_GamepadIds.empty())
                {
                    ImGui::Text("No gamepad detected");
                }

                InputStatesBitRingBuffer<QGamepad, u4> gamepadsButtons;
                if (!s_GamepadsButtons.empty())
                {
                    gamepadsButtons = s_GamepadsButtons[0].second;
                }

                if (ImGui::BeginChild("Gamepad Buttons + Button States", { ImGui::g_pixelsPerCharacter * 10, gamepadsButtons.Size() * lineHeight + lineHeight }))
                {
                    ImGui::Text("Buttons");
                    for (size_t i = 0; i < gamepadsButtons.Size(); i++)
                    {
                        ImGui::Text("%i %i", gamepadsButtons.ReadKey(i), gamepadsButtons.ReadKeyState(i));
                        if (i == gamepadsButtons.Head())
                        {
                            ImGui::SameLine();
                            ImGui::Text("<-");
                        }
                    }
                }
                ImGui::EndChild();

                ImGui::SameLine();

                if (ImGui::BeginChild("Gamepad Axes", { 0.f, gamepadsButtons.Size() * lineHeight + lineHeight }))
                {
                    ImGui::Text("Axes");
                    for (size_t i = 0; i < Input::GamepadsCount(); i++)
                    {
                        ImGui::SeparatorText("Left Stick");
                        ImGui::Dummy({ (ImGui::GetContentRegionAvail().x - ImGui::g_pixelsPerCharacter * 17) * 0.5f, 0.f });
                        ImGui::SameLine();
                        vec2f axisLeft = s_GamepadAxisLeftStickBuffers[i].second.ReadRandom(i);
                        ImGui::Text("%.4f, %.4f", axisLeft.x, axisLeft.y);
                        if (i == s_GamepadAxisLeftStickBuffers[i].second.HeadIndex())
                        {
                            ImGui::SameLine();
                            ImGui::Text("<-");
                        }
                        ImGui::SeparatorText("Right Stick");

                        ImGui::Dummy({ (ImGui::GetContentRegionAvail().x - ImGui::g_pixelsPerCharacter * 17) * 0.5f, 0.f });
                        ImGui::SameLine();
                        vec2f axisRight = s_GamepadAxisRightStickBuffers[i].second.ReadRandom(i);
                        ImGui::Text("%.4f, %.4f", axisRight.x, axisRight.y);
                        if (i == s_GamepadAxisRightStickBuffers[i].second.HeadIndex())
                        {
                            ImGui::SameLine();
                            ImGui::Text("<-");
                        }

                        ImGui::SeparatorText("Triggers");
                        ImGui::Dummy({ (ImGui::GetContentRegionAvail().x - ImGui::g_pixelsPerCharacter * 17) * 0.5f, 0.f });
                        ImGui::SameLine();
                        vec2f axisTriggers = s_GamepadAxisTriggersBuffers[i].second.ReadRandom(i);
                        ImGui::Text("%.4f, %.4f", axisTriggers.x, axisTriggers.y);
                        if (i == s_GamepadAxisTriggersBuffers[i].second.HeadIndex())
                        {
                            ImGui::SameLine();
                            ImGui::Text("<-");
                        }
                    }
                }
                ImGui::EndChild();
            }
            ImGui::End();
#endif // _QDEARIMGUI
        }

        void DrawActiveGamepads()
        {
#if _QDEARIMGUI
            if (!ImGui::Begin("Gamepad"))
            {
                ImGui::End();
                return;
            }

            // Separate Gamepad(s) window
            const u8 count = GamepadsCount();
            for (u8 i = 0; i < count; i++)
            {
                u8 deviceId = i;
                QGamepad deviceQKeyId = static_cast<QGamepad>(i);

                if (ImGui::CollapsingHeader((GamepadName(deviceQKeyId) + std::string(" ") + std::to_string(s_GamepadIds[i])).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
                {
                    const int axesCount = GamepadAxesCount(deviceQKeyId);
                    ImGui::Text("AxesCount: %i", axesCount);

                    for (size_t j = 0; j < axesCount; j++)
                    {
                        if (j % 2)
                        {
                            ImGui::Text("Axes[%i]: %f", j, GamepadAxis(j, deviceQKeyId).x);
                        }
                        else
                        {
                            ImGui::Text("Axes[%i]: %f", j, GamepadAxis(j, deviceQKeyId).y);
                        }
                    }

                    int buttonCount = GamepadButtonCount(deviceQKeyId);
                    ImGui::Text("ButtonCount: %i%", buttonCount);

                    for (u8 j = 0; j < buttonCount; j++)
                    {
                        QGamepad buttonQKey = static_cast<QGamepad>(j);
                        ImGui::Text("Buttons[%i]: %i", j, GamepadDown(buttonQKey, deviceQKeyId));
                    }
                }
            }
            ImGui::End();
#endif // _QDEARIMGUI
        }

        void DrawDebugWindow()
        {
#if _QDEARIMGUI
            // #TODO Fix bug when docking gamepad window. DebugInput window should handle window creation, rather than 2 additional windows
            DrawInputStates();
            DrawActiveGamepads();
#endif // _QDEARIMGUI
        }

    }

}
#endif // _QDEBUG
