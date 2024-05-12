#pragma once

#include <string>

#include "QF_Constants.h"

namespace QwerkE {

    class Asset_Tag;

    class Asset
    {
    public:
        std::string GetName() { return m_Name; }

    protected:
        Asset() = default;

        std::string m_Name = gc_DefaultStringValue;
        Asset_Tag* m_Tag = nullptr;
    };

}
