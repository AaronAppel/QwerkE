#ifndef _imgui_SceneViewer_H_
#define _imgui_SceneViewer_H_

#include "../QwerkE_Framework/Headers/QwerkE_Enums.h"

#include <map>

class SceneManager;
class InputManager;
class FrameBufferObject;
class Scene;

namespace QwerkE {

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
        InputManager* m_Input = nullptr;
        SceneManager* m_SceneManager = nullptr;
        FrameBufferObject* m_FBO = nullptr;
        const std::map<eSceneTypes, Scene*>* m_Scenes = nullptr;
    };

}
#endif // !_imgui_SceneViewer_H_
