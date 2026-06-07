#include "QF_Input.h"

#include <vector>

#include "QF_BitIndexRingBuffer.h"
#include "QF_EventCallback.h"
#include "QF_InputStatesBitRingBuffer.h"
#include "QF_Paths.h"
#include "QF_QKey.h"
#include "QF_Serialize.h"
#include "QF_TypeDefs.h"

namespace QwerkE {

namespace Input {

InputStatesBitRingBuffer<QKey, u5> sKeys;

InputStatesBitRingBuffer<QKey, u3> sMouseButtons; // #TODO Investigate GLFW mouse down limit (estimated 3 until loss of input)
BitIndexRingBuffer<float, u2> sMouseScrolls;
BitIndexRingBuffer<vec2f, u2> sMousePositionsBuffer;

// #TODO Other names to consider: "RollingBuffer", "OverflowBuffer", "WrappingBuffer", "Circular/CircleBuffer"
std::vector<std::pair<QGamepad, InputStatesBitRingBuffer<QGamepad, u4>>> sGamepadsButtons;
std::vector<std::pair<QGamepad, BitIndexRingBuffer<vec2f, u2>>> sGamepadAxisLeftStickBuffers;
std::vector<std::pair<QGamepad, BitIndexRingBuffer<vec2f, u2>>> sGamepadAxisRightStickBuffers;
std::vector<std::pair<QGamepad, BitIndexRingBuffer<vec2f, u2>>> sGamepadAxisTriggersBuffers; // #NOTE Triggers might be better as separate float buffers

static constexpr char* sGameActionsFileName = "GameActions.qdata";
static GameActions sGameActions;
GameActions& GetGameActions() { return sGameActions; }

// #TODO Move to using new EventCallback() class
// auto sEventCallbackKeys = MakeEventCallback<QKey, QKeyState>();
EventCallback<QKey, QKeyState> sEventCallbackKeys;

EventCallbackHandle<QKey, QKeyState> TestHandle;
void OnInput(QKey a, QKeyState b) {
	int bp = 0;
	LOG_INFO("OnInput");
}

std::vector<KeyCallback> sKeyCallbacks;
std::vector<MouseCallback> sMouseCallbacks;
std::vector<GamepadCallback> sGamepadCallbacks;

static constexpr u8 sLastFrameEndMarkerId = 0;

#ifdef _QDEBUG
u64 s_InputsCount = 0;
#endif // _QDEBUG

void InternalInitialize() {
	Serialize::FromFile(Paths::Setting(sGameActionsFileName).c_str(), sGameActions, true);

	sMouseScrolls.AddMarker(0);
	sMouseScrolls.AddMarker(0);
	sMousePositionsBuffer.AddMarker(0);
	sMousePositionsBuffer.AddMarker(0);
}

void InternalNewFrame() {
	sKeys.Advance();

	sMouseButtons.Advance();
	sMouseScrolls.AdvanceAllMarkers();
	sMousePositionsBuffer.AdvanceAllMarkers();

	for (u8 i = 0; i < sGamepadsButtons.size(); i++) {
		sGamepadsButtons[i].second.Advance();
	}

	for (size_t i = 0; i < sGamepadAxisLeftStickBuffers.size(); i++) {
		sGamepadAxisLeftStickBuffers[i].second.AdvanceAllMarkers();
	}
	for (size_t i = 0; i < sGamepadAxisRightStickBuffers.size(); i++) {
		sGamepadAxisRightStickBuffers[i].second.AdvanceAllMarkers();
	}
	for (size_t i = 0; i < sGamepadAxisTriggersBuffers.size(); i++) {
		sGamepadAxisTriggersBuffers[i].second.AdvanceAllMarkers();
	}
}

void InternalShutdown() {
	sKeys.Clear();

	sMouseButtons.Clear();
	sMouseScrolls.Reset();
	sMousePositionsBuffer.Reset();

	for (u8 i = 0; i < sGamepadsButtons.size(); i++) {
		sGamepadsButtons[i].second.Clear();
	}
	sGamepadsButtons.clear();

	for (size_t i = 0; i < sGamepadAxisLeftStickBuffers.size(); i++) {
		sGamepadAxisLeftStickBuffers[i].second.RemoveAllMarkers();
	}
	sGamepadAxisLeftStickBuffers.clear();
	for (size_t i = 0; i < sGamepadAxisRightStickBuffers.size(); i++) {
		sGamepadAxisRightStickBuffers[i].second.RemoveAllMarkers();
	}
	sGamepadAxisRightStickBuffers.clear();
	for (size_t i = 0; i < sGamepadAxisTriggersBuffers.size(); i++) {
		sGamepadAxisTriggersBuffers[i].second.RemoveAllMarkers();
	}
	sGamepadAxisTriggersBuffers.clear();
}

void InternalKeyChanged(const QKey aKey, const QKeyState aKeyState) {
#ifdef _QDEBUG
	++s_InputsCount;
#endif // _QDEBUG
	sKeys.Write(aKey, aKeyState);

	if (QKeyState::e_KeyStateDown == aKeyState) {
		if (QKey::e_1 == aKey) {
			sEventCallbackKeys.Invoke(aKey, aKeyState);
			LOG_INFO("Invoke");
		}
		else if (QKey::e_2 == aKey) {
			TestHandle = sEventCallbackKeys += OnInput;
			LOG_INFO("Register");
		}
		else if (QKey::e_3 == aKey) {
			sEventCallbackKeys -= TestHandle;
			// handle.Unregister();
			LOG_INFO("Unregister");
		}
	}

	for (u16 i = 0; i < sKeyCallbacks.size(); i++) {
		sKeyCallbacks[i](aKey, aKeyState);
	}
}

void InternalMouseMove(const vec2f& aNewPosition) {
#ifdef _QDEBUG
	++s_InputsCount;
#endif // _QDEBUG
	sMousePositionsBuffer.Write(aNewPosition);

	for (u16 i = 0; i < sMouseCallbacks.size(); i++) {
		sMouseCallbacks[i](e_MouseMove, QKeyState::e_KeyStateDown, 0.f, aNewPosition);
	}
}

void InternalMouseButton(const QKey aKey, const QKeyState aKeyState) {
#ifdef _QDEBUG
	++s_InputsCount;
#endif // _QDEBUG
	sMouseButtons.Write(aKey, aKeyState);

	for (u16 i = 0; i < sMouseCallbacks.size(); i++) {
		sMouseCallbacks[i](aKey, aKeyState, 0.f, {});
	}
}

void InternalMouseScroll(const double aOffsetX, const double aOffsetY) {
#ifdef _QDEBUG
	++s_InputsCount;
#endif // _QDEBUG
	sMouseScrolls.Write(aOffsetY); // #TODO Look at how to use the aOffsetX

	for (u16 i = 0; i < sMouseCallbacks.size(); i++) {
		const QKey scrollKey = aOffsetY >= 0.f ? QKey::e_ScrollUp : QKey::e_ScrollDown;
		sMouseCallbacks[i](scrollKey, QKeyState::e_KeyStateDown, aOffsetY, {});
	}
}

void InternalGamepadAxis(const unsigned char aAxisId, const vec2f aAxisValue, const QGamepad aGamepadId) {
#ifdef _QDEBUG
	++s_InputsCount;
#endif // _QDEBUG
	for (u8 i = 0; i < sGamepadAxisLeftStickBuffers.size(); i++) {
		if (aGamepadId == sGamepadAxisLeftStickBuffers[i].first) {
			switch (aAxisId) {
			case 0:
			case 1:
				sGamepadAxisLeftStickBuffers[i].second.Write(aAxisValue); // #TODO Y axis is inverted. Should I invert it again here?

				for (u16 i = 0; i < sGamepadCallbacks.size(); i++) {
					sGamepadCallbacks[i](QGamepad::e_GamepadAxis01, QKeyState::e_KeyStateDown, sGamepadAxisLeftStickBuffers[i].second.ReadTop(), {}, {}, aGamepadId);
				}
				break;
			case 2:
			case 3:
				sGamepadAxisRightStickBuffers[i].second.Write(aAxisValue);

				for (u16 i = 0; i < sGamepadCallbacks.size(); i++) {
					sGamepadCallbacks[i](QGamepad::e_GamepadAxis23, QKeyState::e_KeyStateDown, {}, sGamepadAxisRightStickBuffers[i].second.ReadTop(), {}, aGamepadId);
				}
				break;
			case 4:
			case 5:
				sGamepadAxisTriggersBuffers[i].second.Write(aAxisValue);

				for (u16 i = 0; i < sGamepadCallbacks.size(); i++) {
					sGamepadCallbacks[i](QGamepad::e_GamepadAxis45, QKeyState::e_KeyStateDown, {}, {}, sGamepadAxisTriggersBuffers[i].second.ReadTop(), aGamepadId);
				}
				break;
			}
		}
	}
}

void InternalGamepadButton(const QGamepad aKey, const QKeyState aKeyState, const QGamepad aGamepadId) {
#ifdef _QDEBUG
	++s_InputsCount;
#endif // _QDEBUG
	ASSERT(QGamepad::e_GamepadId0 <= aGamepadId && QGamepad::e_QGamepadIdMax > aGamepadId, "Invalid aGamepadId!");
	for (u8 i = 0; i < sGamepadsButtons.size(); i++) {
		if (aGamepadId == sGamepadsButtons[i].first) {
			sGamepadsButtons[i].second.Write(aKey, aKeyState);
			break;
		}
	}

	for (u16 i = 0; i < sGamepadCallbacks.size(); i++) {
		sGamepadCallbacks[i](aKey, aKeyState, {}, {}, {}, aGamepadId);
	}
}

template <typename T>
bool Local_KeyChanged(T aBitRingBuffer, const QKey aKey, const QKeyState e_KeyState) {
	ASSERT(QKey::e_QKeyMax > aKey, "Invalid QKey!");

	switch (aKey) {
	case QKey::e_Any:
		return aBitRingBuffer.KeyThisFrame(aKey, e_KeyState, true);

	case QKey::e_CtrlAny:
		return aBitRingBuffer.KeyThisFrame(QKey::e_CtrlL, e_KeyState) || aBitRingBuffer.KeyThisFrame(QKey::e_CtrlR, e_KeyState);
	case QKey::e_ShiftAny:
		return aBitRingBuffer.KeyThisFrame(QKey::e_ShiftL, e_KeyState) || aBitRingBuffer.KeyThisFrame(QKey::e_ShiftR, e_KeyState);
	case QKey::e_AltAny:
		return aBitRingBuffer.KeyThisFrame(QKey::e_AltL, e_KeyState) || aBitRingBuffer.KeyThisFrame(QKey::e_AltR, e_KeyState);
	}
	return aBitRingBuffer.KeyThisFrame(aKey, e_KeyState);
}

bool KeyPressed(const QKey aKey) {
	return Local_KeyChanged(sKeys, aKey, e_KeyStateDown);
}

bool KeyReleased(const QKey aKey) {
	return Local_KeyChanged(sKeys, aKey, e_KeyStateUp);
}

bool MousePressed(const QKey aKey) {
	return Local_KeyChanged(sMouseButtons, aKey, e_KeyStateDown);
}

bool MouseReleased(const QKey aKey) {
	return Local_KeyChanged(sMouseButtons, aKey, e_KeyStateDown);
}

bool MouseScrolled() {
	return sMouseScrolls.HeadIndex() != sMouseScrolls.MarkerPosition(sLastFrameEndMarkerId);
}

float MouseScrollDelta() {
	if (MouseScrolled()) {
		return sMouseScrolls.ReadTop();
	}
	return 0.f;
}

vec2f MousePos() {
	return sMousePositionsBuffer.ReadTop();
}

bool MouseMoved() {
	return sMousePositionsBuffer.HeadIndex() != sMousePositionsBuffer.MarkerPosition(sLastFrameEndMarkerId);
}

vec2f MouseDelta() {
	if (MouseMoved()) {
		u2 index = sMousePositionsBuffer.HeadIndex();
		index.value -= 1;
		vec2f thisFrame = sMousePositionsBuffer.ReadRandom(index.value);
		index.value -= 1;
		return thisFrame - sMousePositionsBuffer.ReadRandom(index.value);
	}
	return vec2f(0.f, 0.f);
}

bool GamepadPressed(const QGamepad aKey, const QGamepad aGamepadId) {
	ASSERT(QGamepad::e_GamepadId0 <= aGamepadId && QGamepad::e_QGamepadIdMax > aGamepadId, "Invalid aGamepadId!");
	for (u8 i = 0; i < sGamepadsButtons.size(); i++) {
		if (aGamepadId == sGamepadsButtons[i].first) {
			sGamepadsButtons[i].second.KeyThisFrame(aKey, QKeyState::e_KeyStateDown, QGamepad::e_GamepadAny == aKey);
		}
	}
	// LOG_WARN("aGamepadId not found!");
	return false;
}

bool GamepadReleased(const QGamepad aKey, const QGamepad aGamepadId) {
	ASSERT(QGamepad::e_GamepadId0 <= aGamepadId && QGamepad::e_QGamepadIdMax > aGamepadId, "Invalid aGamepadId!");
	for (u8 i = 0; i < sGamepadsButtons.size(); i++) {
		if (aGamepadId == sGamepadsButtons[i].first) {
			sGamepadsButtons[i].second.KeyThisFrame(aKey, QKeyState::e_KeyStateUp, QGamepad::e_GamepadAny == aKey);
		}
	}
	// LOG_WARN("aGamepadId not found!");
	return false;
}

vec2f GamepadAxis(const int aAxisIndex, const QGamepad aGamepadId) {
	ASSERT(QGamepad::e_QGamepadAxisIndexFirst <= aAxisIndex && QGamepad::e_QGamepadAxisIndexMax >= aAxisIndex, "Invalid axis");
	ASSERT(QGamepad::e_GamepadId0 <= aGamepadId && QGamepad::e_QGamepadIdMax >= aGamepadId, "Invalid aGamepadId!");
	// #TODO What is more intuitive, always returning a vec2f, or individual floats? Which would users prefer?

	for (u8 i = 0; i < sGamepadAxisLeftStickBuffers.size(); i++) {
		if (aGamepadId == sGamepadAxisLeftStickBuffers[i].first) {
			switch (aAxisIndex) {
			case 0:
			case 1:
				return sGamepadAxisLeftStickBuffers[i].second.ReadTop();
			case 2:
			case 3:
				return sGamepadAxisLeftStickBuffers[i].second.ReadTop();
			case 4:
			case 5:
				return sGamepadAxisLeftStickBuffers[i].second.ReadTop();
			}
		}
	}

	// LOG_WARN("aGamepadId not found!");
	return vec2f(0.f, 0.f);
}

bool GamepadAxisMoved(const int aAxisIndex, const QGamepad aGamepadId) {
	ASSERT(QGamepad::e_QGamepadAxisIndexFirst <= aAxisIndex && QGamepad::e_QGamepadAxisIndexMax > aAxisIndex, "Invalid axis");
	ASSERT(QGamepad::e_GamepadId0 <= aGamepadId && QGamepad::e_QGamepadIdMax > aGamepadId, "Invalid aGamepadId!");
	for (u8 i = 0; i < sGamepadAxisLeftStickBuffers.size(); i++) {
		if (aGamepadId == sGamepadAxisLeftStickBuffers[i].first) {
			switch (aAxisIndex) {
			case 0:
			case 1:
				return sGamepadAxisLeftStickBuffers[i].second.HeadIndex() != sGamepadAxisLeftStickBuffers[i].second.MarkerPosition(sLastFrameEndMarkerId);
			case 2:
			case 3:
				return sGamepadAxisRightStickBuffers[i].second.HeadIndex() != sGamepadAxisRightStickBuffers[i].second.MarkerPosition(sLastFrameEndMarkerId);
			case 4:
			case 5:
				return sGamepadAxisTriggersBuffers[i].second.HeadIndex() != sGamepadAxisTriggersBuffers[i].second.MarkerPosition(sLastFrameEndMarkerId);
			}
		}
	}
	// LOG_WARN("aGamepadId not found!");
	return false;
}

void OnKey(const KeyCallback& aOnKeyFunction) {
	for (u16 i = 0; i < sKeyCallbacks.size(); i++) {
		 // #TODO Check fails for multiple globally defined or static functions try to register
		if (aOnKeyFunction.target_type() == sKeyCallbacks[i].target_type()) {
			LOG_WARN("KeyCallback event already registered!");
			return;
		}
	}
	sKeyCallbacks.push_back(aOnKeyFunction);
}

void OnKeyStop(const KeyCallback& aOnKeyFunction) {
	for (u16 i = 0; i < sKeyCallbacks.size(); i++) {
		if (aOnKeyFunction.target_type() == sKeyCallbacks[i].target_type()) {
			sKeyCallbacks.erase(sKeyCallbacks.begin() + i);
			return;
		}
	}
	LOG_WARN("Could not remove KeyCallback event!");
}

void OnMouse(const MouseCallback& aOnMouseFunction) {
	for (u16 i = 0; i < sMouseCallbacks.size(); i++) {
		if (aOnMouseFunction.target_type() == sMouseCallbacks[i].target_type()) {
			LOG_WARN("MouseCallback event already registered!");
			return;
		}
	}
	sMouseCallbacks.push_back(aOnMouseFunction);
}

void OnMouseStop(const MouseCallback& aOnMouseFunction) {
	for (u16 i = 0; i < sMouseCallbacks.size(); i++) {
		if (aOnMouseFunction.target_type() == sMouseCallbacks[i].target_type()) {
			sMouseCallbacks.erase(sMouseCallbacks.begin() + i);
			return;
		}
	}
	LOG_WARN("Could not remove MouseCallback event!");
}

void OnGamepad(const GamepadCallback& aOnGamepadFunction) {
	for (u16 i = 0; i < sGamepadCallbacks.size(); i++) {
		if (aOnGamepadFunction.target_type() == sGamepadCallbacks[i].target_type()) {
			LOG_WARN("GamepadCallback event already registered!");
			return;
		}
	}
	sGamepadCallbacks.push_back(aOnGamepadFunction);
}

void OnGamepadStop(const GamepadCallback& aOnGamepadFunction) {
	for (u16 i = 0; i < sGamepadCallbacks.size(); i++) {
		if (aOnGamepadFunction.target_type() == sGamepadCallbacks[i].target_type()) {
			sGamepadCallbacks.erase(sGamepadCallbacks.begin() + i);
			return;
		}
	}
	LOG_WARN("Could not remove GamepadCallback event!");
}

}

}
