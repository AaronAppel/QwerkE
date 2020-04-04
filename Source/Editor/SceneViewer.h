#ifndef _SceneViewer_H_
#define _SceneViewer_H_

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
#endif // _SceneViewer_H_
