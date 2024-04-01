#include "QE_EditComponent.h"

#include <map>
#include <string>
#include <vector>

#include "imgui/imgui.h"

#include "QC_StringHelpers.h"

#include "QF_Enums.h"
#include "QF_GameObject.h"

namespace QwerkE {

    void EditComponent::Draw(GameObject* entity)
    {
        if (m_LastEntity != entity)
        {
            m_SelectedRenderableIndex = 0;
        }

        m_LastEntity = entity;
    }

}
