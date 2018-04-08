#include "PrintRoutine.h"
#include "../Shared_Engine/GameObject/GameObject.h"

PrintRoutine::PrintRoutine()
{
}

PrintRoutine::~PrintRoutine()
{
}

void PrintRoutine::Initialize()
{
    m_pParent->AddUpdateRoutine(this);
    m_pPrint = (PrintComponent*)m_pParent->GetComponent(Component_Print);
	m_Type = eRoutineTypes::Routine_Print;
}

void PrintRoutine::Update(double a_Deltatime)
{
    m_TimePassed += a_Deltatime;
    if (m_TimePassed >= m_PrintPeriod)
    {
        m_pPrint->PrintMessage();
        m_TimePassed = 0.0f;
    }
}