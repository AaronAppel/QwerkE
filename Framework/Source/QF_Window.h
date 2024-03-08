#pragma once

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

        // #TODO Callbacks
        // void RegisterResizeCallback(resizeCallback); // Used by Renderer
        // void RegisterKeyCallback(keyCallback);       // Used by Input
    }

}
