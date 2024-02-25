#include "QF_Mouse.h"

#include "QF_Input.h"

#include <cstring>

namespace QwerkE {

    namespace Input {

        void Mouse::NewFrame()
        {
            if (s_DragReset)
            {
                s_MouseDragStart = 0.0f;
                s_DragReset = false;
            }

            if (s_OneFrameBuffersAreDirty)
            {
                memset(s_OneFrameKeyBuffer, eKeys_NULL_KEY, ONE_FRAME_MAX_INPUT * sizeof(short));
                memset(s_OneFrameValueBuffer, 0, ONE_FRAME_MAX_INPUT); // TODO: Do I want a 3rd key state?
                s_OneFrameBuffersAreDirty = false;
            }
        }

    }

}
