#pragma once

#include "Libraries/Mirror/Source/Mirror.h"

#include "QF_Routine.h"

#include "QC_Vector.h"

namespace QwerkE {

    class TransformRoutine : public Routine
    {
    public:
        void Initialize();

        void Update(double a_Deltatime);

        float GetSpeed() { return m_Speed; }
        vec3 GetPositionOff() { return m_PositionOffset; }
        vec3 GetRotationOff() { return m_RotationOffset; }
        vec3 GetScaleOff() { return m_ScaleOffset; }

        void SetSpeed(float speed) { m_Speed = speed; }
        void SetPositionOff(vec3 pos) { m_PositionOffset = pos; }
        void SetRotationOff(vec3 rot) { m_RotationOffset = rot; }
        void SetScaleOff(vec3 scale) { m_ScaleOffset = scale; }

    private:
        MIRROR_PRIVATE_MEMBERS

        float m_Speed = 1.0f;

        vec3 m_PositionOffset = vec3(0, 0, 0);
        vec3 m_RotationOffset = vec3(0, 0, 0);
        vec3 m_ScaleOffset = vec3(0, 0, 0);
    };

}
