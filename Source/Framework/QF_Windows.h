#pragma once

#include <vector>

#include "QC_TypeDefs.h"

namespace QwerkE {

    class Window;

    class Windows
    {
    public:
        static void AddWindow(Window* window);
        static Window* GetWindow(uint windowID);
        static const Window* GetLastFocusedWindow();

        static void Initialize();
        static void Shutdown();

    private:
        Windows() = default;

        static std::vector<Window*> m_Windows;
        static unsigned char m_lastFocusedWindowIndex;
    };

}
