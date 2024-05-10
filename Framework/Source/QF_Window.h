#pragma once

#include "QF_Callbacks.h"

namespace QwerkE {

    namespace Window {

        void Initialize(u16 windowWidth = 1920, u16 windowHeight = 1080);
        void Shutdown();

        void NewFrame();

        void RequestClose();
        bool CloseRequested();

        const vec2f GetSize(); // #TODO Change to integers like vec2u16
        float GetAspectRatio();

        void* GetContext();

        bool IsMinimized();

        void RegisterFramebufferResizedCallback(CallBacks::FramebufferResizedCallback* framebufferResizedCallback);
        void RegisterWindowResizedCallback(CallBacks::WindowResizedCallback* windowResizedCallback);

        void RegisterKeyCallback(CallBacks::KeyCallback* keyCallback);
    }

}
