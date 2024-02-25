#pragma once

#include "QF_eKeys.h"

namespace QwerkE {

    namespace Input {

        struct Mouse;

        class MouseExtAPI
        {
        public:
            MouseExtAPI(Mouse* mouse) { m_Mouse = mouse; }
            ~MouseExtAPI() {}

            bool GetIsKeyDown(eKeys key) const;

            bool FrameAction(eKeys key, eKeyState state) const;

        private:
            Mouse* m_Mouse = nullptr;
        };

    }

}
