#pragma once

#include <vector>

#include "QC_TypeDefs.h"

namespace QwerkE {

    class Window;

    class Windows
    {
    public:
        static Window* GetWindow(uint windowID);

        static void Initialize();
        static void Shutdown();

        static void Render();

    private:
        Windows() = default;

        static std::vector<Window*> m_Windows;
        static unsigned char m_lastFocusedWindowIndex;
    };

}
