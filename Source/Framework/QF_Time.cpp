#include "QF_Time.h"

#include "QF_Log.h"

#ifdef _WIN32
#include <profileapi.h>
#include <winnt.h>
#else
#error "Support non windows platform!"
#endif

namespace QwerkE {

    double Time::m_StartTime = 0.f;
    double Time::m_FrameDelta = 0.f;
    double Time::m_CurrentFrame = 0.f;
    double Time::m_LastFrame = 0.f;

    void Time::InitStartTime()
    {
        ASSERT(!m_StartTime, "Start time has already been set!");
        m_StartTime = Now();
    }

    void Time::EndFrame()
    {
        m_LastFrame = m_CurrentFrame;
        m_CurrentFrame = Now();
        m_FrameDelta = m_CurrentFrame - m_LastFrame;
    }

    double Time::Now()
    {
        unsigned __int64 freq; // #TODO Frequency can be cached to avoid future assignments, which will be often
        unsigned __int64 time;

        QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
        QueryPerformanceCounter((LARGE_INTEGER*)&time);

        return (double)time / freq;
    }

}
