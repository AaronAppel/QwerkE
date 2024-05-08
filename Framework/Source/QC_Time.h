#pragma once

namespace QwerkE { // #TODO Doesn't need to be in the QwerkE namespace

    namespace Time {

        // #TODO Manage game time and real time separately

        double Now();

		class Timer
		{
		public:
            Timer() = default;
            Timer(double duration) { m_Duration = duration; Start(); }

            void Start() { m_StartTime = Now(); }

            bool Expired() { return (Now() - m_StartTime) >= m_Duration; }

            void SetDuration(double newDuration) { m_Duration = newDuration; }

		private:
            double m_StartTime = 0.;
            double m_Duration = 0.;
		};

        // #TODO TrackGameTime differently, using OnGamePaused(bool paused = true/false). Allows lifetimes to be game time relative

        void WriteAppStartTime();
        const double& AppStartTime();

        void SetMaximumFramerate(uint16_t maximumFramerate);

        bool ShouldProcessNextFrame();
        void StartFrame();

        const double& PreviousFrameDuration();

        float NowFloat();

        // TODO: Add methods: UpdateDeltaTime(), DeltaTime(), GetSystemTime(), GetFixedTimestep()

        // #TODO Register timers for call backs, instead of polling everywhere. Can order callbacks to only poll 1 at a time.
    }

}

