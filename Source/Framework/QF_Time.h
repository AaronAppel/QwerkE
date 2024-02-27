#pragma once

namespace QwerkE {

    namespace Time {

        const double Now();

		class Timer
		{
		public:
            Timer(float duration) { m_Duration = duration; m_StartTime = Now(); }

            bool Expired() { return (Now() - m_StartTime) >= m_Duration; }

            bool Reset() { m_StartTime = Now(); }

		private:
            double m_StartTime = 0.;
            float m_Duration = 0.f;
		};

        void InitStartTime();
        const double& StartTime();

        void EndFrame();

        float FrameDelta();
        const double& FrameDeltaDouble();

        const double Now();

		// #TODO This should be in window or some other engine section
        // static void SetMaxFrameRate(float framesPerSecond);
		// static float GetMaxFrameRate();

        // TODO: Make class static
        // TODO: Add methods: UpdateDeltaTime(), DeltaTime(), GetSystemTime(), GetFixedTimestep()

        // #TODO Register timers for call backs, instead of polling everywhere. Can order callbacks to only poll 1 at a time.
    }

}

