#pragma once

namespace QwerkE {

    class Time
    {
    public:

        static void InitStartTime();

        static void EndFrame();

        static inline float FrameDelta() { return (float)m_FrameDelta; };
        static inline double FrameDeltaD() { return m_FrameDelta; };

        static double Now();
        static double StartTime() { return m_StartTime; }

		// #TODO This should be in window or some other engine section
        // static void SetMaxFrameRate(float framesPerSecond);
		// static float GetMaxFrameRate();

        // TODO: Make class static
        // TODO: Add methods: UpdateDeltaTime(), DeltaTime(), GetSystemTime(), GetFixedTimestep()

        // #TODO Register timers for call backs, instead of polling everywhere. Can order callbacks to only poll 1 at a time.

    private:
        Time() = default;

        static double m_StartTime;
        static double m_FrameDelta;
        static double m_CurrentFrame;
        static double m_LastFrame;
    };

}

