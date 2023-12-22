#pragma once

namespace QwerkE {

    class FrameBufferObject;

    class SceneViewer
    {
    public:
        SceneViewer();
        ~SceneViewer();

        void NewFrame();
        void Update();
        void Draw();

    private:
        void DrawSceneView();
        void DrawSceneList();
        FrameBufferObject* m_FBO = nullptr;
    };

}
