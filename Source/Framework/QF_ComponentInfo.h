#pragma once

#include "QC_Guid.h"

namespace QwerkE {

    struct ComponentInfo
    {
        ComponentInfo() = default;
        ComponentInfo(const GUID& existingGuid) : m_Guid(existingGuid) { }

        std::string m_EntityName = Constants::gc_DefaultStringValue;
        GUID m_Guid;
        bool m_Enabled = false; // #TODO Here for now. Should be reviewed
    };

}
