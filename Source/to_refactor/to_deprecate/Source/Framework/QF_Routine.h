#pragma once

#include "Libraries/Mirror/Source/Mirror.h"

#include "QF_Enums.h"
#include "QF_GameObject.h"

namespace QwerkE {

    class Routine
    {
    public:
        virtual ~Routine() = default;

        virtual void Update(double a_Deltatime) {};

        virtual void Draw(GameObject* a_Camera) {};

        virtual void Initialize() = 0;

        virtual void Activate() { m_Enabled = true; }
        virtual void Deactivate() { m_Enabled = false; }

        virtual void CleanUp() {}; // #TODO Review. CleanUp() is used to detach routines

        eRoutineTypes GetRoutineType() { return m_Type; };
        GameObject* GetParent() { return m_pParent; };

        virtual void SetParent(GameObject* a_Parent) { m_pParent = a_Parent; };
        void SetRoutineType(eRoutineTypes type) { m_Type = type; };

    protected:
        MIRROR_PRIVATE_MEMBERS

        Routine() = default;

        bool m_Enabled = true;
        GameObject* m_pParent = nullptr;
        eRoutineTypes m_Type = eRoutineTypes::Routine_Null;
    };

}
