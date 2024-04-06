#pragma once

#include "QC_Guid.h"

namespace QwerkE {

    struct ComponentInfo
    {
        const char* m_EditorDisplayName = Constants::gc_DefaultStringValue;
        GUID m_Guid;
        bool m_Enabled = false; // #TODO Here for now. Should be reviewed
    };

}
