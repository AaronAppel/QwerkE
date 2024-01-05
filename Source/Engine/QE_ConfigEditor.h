#pragma once

#include <string>

#include "QF_QwerkE_Global_Constants.h"

namespace QwerkE {

    class ConfigEditor
    {
    public:

        void Draw();

    private:
        std::string m_CurrentConfigFile = gc_DefaultStringValue;
    };

}
