#pragma once

#include <string>

#include "../QwerkE_Framework/Source/Headers/QwerkE_Global_Constants.h"

namespace QwerkE {

    class ConfigEditor
    {
    public:

        void Draw();

    private:
        std::string m_CurrentConfigFile = gc_DefaultStringValue;
    };

}
