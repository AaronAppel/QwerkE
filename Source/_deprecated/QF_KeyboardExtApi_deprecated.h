#pragma once

#include "QF_eKeys.h"
#include "QF_Input.h"

namespace QwerkE {

    namespace Input {

        struct Keyboard;

        class KeyboardExtAPI
        {
        public:
            KeyboardExtAPI(Keyboard* keyboard) { m_Keyboard = keyboard; }

            bool GetIsKeyDown(eKeys key) const;

            bool FrameAction(eKeys key, eKeyState state) const; // #TODO Rename

        private:
            Keyboard* m_Keyboard = nullptr;
        };

    }

}
