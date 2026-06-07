#ifdef _QDEBUG
#include <vector>

#include "QF_BitIndexRingBuffer.h"
#include "QF_Input.h"
#include "QF_InputStatesBitRingBuffer.h"

#include "QE_Input.h"

namespace QwerkE::Input {

extern InputStatesBitRingBuffer<QKey, u5> sKeys;

extern InputStatesBitRingBuffer<QKey, u3> sMouseButtons;
extern BitIndexRingBuffer<float, u2> sMouseScrolls;
extern BitIndexRingBuffer<vec2f, u2> sMousePositionsBuffer;

extern std::vector<std::pair<QGamepad, InputStatesBitRingBuffer<QGamepad, u4>>> sGamepadsButtons;
extern std::vector<std::pair<QGamepad, BitIndexRingBuffer<vec2f, u2>>> sGamepadAxisLeftStickBuffers;
extern std::vector<std::pair<QGamepad, BitIndexRingBuffer<vec2f, u2>>> sGamepadAxisRightStickBuffers;
extern std::vector<std::pair<QGamepad, BitIndexRingBuffer<vec2f, u2>>> sGamepadAxisTriggersBuffers;

extern u64 s_InputsCount;
extern std::vector<int> sGamepadIds;

u8 s_Most1FrameKeyInputs = 0;
vec2f s_LastNonZeroMouseDelta;

// #TODO Review "Send Input" section
s32 s_SendSystemInputKeyCode = 65;
bool s_SendSystemInputKeyDown = true;

void DrawInputStates() {
#if _QDEARIMGUI
	if (!ImGui::Begin("Input System")) {
		ImGui::End();
		return;
	}

	if (KeyDown(QKey::e_A)) {
		SendSystemInput(s_SendSystemInputKeyCode, s_SendSystemInputKeyDown, 0);
	}

	ImGui::PushItemWidth(100.f);
	ImGui::InputInt("Vk Code", &s_SendSystemInputKeyCode, 1, 100, ImGuiInputTextFlags_::ImGuiInputTextFlags_None);
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::Checkbox("Up", &s_SendSystemInputKeyDown);
	ImGui::SameLine();
	if (ImGui::SmallButton("Send Input")) {
		SendSystemInput(s_SendSystemInputKeyCode, s_SendSystemInputKeyDown, 0);
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("Send Both")) {
		SendSystemInput(s_SendSystemInputKeyCode, true, 0);
		SendSystemInput(s_SendSystemInputKeyCode, false, 0);
	}
	ImGui::Separator();

	constexpr float lineHeight = 22.f;

	if (ImGui::SmallButton("Reset")) {
		Shutdown();
	}

	if (ImGui::BeginChild("##Misc", { ImGui::GetContentRegionAvail().x - (strlen("Gamepad [0] down: %i  ") * ImGui::g_pixelsPerCharacter), 3 * lineHeight })) {
		 // ImGui::Text("Misc");
		unsigned char keysThisFrame = sKeys.InputThisFrame();
		ImGui::Text("Keys this frame: %i", keysThisFrame);
		if (keysThisFrame > s_Most1FrameKeyInputs) {
			s_Most1FrameKeyInputs = keysThisFrame;
		}
		ImGui::Text("Most 1 frame keys: %i", s_Most1FrameKeyInputs);
		ImGui::Text("Inputs count: %i", s_InputsCount);
	}
	ImGui::EndChild();

	ImGui::SameLine();
	if (ImGui::BeginChild("##CurrentlyDown", { 0.f, 3 * lineHeight })) {
		ImGui::Text("               Keys down: %i", sKeys.DownKeys());
		ImGui::Text("           Mouse down: %i", sMouseButtons.DownKeys());
		if (sGamepadsButtons.empty()) {
			ImGui::Text("No gamepad detected");
		}
		else {
			ImGui::Text("Gamepad [0] down: %i", sGamepadsButtons[0].second.DownKeys());
		}
	}
	ImGui::EndChild();
	ImGui::Dummy({ 0.f, 2.f });

	ImGui::Text("Mouse pos: %.0f, %.0f", Input::MousePos().x, Input::MousePos().y);
	const vec2f newMouseDelta = Input::MouseDelta();
	ImGui::Text("Mouse delta: %.0f, %.0f", newMouseDelta.x, newMouseDelta.y);
	if (newMouseDelta.Magnitude() > 1.0f) {
		s_LastNonZeroMouseDelta = newMouseDelta;
	}
	ImGui::Text("Last non-zero mouse delta: %.0f, %.0f", s_LastNonZeroMouseDelta.x, s_LastNonZeroMouseDelta.y);
	ImGui::Dummy({ 0.f, 2.f });

	if (ImGui::CollapsingHeader("Keyboard")) {
		ImGui::Text("QKeys + Key QKeyStates");
		for (size_t i = 0; i < sKeys.Size(); i++) {
			ImGui::Text("%i %i", sKeys.ReadKey(i), sKeys.ReadKeyState(i));
			if (i == sKeys.Head()) {
				ImGui::SameLine();
				ImGui::Text("<-");
			}
		}
	}

	if (ImGui::CollapsingHeader("Mouse")) {
		if (ImGui::BeginChild("##MouseButtons", { ImGui::g_pixelsPerCharacter * 7, sMouseButtons.Size() * lineHeight + lineHeight })) {
			ImGui::Text("Buttons");
			for (size_t i = 0; i < sMouseButtons.Size(); i++) {
				ImGui::Text("%i %i", sMouseButtons.ReadKey(i), sMouseButtons.ReadKeyState(i));
				if (i == sMouseButtons.Head()) {
					ImGui::SameLine();
					ImGui::Text("<-");
				}
			}
		}
		ImGui::EndChild();

		ImGui::SameLine();

		if (ImGui::BeginChild("Mouse Positions", { ImGui::g_pixelsPerCharacter * 14, sMousePositionsBuffer.Size() * lineHeight + lineHeight })) {
			ImGui::Text("Positions");
			for (size_t i = 0; i < sMousePositionsBuffer.Size(); i++) {
				ImGui::Text("%.0f, %.0f", sMousePositionsBuffer.ReadRandom(i).x, sMousePositionsBuffer.ReadRandom(i).y);
				if (i == sMousePositionsBuffer.HeadIndex()) {
					ImGui::SameLine();
					ImGui::Text("<-");
				}
			}
		}
		ImGui::EndChild();

		ImGui::SameLine();

		if (ImGui::BeginChild("Mouse Scrolls", { 0.f, sMouseScrolls.Size() * lineHeight + lineHeight })) {
			ImGui::Text("Scrolls");
			for (size_t i = 0; i < sMouseScrolls.Size(); i++) {
				ImGui::Text("%.0f", sMouseScrolls.ReadRandom(i));
				if (i == sMouseScrolls.HeadIndex()) {
					ImGui::SameLine();
					ImGui::Text("<-");
				}
			}
		}
		ImGui::EndChild();
	}

	// #TODO Get number of gamepads and names, ids, etc
	if (ImGui::CollapsingHeader("Gamepad##CollapsingHeader")) {
		if (sGamepadIds.empty()) {
			ImGui::Text("No gamepad detected");
		}

		InputStatesBitRingBuffer<QGamepad, u4> gamepadsButtons;
		if (!sGamepadsButtons.empty()) {
			gamepadsButtons = sGamepadsButtons[0].second;
		}

		if (ImGui::BeginChild("Gamepad Buttons + Button States", { ImGui::g_pixelsPerCharacter * 10, gamepadsButtons.Size() * lineHeight + lineHeight })) {
			ImGui::Text("Buttons");
			for (size_t i = 0; i < gamepadsButtons.Size(); i++) {
				ImGui::Text("%i %i", gamepadsButtons.ReadKey(i), gamepadsButtons.ReadKeyState(i));
				if (i == gamepadsButtons.Head()) {
					ImGui::SameLine();
					ImGui::Text("<-");
				}
			}
		}
		ImGui::EndChild();

		ImGui::SameLine();

		if (ImGui::BeginChild("Gamepad Axes", { 0.f, gamepadsButtons.Size() * lineHeight + lineHeight })) {
			ImGui::Text("Axes");
			for (size_t i = 0; i < Input::GamepadsCount(); i++) {
				ImGui::SeparatorText("Left Stick");
				ImGui::Dummy({ (ImGui::GetContentRegionAvail().x - ImGui::g_pixelsPerCharacter * 17) * 0.5f, 0.f });
				ImGui::SameLine();
				vec2f axisLeft = sGamepadAxisLeftStickBuffers[i].second.ReadRandom(i);
				ImGui::Text("%.4f, %.4f", axisLeft.x, axisLeft.y);
				if (i == sGamepadAxisLeftStickBuffers[i].second.HeadIndex()) {
					ImGui::SameLine();
					ImGui::Text("<-");
				}
				ImGui::SeparatorText("Right Stick");

				ImGui::Dummy({ (ImGui::GetContentRegionAvail().x - ImGui::g_pixelsPerCharacter * 17) * 0.5f, 0.f });
				ImGui::SameLine();
				vec2f axisRight = sGamepadAxisRightStickBuffers[i].second.ReadRandom(i);
				ImGui::Text("%.4f, %.4f", axisRight.x, axisRight.y);
				if (i == sGamepadAxisRightStickBuffers[i].second.HeadIndex()) {
					ImGui::SameLine();
					ImGui::Text("<-");
				}

				ImGui::SeparatorText("Triggers");
				ImGui::Dummy({ (ImGui::GetContentRegionAvail().x - ImGui::g_pixelsPerCharacter * 17) * 0.5f, 0.f });
				ImGui::SameLine();
				vec2f axisTriggers = sGamepadAxisTriggersBuffers[i].second.ReadRandom(i);
				ImGui::Text("%.4f, %.4f", axisTriggers.x, axisTriggers.y);
				if (i == sGamepadAxisTriggersBuffers[i].second.HeadIndex()) {
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

void DrawActiveGamepads() {
#if _QDEARIMGUI
	if (!ImGui::Begin("Gamepad")) {
		ImGui::End();
		return;
	}

	// Separate Gamepad(s) window
	const u8 count = GamepadsCount();
	for (u8 i = 0; i < count; i++) {
		u8 deviceId = i;
		QGamepad deviceQKeyId = static_cast<QGamepad>(i);

		if (ImGui::CollapsingHeader((GamepadName(deviceQKeyId) + std::string(" ") + std::to_string(sGamepadIds[i])).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
			const int axesCount = GamepadAxesCount(deviceQKeyId);
			ImGui::Text("AxesCount: %i", axesCount);

			for (size_t j = 0; j < axesCount; j++) {
				if (j % 2) {
					ImGui::Text("Axes[%i]: %f", j, GamepadAxis(j, deviceQKeyId).x);
				}
				else {
					ImGui::Text("Axes[%i]: %f", j, GamepadAxis(j, deviceQKeyId).y);
				}
			}

			int buttonCount = GamepadButtonCount(deviceQKeyId);
			ImGui::Text("ButtonCount: %i%", buttonCount);

			for (u8 j = 0; j < buttonCount; j++) {
				QGamepad buttonQKey = static_cast<QGamepad>(j);
				ImGui::Text("Buttons[%i]: %i", j, GamepadDown(buttonQKey, deviceQKeyId));
			}
		}
	}
	ImGui::End();
#endif // _QDEARIMGUI
}

void DrawDebugWindow() {
#if _QDEARIMGUI
				// #TODO Fix bug when docking gamepad window. DebugInput window should handle window creation, rather than 2 additional windows
	DrawInputStates();
	DrawActiveGamepads();
#endif // _QDEARIMGUI
}

}
#endif // _QDEBUG
