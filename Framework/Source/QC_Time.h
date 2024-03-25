#pragma once

namespace QwerkE {

    namespace Time {

        // #TODO Manage game time and real time separately

        double Now();

		class Timer
		{
		public:
            Timer() = default;
            Timer(float duration) { m_Duration = duration; Start(); }

            void Start() { m_StartTime = Now(); }

            bool Expired() { return (Now() - m_StartTime) >= m_Duration; }

            void SetDuration(float newDuration) { m_Duration = newDuration; }

		private:
            double m_StartTime = 0.;
            float m_Duration = 0.f;
		};

        // #TODO TrackGameTime differently, using OnGamePaused(bool paused = true/false). Allows lifetimes to be game time relative

        void WriteAppStartTime();
        const double& AppStartTime();

        void SetMaximumFramerate(u16 maximumFramerate);

        bool ShouldProcessNextFrame();
        void StartFrame();

        const double& PreviousFrameDuration();

        float NowFloat();

        // TODO: Add methods: UpdateDeltaTime(), DeltaTime(), GetSystemTime(), GetFixedTimestep()

        // #TODO Register timers for call backs, instead of polling everywhere. Can order callbacks to only poll 1 at a time.
    }

}

