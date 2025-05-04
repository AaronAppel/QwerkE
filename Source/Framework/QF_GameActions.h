#pragma once

#include "QF_QKey.h"

namespace QwerkE {

	namespace Input {

		struct GameActions
		{
			QKey Camera_MoveForward = QKey::e_W;
			QKey Camera_MoveBackward = QKey::e_S;
			QKey Camera_MoveLeft = QKey::e_A;
			QKey Camera_MoveRight = QKey::e_D;
			QKey Camera_MoveUp = QKey::e_E;
			QKey Camera_MoveDown = QKey::e_Q;
			QKey Camera_RotateLeft = QKey::e_R;
			QKey Camera_RotateRight = QKey::e_T;
		};

	}

}