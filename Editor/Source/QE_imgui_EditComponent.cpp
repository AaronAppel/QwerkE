#include "QE_EditComponent.h"

#include <map>
#include <string>
#include <vector>

#include "imgui/imgui.h"

#include "QC_StringHelpers.h"

#include "QF_EntityHandle.h"
#include "QF_Enums.h"

namespace QwerkE {

    void EditComponent::Draw(const EntityHandle& entity)
    {
        if (m_LastEntity != entity)
        {
        }
        m_LastEntity = entity;
    }

}
