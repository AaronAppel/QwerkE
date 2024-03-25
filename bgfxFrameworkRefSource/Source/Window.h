#pragma once

namespace Window {

    void Initialize(int windowWidth, int windowHeight);

    void Shutdown();

    void NewFrame();

    void RequestClose();
    bool CloseRequested();

    void GetWindowSize(int* x, int* y);

    void* GetContext();

    bool IsMinimized();

}
