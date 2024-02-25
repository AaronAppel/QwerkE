#include "QF_KeyboardExtApi.h"

#include "QF_Keyboard.h"

#include "QF_Input.h"

namespace QwerkE {

    namespace Input {

        bool KeyboardExtAPI::GetIsKeyDown(eKeys key) const
        {
            return m_Keyboard->GetKeyStates()[key];
        }

        bool KeyboardExtAPI::FrameAction(eKeys key, eKeyState state) const
        {
            if (m_Keyboard->OneFrameKeyBuffer()[0] != eKeys::eKeys_NULL_KEY)
            {
                for (int i = 0; i < ONE_FRAME_MAX_INPUT; i++)
                {
                    if (m_Keyboard->OneFrameKeyBuffer()[i] == key)
                        return m_Keyboard->OneFrameKeyBuffer()[i] == (bool)state;
                }
            }
            return false;
        }

    }

}
