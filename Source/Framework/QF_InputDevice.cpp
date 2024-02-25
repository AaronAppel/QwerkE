#include "QF_InputDevice.h"

#include "QF_Input.h"

namespace QwerkE {

    namespace Input {

        void InputDevice::RaiseInputEvent(eKeys key, eKeyState state)
        {
            s_OneFrameBuffersAreDirty = true;

            for (int i = 0; i < ONE_FRAME_MAX_INPUT; i++)
            {
                if (s_OneFrameKeyBuffer[i] == eKeys_NULL_KEY)
                {
                    s_OneFrameKeyBuffer[i] = key;
                    s_OneFrameValueBuffer[i] = state;
                    return;
                }
            }
        }

        void InputDevice::NewFrame()
        {
            if (s_OneFrameBuffersAreDirty)
            {
                // m_MouseDragStart = 0.0f;
                memset(s_OneFrameKeyBuffer, eKeys_NULL_KEY, ONE_FRAME_MAX_INPUT * sizeof(short));
                memset(s_OneFrameValueBuffer, 0, ONE_FRAME_MAX_INPUT);
                s_OneFrameBuffersAreDirty = false;
            }
        }
    }

}
