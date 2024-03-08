#pragma once

#include "QF_WindowCallbacks.h"

namespace QwerkE {

    namespace Window {

        void Initialize();
        void Shutdown();

        void Render();
        void SwapBuffers(); // #TODO Only for loading font currently, so try to remove. Should be in Renderer

        void NewFrame();

        void RequestClose();
        bool CloseRequested();

        vec2 GetResolution();
        vec2 GetAspectRatio();

        void* GetContext();

        bool IsMinimized();

        void RegisterFramebufferResizedCallback(FramebufferResizedCallback* framebufferResizedCallback);
        void RegisterWindowResizedCallback(WindowResizedCallback* windowResizedCallback);

        void RegisterKeyCallback(KeyCallback* keyCallback);
    }

}
