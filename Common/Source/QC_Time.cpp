#include "QC_Time.h"

// #TODO C++ or platform agnostic time solution : https://stackoverflow.com/questions/2808398/easily-measure-elapsed-time
#ifdef _QWINDOWS
#include <windows.h>
#include <profileapi.h>
#include <winnt.h>
#else
#error "Support non windows platform!"
#endif

#include <assert.h>

namespace QwerkE {

    static double s_AppStartTime = 0.;
    static double s_FrameDelta = 0.;

    static double s_MinimumtimeBetweenFrames = 0.;
    static double s_LatestFrameStartTime = 0.;
    static double s_PreviousFrameDuration = 0.;

    namespace Time {

        void WriteAppStartTime()
        {
            assert(!s_AppStartTime && "App start time has already been set");
            s_AppStartTime = Now();
            s_LatestFrameStartTime = s_AppStartTime;
        }

        void SetMaximumFramerate(unsigned short maximumFramerate)
        {
            assert(maximumFramerate > 0 && "Maximum framerate must be greater than 0!");
            s_MinimumtimeBetweenFrames = 1.0 / (double)maximumFramerate;
        }

        bool ShouldProcessNextFrame()
        {
            const double now = Now();
            return (now - s_LatestFrameStartTime) >= s_MinimumtimeBetweenFrames;
        }

        void StartFrame()
        {
            const double now = Now();
            s_PreviousFrameDuration = now - s_LatestFrameStartTime;
            s_LatestFrameStartTime = now;
        }

        const double& AppStartTime()
        {
            return s_AppStartTime;
        }

        const double& PreviousFrameDuration()
        {
            return s_PreviousFrameDuration;
        }

        double Now()
        {
            unsigned __int64 freq; // #TODO Frequency can be cached to avoid future assignments, which will be often
            unsigned __int64 time;

            QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
            QueryPerformanceCounter((LARGE_INTEGER*)&time);

            return (double)time / (double)freq;
        }

        float NowFloat()
        {
            unsigned __int64 freq; // #TODO Frequency can be cached to avoid future assignments, which will be often
            unsigned __int64 time;

            QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
            QueryPerformanceCounter((LARGE_INTEGER*)&time);

            return (float)time / (float)freq;
        }

    }

}
