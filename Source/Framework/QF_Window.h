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
        const vec2f GetPosition(); // #TODO Change to integers like vec2u16
        float GetAspectRatio();

        void* GetContext();

        bool IsMinimized();

        void RegisterFramebufferResizedCallback(Callbacks::FramebufferResizedCallback* framebufferResizedCallback);
        void RegisterWindowResizedCallback(Callbacks::WindowResizedCallback* windowResizedCallback);

        void RegisterKeyCallback(Callbacks::KeyCallback* keyCallback);

#if _QDEBUG // #TODO Omit from retail builds
        void RegisterFileDropCallback(Callbacks::FileDropCallback* fileDropCallback);

        void ToggleConsoleWindow(); // #TODO Consider moving to Debug:: Namespace
        bool IsConsoleWindowShowing(); // #TODO Consider moving to Debug:: Namespace
#endif // _QDEBUG
    }

}
