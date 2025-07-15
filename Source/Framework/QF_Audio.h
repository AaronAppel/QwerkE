#pragma once

namespace QwerkE {

    namespace Audio {

        // #TODO External API for creating new "AudioWorld"s per scene with a reference getter/return.
        // Could be private for scene classes
		class AudioWorld
		{
		public:
			AudioWorld() = default;

		private:
            int m_Placeholder = 0;
		};

        void Initialize();
        void Shutdown();

        void PlaySound(const char* name);

        void SetListenerOrientation(vec3f a_Position, vec3f a_Velocity);

    }

}

