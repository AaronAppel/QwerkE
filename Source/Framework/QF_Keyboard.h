#pragma once

#include "QF_InputDevice.h"

namespace QwerkE {

    struct Keyboard : public InputDevice
    {
        Keyboard(eInputDeviceTypes type) : InputDevice(type) {}
        ~Keyboard() {}
    };

}
