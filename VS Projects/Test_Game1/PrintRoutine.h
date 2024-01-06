#ifndef _PrintRoutine_H_
#define _PrintRoutine_H_

#include "../../../QwerkE_Framework/Source/Core/Scenes/Entities/Routines/Routine.h"
#include "PrintComponent.h"

class PrintComponent;

class PrintRoutine : Routine
{
public:
    PrintRoutine();
    ~PrintRoutine();

    void Initialize();

    void Update(double a_Deltatime);

private:
    float m_PrintPeriod = 3.0f;
    float m_TimePassed = m_PrintPeriod;

    PrintComponent* m_pPrint = nullptr;
};

#endif // !_PrintRoutine_H_
