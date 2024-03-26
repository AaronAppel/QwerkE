#pragma once

#include "QF_Callbacks.h"

namespace QwerkE {

    namespace Window {

        void Initialize();
        void Shutdown();

        void ImGuiRender();

        void NewFrame();

        void RequestClose();
        bool CloseRequested();

        const vec2 GetSize();
        float GetAspectRatio();

        void* GetContext();

        bool IsMinimized();

        void RegisterFramebufferResizedCallback(CallBacks::FramebufferResizedCallback* framebufferResizedCallback);
        void RegisterWindowResizedCallback(CallBacks::WindowResizedCallback* windowResizedCallback);

        void RegisterKeyCallback(CallBacks::KeyCallback* keyCallback);
    }

}
