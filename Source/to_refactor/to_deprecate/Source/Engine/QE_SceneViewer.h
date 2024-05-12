#pragma once

namespace QwerkE {

    class FrameBufferObject;

    class SceneViewer // #TODO Look at simplifying and deprecating class. Move to another editor class
    {
    public:
        SceneViewer();
        ~SceneViewer();

        void Draw();

    private:
        void DrawSceneView();
        void DrawSceneList();

        FrameBufferObject* m_FBO = nullptr;
        int m_currentSceneStateIndex = 0;
    };

}
