#pragma once

#include "QF_eKeys.h"

namespace QwerkE {

	namespace Input {

		struct GameActions
		{
			eKeys Camera_MoveForward = eKeys::eKeys_W;
			eKeys Camera_MoveBackward = eKeys::eKeys_S;
			eKeys Camera_MoveLeft = eKeys::eKeys_A;
			eKeys Camera_MoveRight = eKeys::eKeys_D;
			eKeys Camera_MoveUp = eKeys::eKeys_E;
			eKeys Camera_MoveDown = eKeys::eKeys_Q;
			eKeys Camera_RotateLeft = eKeys::eKeys_R;
			eKeys Camera_RotateRight = eKeys::eKeys_T;
		};

	}

}