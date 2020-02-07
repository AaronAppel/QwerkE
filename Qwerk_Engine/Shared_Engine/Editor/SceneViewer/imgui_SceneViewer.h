#ifndef _imgui_SceneViewer_H_
#define _imgui_SceneViewer_H_

#include "../QwerkE_Framework/Headers/QwerkE_Enums.h"

#include <map>

namespace QwerkE {

    class Input;
    class FrameBufferObject;
    class Scene;

    class imgui_SceneViewer
    {
    public:
        imgui_SceneViewer();
        ~imgui_SceneViewer();

        void NewFrame();
        void Update();
        void Draw();

    private:
        void DrawSceneView();
        void DrawSceneList();
        Input* m_Input = nullptr;
        FrameBufferObject* m_FBO = nullptr;
        const std::map<eSceneTypes, Scene*>* m_Scenes = nullptr;
    };

}
#endif // !_imgui_SceneViewer_H_
