#pragma once

#include <vector>

#include "QC_Vector.h"

namespace QwerkE {

    class Window
    {
    public:
        static void Initialize();
        static void Shutdown();

        static void Render();
        static void SwapBuffers(); // #TODO Only for loading font currently, so try to remove

        static void NewFrame();

        static void RequestClose();
        static bool CloseRequested();

        static vec2 GetResolution();
        static vec2 GetAspectRatio();

        static void* GetContext();

    private:
        Window() = default;

        static unsigned char m_lastFocusedWindowIndex;
    };

}
