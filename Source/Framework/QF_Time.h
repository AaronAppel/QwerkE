#pragma once

namespace QwerkE {

    namespace Time {

        void InitStartTime();
        const double& StartTime();

        void EndFrame();

        const float& FrameDelta();
        const double& FrameDeltaDouble();

        const double& Now();

		// #TODO This should be in window or some other engine section
        // static void SetMaxFrameRate(float framesPerSecond);
		// static float GetMaxFrameRate();

        // TODO: Make class static
        // TODO: Add methods: UpdateDeltaTime(), DeltaTime(), GetSystemTime(), GetFixedTimestep()

        // #TODO Register timers for call backs, instead of polling everywhere. Can order callbacks to only poll 1 at a time.
    }

}

