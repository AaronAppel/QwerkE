#pragma once


namespace QwerkE {

    namespace Audio {

        bool Initialize();
        void Shutdown();

        void PlaySound(const char* name);

        void SetListenerOrientation(vec3 position, vec3 velocity);

        bool Initialized();

    }

}

