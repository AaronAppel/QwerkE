#ifndef __XinputInterface_H__
#define __XinputInterface_H__

#ifdef XinAPI_Export
#define XinAPI __declspec(dllexport)
#else
#define XinAPI __declspec(dllimport)
#endif

#define XinExternal __declspec(dllexport)

// https://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.reference.xinput_gamepad(v=vs.85).aspx <-- button codes
// https://katyscode.wordpress.com/2013/08/30/xinput-tutorial-part-1-adding-gamepad-support-to-your-windows-game/ <-- reading Xinput
// https://msdn.microsoft.com/en-ca/library/windows/desktop/ee417001(v=vs.85).aspx <- microsoft tutorial
// "The constant XINPUT_GAMEPAD_TRIGGER_THRESHOLD may be used as the value which bLeftTrigger and bRightTrigger must be greater than to register as pressed." - MSDN XINPUT_GAMEPAD structure
// XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE
#include <Xinput.h>
// #include Xinput.lib, Xinput9_1_0.lib or other version

/*  HOW TO USE
XinputHandler newHandler = new XinputHanlder(int playerCount, bool debugEnabled = false);
...
newHanlder->ResetStates();
newHanlder->SetPlayer(0);
newHanlder->UpdatePlayerState();
newHanlder->AButtonIsDown();
*/

// TODO:: increase efficiency
// Compare previous frame state to see if face buttons changed.
// If they did, queue an event, otherwise don't waste ram and processor's time
// Add option to disable auto stick+trigger event generation

// TODO: consider removing dependencies like vec2 and std::vector to make incorporation simpler
// TODO: Remove m_CurrentPlayer?

// TODO:: Finalize interface.
// Find better variable and function names. Possibly add prefixes like xin_.

#include <vector>
#include <assert.h>
#define MyAssert assert

typedef unsigned int uint;

// TODO: reference additional headers your program requires here
#include "MathHelpers.h"
#include "Vector.h"
#include "XinputInterface.h"

class XinAPI XinputHandler
{
public:
	// setup
	XinputHandler(int numPlayers, bool debugging = false);
	void SetDebugging(bool debugging) { m_Debugging = debugging; };

	// utility
	void ResetState() { ResetState(m_CurrentPlayer); };
	void ResetState(int player);
	void ResetStates();

	void SetPlayer(int player) { m_CurrentPlayer = player; };
	void SetStickDeadZone(int deadzone) { m_StickDeadZone = (float)deadzone; };
	void SetTriggerDeadZone(int deadzone) { m_TriggerDeadZone = deadzone; };

	bool UpdatePlayerState() { return UpdatePlayerState(m_CurrentPlayer); };
	bool UpdatePlayerState(int player);

	//// input
	// Sticks + Triggers
	Vector2 LeftStick() { return LeftStick(m_CurrentPlayer); };
	Vector2 LeftStick(int player);

	Vector2 RightStick() { return RightStick(m_CurrentPlayer); };
	Vector2 RightStick(int player);

	Vector2 Triggers() { return Triggers(m_CurrentPlayer); };
	Vector2 Triggers(int player);

	bool ButtonIsDown(int button, int player = 0);
	bool ButtonIsDown(int button);

	// Face
	bool AButtonIsDown() { return AButtonIsDown(m_CurrentPlayer); };
	bool AButtonIsDown(int player);

	bool BButtonIsDown() { return BButtonIsDown(m_CurrentPlayer); };
	bool BButtonIsDown(int player);

	bool XButtonIsDown() { return XButtonIsDown(m_CurrentPlayer); };
	bool XButtonIsDown(int player);

	bool YButtonIsDown() { return YButtonIsDown(m_CurrentPlayer); };
	bool YButtonIsDown(int player);
	// DPad
	bool DPadUpButtonIsDown() { return DPadUpButtonIsDown(m_CurrentPlayer); };
	bool DPadUpButtonIsDown(int player);

	bool DPadDownButtonIsDown() { return DPadDownButtonIsDown(m_CurrentPlayer); };
	bool DPadDownButtonIsDown(int player);

	bool DPadLeftButtonIsDown() { return DPadLeftButtonIsDown(m_CurrentPlayer); };
	bool DPadLeftButtonIsDown(int player);

	bool DPadRightButtonIsDown() { return DPadRightButtonIsDown(m_CurrentPlayer); };
	bool DPadRightButtonIsDown(int player);
	// Bumper
	bool BumperLeftButtonIsDown() { return BumperLeftButtonIsDown(m_CurrentPlayer); };
	bool BumperLeftButtonIsDown(int player);

	bool BumperRightButtonIsDown() { return BumperRightButtonIsDown(m_CurrentPlayer); };
	bool BumperRightButtonIsDown(int player);
	// Start + Back
	bool StartButtonIsDown() { return StartButtonIsDown(m_CurrentPlayer); };
	bool StartButtonIsDown(int player);

	bool BackButtonIsDown() { return BackButtonIsDown(m_CurrentPlayer); };
	bool BackButtonIsDown(int player);

	// tear down
	~XinputHandler();

private:
	// variables
	bool m_Debugging; // TODO: Remove?

	Vector2 m_StickDeadZone;
	int m_TriggerDeadZone;

	unsigned int m_NumPlayers;

	unsigned int m_CurrentPlayer;

	std::vector<XINPUT_STATE> m_PlayerStates;
	std::vector<XINPUT_STATE> m_PreviousStates; // reduce polling by generating events
};

#endif // __XinputInterface_H__