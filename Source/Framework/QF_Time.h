#pragma once

namespace QwerkE {

    class Time
    {
    public:

        static void Initialize();

        static void NewFrame();

        static inline float Delta() { return m_Delta; };

        static float Now();
        static float StartTime() { return m_StartTime; }

		// #TODO This should be in window or some other engine section
        // static void SetMaxFrameRate(float framesPerSecond);
		// static float GetMaxFrameRate();

        // TODO: Make class static
        // TODO: Add methods: UpdateDeltaTime(), DeltaTime(), GetSystemTime(), GetFixedTimestep()

    private:
        Time() {}
        ~Time() {}

        static float m_StartTime;
        static float m_Delta;
        static float m_CurrentFrame;
        static float m_LastFrame;
    };

}

