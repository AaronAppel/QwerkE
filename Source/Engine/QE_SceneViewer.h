#pragma once

namespace QwerkE {

    class FrameBufferObject;

    class SceneViewer
    {
    public:
        SceneViewer();
        ~SceneViewer();

        void Draw();

    private:
        void DrawSceneView();
        void DrawSceneList();

        FrameBufferObject* m_FBO = nullptr;
        int m_currentEngineStateIndex = 0;
    };

}
