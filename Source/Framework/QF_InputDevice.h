#pragma once

#include "QF_eKeys.h"
#include "QF_Enums.h"
#include "QF_Input.h"
#include "QF_Log.h"

namespace QwerkE {

    namespace Input {

        class InputDevice // #TODO Abstract class?
        {
        public:
            InputDevice(eInputDeviceTypes type) { m_DeviceType = type; } // #TODO Shouldn't the type be known to the derived class?

            void Initialize()
            {
                if (!s_KeyCodex)
                {
                    // #TODO Array size of GLFW_KEY_LAST is overkill
                    s_KeyCodex = new unsigned short[GLFW_KEY_LAST];
                    memset(s_KeyCodex, 0, GLFW_KEY_LAST);
                }
                else
                {
                    LOG_ERROR("", __FUNCTION__);
                }
            }

            virtual void RaiseInputEvent(eKeys key, eKeyState state);

            virtual void NewFrame();

            eInputDeviceTypes GetType() const { return m_DeviceType; }

            void SetType(eInputDeviceTypes type) { m_DeviceType = type; }

            unsigned short* GetKeyCodex() { return s_KeyCodex; }
            bool* GetKeyStates() { return s_KeyStates; }

            void SetOneFrameBufferDirty() { s_OneFrameBuffersAreDirty = true; }

            const u16* OneFrameKeyBuffer() const { return s_OneFrameKeyBuffer; }

        protected:
            u16* s_KeyCodex = nullptr;
            bool s_KeyStates[eKeys_MAX] = { false };

            bool s_OneFrameBuffersAreDirty = true;
            u16 s_OneFrameKeyBuffer[ONE_FRAME_MAX_INPUT];
            bool s_OneFrameValueBuffer[ONE_FRAME_MAX_INPUT] = { false };

            eInputDeviceTypes m_DeviceType = eInputDeviceTypes::InputDeviceTypes_Max;
        };

    }

}
