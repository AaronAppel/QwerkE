#include "QC_Time.h"

// #include "QF_Platform.h"

#ifdef _QWINDOWS
#include <windows.h>
#include <profileapi.h>
#include <winnt.h>
#else
#error "Support non windows platform!"
#endif

#include "QF_Log.h"
#include "QF_Debug.h"

namespace QwerkE {

    double s_StartTime = 0.;
    double s_FrameDelta = 0.;
    double s_CurrentFrame = 0.;
    double s_LastFrame = 0.;

    namespace Time {

        void InitStartTime()
        {
            ASSERT(!s_StartTime, "Start time has already been set!");
            s_StartTime = Now();
        }

        void EndFrame()
        {
            s_LastFrame = s_CurrentFrame;
            s_CurrentFrame = Now();
            s_FrameDelta = s_CurrentFrame - s_LastFrame;
        }

        const double Now()
        {
            unsigned __int64 freq; // #TODO Frequency can be cached to avoid future assignments, which will be often
            unsigned __int64 time;

            QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
            QueryPerformanceCounter((LARGE_INTEGER*)&time);

            return (double)time / freq;
        }

        const double& StartTime()
        {
            return s_StartTime;
        }

        float FrameDelta()
        {
            return (float)s_FrameDelta;
        };

        const double& FrameDeltaDouble()
        {
            return s_FrameDelta;
        };

    }

}
