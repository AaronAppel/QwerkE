#pragma once

#include "QF_QKey.h"

namespace QwerkE {

    namespace Input {

        void SendSystemInput(u16 a_KeyCode, bool a_KeyDown, u16 a_WScanCode = 0);
        void SendSystemInput(QKey a_Key, bool a_KeyDown);

    }

}
