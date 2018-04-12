#include "Time.h"

namespace QwerkE
{
    const double* Time::m_DeltaTime = nullptr;

    Time::Time()
    {
    }

    Time::~Time()
    {
    }

    void Time::SetDeltatime(double* deltaTime)
    {
        m_DeltaTime = deltaTime;
    }

    double Time::GetDeltaTime()
    {
        return *m_DeltaTime;
    }
}