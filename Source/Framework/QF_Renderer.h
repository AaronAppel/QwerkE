#pragma once

#include "QC_Vector.h"

namespace QwerkE {

    namespace Renderer {

        void Initialize(); // #TODO Review any missing shutdown logic

        void NewFrame();

        void ClearScreen();

        void DrawFont(const char* text, float x = 100.f, float y = 100.f, float scale = 1.f);

        const void OnWindowResized(const vec2& newWindowSize);

        const vec2& WindowSize();
    }

}

