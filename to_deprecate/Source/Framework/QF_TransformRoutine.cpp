#include "QF_TransformRoutine.h"

#include "QF_GameObject.h"

namespace QwerkE {

    void TransformRoutine::Initialize()
    {
        m_pParent->AddUpdateRoutine(this);
        m_Type = eRoutineTypes::Routine_Transform;
    }

    void TransformRoutine::Update(double a_Deltatime)
    {
        if (!m_pParent || !m_Enabled) return;

        const vec3& rotation = m_pParent->GetRotation();

        m_pParent->SetPosition(m_pParent->GetPosition() + (m_PositionOffset * m_Speed));
        m_pParent->SetRotation(rotation + (m_RotationOffset * m_Speed));
        m_pParent->SetScale(m_pParent->GetScale() + (m_ScaleOffset * m_Speed));

        if (rotation.x >= 360.f)
        {
            m_pParent->SetRotation(vec3(rotation.x - 360.f, rotation.y, rotation.z));
        }
        else if (rotation.x < 0.f)
        {
            m_pParent->SetRotation(vec3(rotation.x + 360.f, rotation.y, rotation.z));
        }

        if (rotation.y >= 360.f)
        {
            m_pParent->SetRotation(vec3(rotation.x, rotation.y - 360.f, rotation.z));
        }
        else if (rotation.y < 0.f)
        {
            m_pParent->SetRotation(vec3(rotation.x, rotation.y + 360.f, rotation.z));
        }

        if (rotation.z >= 360.f)
        {
            m_pParent->SetRotation(vec3(rotation.x, rotation.y, rotation.z - 360.f));
        }
        else if (rotation.z < 0.f)
        {
            m_pParent->SetRotation(vec3(rotation.x, rotation.y, rotation.z + 360.f));
        }
    }

}
