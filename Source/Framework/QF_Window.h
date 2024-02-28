#pragma once

namespace QwerkE {

    namespace Window {

        void Initialize();
        void Shutdown();

        void Render();
        void SwapBuffers(); // #TODO Only for loading font currently, so try to remove

        void NewFrame();

        void RequestClose();
        bool CloseRequested();

        vec2 GetResolution();
        vec2 GetAspectRatio();

        void* GetContext();

        bool IsMinimized();

    }

}
