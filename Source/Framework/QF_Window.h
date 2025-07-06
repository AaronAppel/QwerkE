#pragma once

#include "QF_Callbacks.h"

namespace QwerkE {

    namespace Framework {
        struct StartUpData;
    }

    namespace Window {

        void Initialize(const Framework::StartUpData& startUpData);
        void Shutdown();

        void NewFrame();

        void RequestClose();
        bool CloseRequested();

        const vec2f GetSize(); // #TODO Change to integers like vec2u16
        const vec2f GetPosition(); // #TODO Change to integers like vec2u16
        float GetAspectRatio();

        void* GetContext();

        bool IsMinimized();

        void RegisterFramebufferResizedCallback(Callbacks::FramebufferResizedCallback* framebufferResizedCallback);
        void RegisterWindowResizedCallback(Callbacks::WindowResizedCallback* windowResizedCallback);

        // void RegisterKeyCallback(Callbacks::KeyCallback* keyCallback);

#if 1 // #TODO Omit from retail builds
        void RegisterFileDropCallback(Callbacks::FileDropCallback* fileDropCallback);
#endif
    }

}
