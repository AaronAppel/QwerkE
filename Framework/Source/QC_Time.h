#pragma once

namespace QwerkE {

    namespace Time {

        float FrameNow();

		class Timer
		{
		public:
            Timer(float duration) { m_Duration = duration; Start(); }

            void Start() { m_StartTime = FrameNow(); }

            bool Expired() { return (FrameNow() - m_StartTime) >= m_Duration; }

            void Reset() { m_StartTime = FrameNow(); }

            void SetDuration(float newDuration) { m_Duration = newDuration; }

		private:
            double m_StartTime = 0.;
            float m_Duration = 0.f;
		};

        void InitStartTime();
        const double& StartTime();

        void EndFrame();

        float FrameDelta();
        const double& FrameDeltaDouble();

        float Now();
        double NowDouble();

		// #TODO This should be in window or some other engine section
        // static void SetMaxFrameRate(float framesPerSecond);
		// static float GetMaxFrameRate();

        // TODO: Make class static
        // TODO: Add methods: UpdateDeltaTime(), DeltaTime(), GetSystemTime(), GetFixedTimestep()

        // #TODO Register timers for call backs, instead of polling everywhere. Can order callbacks to only poll 1 at a time.
    }

}

