#pragma once

namespace QwerkE {

    namespace Time {

        float LastEndFrameNow();

		class Timer
		{
		public:
            Timer(float duration) { m_Duration = duration; Start(); }

            void Start() { m_StartTime = LastEndFrameNow(); }

            bool Expired() { return (LastEndFrameNow() - m_StartTime) >= m_Duration; }

            void Reset() { m_StartTime = LastEndFrameNow(); }

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

        // TODO: Add methods: UpdateDeltaTime(), DeltaTime(), GetSystemTime(), GetFixedTimestep()

        // #TODO Register timers for call backs, instead of polling everywhere. Can order callbacks to only poll 1 at a time.
    }

}

