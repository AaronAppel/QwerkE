#pragma once

#include "QC_Vector.h"

namespace QwerkE {

    class Renderer final
    {
    public:
        static int g_WindowWidth, g_WindowHeight; // #TODO No globals

        // TODO: Expose API for changing clear color, culling, etc
        static void Initialize(); // #TODO Review any missing shutdown logic

        static void NewFrame();

        static void ClearScreen();

        static void DrawFont(const char* text, float x = 100.f, float y = 100.f, float scale = 1.0f);

    private:
        Renderer() = default;
    };

}

